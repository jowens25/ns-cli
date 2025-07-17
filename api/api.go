package api

import (
	"NovusTimeServer/axi"
	"NovusTimeServer/web"
	"errors"
	"fmt"
	"io/fs"
	"log"
	"net/http"
	"os"
	"sync"
	"time"

	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
	"github.com/golang-jwt/jwt/v5"
	"golang.org/x/crypto/bcrypt"
	"gorm.io/driver/sqlite"
	"gorm.io/gorm"
	"gorm.io/gorm/logger"
)

const (
	JWT_SECRET = "your-secret-key"
	API_HOST   = "0.0.0.0"
	API_PORT   = ":5000"
	WEB_HOST   = "0.0.0.0"
	WEB_PORT   = ":3000"
	DB_PATH    = "./app.db"
)

var serialMutex sync.Mutex

//var NtpProperties = make(map[string]string)

func init() {
	os.Setenv(SNMP_CONFIG_PATH, "/etc/snmp/snmpd.conf")

	//NtpProperties["version"] = ""
	//NtpProperties["instance"] = ""
	//NtpProperties["mac"] = ""
	//NtpProperties["vlan_address"] = ""
	//NtpProperties["vlan_status"] = ""
	//NtpProperties["ip_mode"] = ""
	//NtpProperties["ip_address"] = ""
	//NtpProperties["unicast_mode"] = ""
	//NtpProperties["multicast_mode"] = ""
	//NtpProperties["broadcast_mode"] = ""
	//NtpProperties["status"] = ""
	//NtpProperties["stratum"] = ""
	//NtpProperties["poll_interval"] = ""
	//NtpProperties["precision"] = ""
	//NtpProperties["reference_id"] = ""
	//NtpProperties["leap59"] = ""
	//NtpProperties["leap59_inprogress"] = ""
	//NtpProperties["leap61"] = ""
	//NtpProperties["leap61_inprogress"] = ""
	//NtpProperties["utc_smearing"] = ""
	//NtpProperties["utc_offset_status"] = ""
	//NtpProperties["utc_offset_value"] = ""
	//NtpProperties["requests"] = ""
	//NtpProperties["responses"] = ""
	//NtpProperties["requests_dropped"] = ""
	//NtpProperties["broadcasts"] = ""
	//NtpProperties["clear_counters"] = ""

}

func RunApiServer() {

	jsRouter := gin.Default()

	webFS, err := fs.Sub(web.Files, "files")
	if err != nil {
		panic(err)
	}
	jsRouter.StaticFS("/", http.FS(webFS))

	go jsRouter.Run(WEB_HOST + WEB_PORT)

	initDataBase()
	r := gin.Default()
	r.SetTrustedProxies(nil)
	corsConfig := cors.DefaultConfig()
	corsConfig.AllowMethods = []string{"GET", "POST", "PUT", "DELETE", "OPTIONS", "PATCH"}
	corsConfig.AllowHeaders = []string{"Authorization", "Content-Type", "X-Request-ID"}
	corsConfig.AllowCredentials = true
	corsConfig.AllowAllOrigins = true
	//corsConfig.AllowOrigins = []string{
	//	"http://" + WEB_HOST + WEB_PORT,
	//	"http://" + API_HOST + API_PORT,
	//}

	//r.Use(corsConfig)
	r.Use(cors.New(corsConfig))

	// middleware
	//r.Use(gin.Logger())
	r.Use(gin.Recovery())

	// api version group
	v1 := r.Group("/api/v1")

	// public routes
	v1.POST("/auth/login", loginHandler)

	//r.Static("/", "")
	//r.GET("/users", usersHandler)
	// protected routes
	protected := v1.Group("/")
	protected.Use(authorizationMiddleware())
	{
		protected.POST("/logout", logoutHandler)

		protected.GET("/health", healthHandler)

		protected.GET("/users", getUsersHandler)
		protected.POST("/users", createUsersHandler)
		protected.DELETE("/users/:id", deleteUsersHandler)
		protected.PATCH("/users/:id", patchUsersHandler)

		//protected.GET("/snmp/:version/:id", getSnmpUserHandler)
		//protected.POST("/snmp/:version/:id", postSnmpUserHandler)
		//protected.PATCH("/snmp/:version/:id", patchSnmpUserHandler)
		//protected.DELETE("/snmp/:version/:id", deleteSnmpUserHandler)

		snmpGroup := protected.Group("/snmp")

		snmpGroup.GET("/v1v2c_user", readSnmpV1V2cUsers)
		snmpGroup.POST("/v1v2c_user", writeSnmpV1V2cUser)
		snmpGroup.PATCH("/v1v2c_user/:id", editSnmpV1V2cUser)
		snmpGroup.DELETE("/v1v2c_user/:id", deleteSnmpV1V2cUser)
		//
		snmpGroup.GET("v3_user", readSnmpV3Users)
		snmpGroup.POST("v3_user", writeSnmpV3User)
		snmpGroup.PATCH("v3_user/:id", editSnmpV3User)
		snmpGroup.DELETE("v3_user/:id", deleteSnmpV3User)

		//snmpGroup.GET("/status", readSnmpStatus)
		//snmpGroup.POST("/status", updateSnmpStatus)

		snmpGroup.GET("/info", readSnmpInfo)
		snmpGroup.PATCH("/info", writeSnmpInfo)
		snmpGroup.GET("/reset_config", resetSnmpConfig)

		protected.GET("/ntp/:prop", readNtpProperty)
		protected.POST("/ntp/:prop", writeNtpProperty)
		//r.GET("/:function/:resource/:id/:property", readHandler)
		////snmp / users / id / name
		////snmp / users / id / addr
		////snmp / v1users / id / rule
		////ntp / server / id / ip_addr
		////ntp / server / id /
		//protected.POST("/:function/:resource/:property", readHandler)
		//protected.PATCH("/:function/:resource/:property", readHandler)
		//protected.DELETE("/:function/:resource/:property", readHandler)
		//snmp/user/username
		//snmp/trap/setting
		//snmp/details/asdf

	}

	// 404 handler
	r.NoRoute(func(c *gin.Context) {
		c.JSON(http.StatusNotFound, gin.H{
			"error":   "Endpoint not found",
			"path":    c.Request.URL.Path,
			"method":  c.Request.Method,
			"message": "The requested resource could not be found",
		})
	})

	//readAllNtp()

	r.Run(API_HOST + API_PORT)
}

func readNtpProperty(c *gin.Context) {
	serialMutex.Lock()
	defer serialMutex.Unlock()
	property := c.Param("prop")
	operation := "read"
	module := "ntp-server"
	value := ""

	err := axi.Operate(&operation, &module, &property, &value)

	if err != nil {
		log.Println(err.Error())
	}

	c.JSON(http.StatusOK, gin.H{property: value})
}

func writeNtpProperty(c *gin.Context) {
	serialMutex.Lock()
	defer serialMutex.Unlock()
	var data map[string]string
	if err := c.ShouldBindJSON(&data); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}
	operation := "write"
	module := "ntp-server"
	property := c.Param("prop")
	//value := ""
	value := data[property]

	err := axi.Operate(&operation, &module, &property, &value)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusOK, gin.H{property: value})

}

func healthHandler(c *gin.Context) {
	sqlDB, err := db.DB()
	if err != nil {
		c.JSON(http.StatusServiceUnavailable, gin.H{
			"status": "unhealthy",
			"error":  "Database connection failed",
		})
		return
	}

	if err := sqlDB.Ping(); err != nil {
		c.JSON(http.StatusServiceUnavailable, gin.H{
			"status": "unhealthy",
			"error":  "Database ping failed",
		})
		return
	}
	c.JSON(http.StatusOK, gin.H{
		"status":    "healthy",
		"timestamp": time.Now(),
		"database":  "connected",
	})
}

func getUsersHandler(c *gin.Context) {

	requestID := c.GetHeader("X-Request-ID")
	//requestID := c.GetString("request_id")
	log.Printf("Request ID: %s\n", requestID)

	var users []struct {
		ID       int    `json:"id"`
		Role     string `json:"role"`
		Username string `json:"username"`
		Email    string `json:"email"`
	}

	result := db.Model(&User{}).Select("id, role, username, email").Find(&users)

	if result.Error != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"users":       users,
		"total_users": len(users),
		"server_time": time.Now(),
		"database":    "SQLite",
		"request_id":  requestID, // Optionally include in the response

	})

}

func createUsersHandler(c *gin.Context) {

	var newUser User

	if err := c.ShouldBindJSON(&newUser); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	// Add validation here
	if newUser.Username == "" || newUser.Email == "" {
		c.JSON(http.StatusBadRequest, gin.H{"error": "Username and email are required"})
		return
	}

	if !(newUser.Role == "admin" || newUser.Role == "viewer") {
		c.JSON(http.StatusBadRequest, gin.H{"error": "role must be 'admin' or 'viewer'"})
		return
	}

	hashedPassword, err := bcrypt.GenerateFromPassword([]byte(newUser.Password), bcrypt.DefaultCost)

	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to hash password"})
		return
	}
	newUser.Password = string(hashedPassword)

	result := db.Create(&newUser)
	if result.Error != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
		return
	}

	c.JSON(http.StatusCreated, gin.H{
		"message": "User created successfully",
		"user": gin.H{
			"id":       newUser.ID,
			"role":     newUser.Role,
			"username": newUser.Username,
			"email":    newUser.Email,
		},
	})
}

func deleteUsersHandler(c *gin.Context) {

	userID := c.Param("id")

	var userToDelete User

	if err := db.First(&userToDelete, userID).Error; err != nil {
		c.JSON(http.StatusNotFound, gin.H{"error": "User not found"})
		return
	}

	if userToDelete.Role == "viewer" {
		if err := db.Delete(&userToDelete).Error; err != nil {
			c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
			return
		}
	}

	if userToDelete.Role == "admin" {

		var count int64

		db.Model(&User{}).Where("role = ?", "admin").Count(&count)

		if count > 1 {
			if err := db.Delete(&userToDelete).Error; err != nil {
				c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
				return
			}

			c.JSON(http.StatusCreated, gin.H{
				"message": "User deleted successfully",
			})

		} else {
			c.JSON(http.StatusUnprocessableEntity, gin.H{
				"error": "Cannot delete the last admin",
			})
			return
		}

	}

}

func patchUsersHandler(c *gin.Context) {
	userID := c.Param("id")

	var existingUser User
	if err := db.First(&existingUser, userID).Error; err != nil {
		if errors.Is(err, gorm.ErrRecordNotFound) {
			c.JSON(http.StatusNotFound, gin.H{"error": "User not found"})
		} else {
			c.JSON(http.StatusInternalServerError, gin.H{"error": "Database error"})
		}
		return
	}

	var updateData User
	if err := c.ShouldBindJSON(&updateData); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	updates := make(map[string]interface{})

	if updateData.Username != "" {
		updates["username"] = updateData.Username
	}

	if updateData.Email != "" {
		updates["email"] = updateData.Email
	}

	if updateData.Role != "" {
		if !(updateData.Role == "admin" || updateData.Role == "viewer") {
			c.JSON(http.StatusBadRequest, gin.H{"error": "role must be 'admin' or 'viewer'"})
			return
		}
		updates["role"] = updateData.Role
	}

	if updateData.Password != "" {
		hashedPassword, err := bcrypt.GenerateFromPassword([]byte(updateData.Password), bcrypt.DefaultCost)
		if err != nil {
			c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to hash password"})
			return
		}
		updates["password"] = string(hashedPassword)
	}

	// Check if there's anything to update
	if len(updates) == 0 {
		c.JSON(http.StatusOK, gin.H{"message": "No update"})
		return
	}

	// Perform the update using the user ID
	result := db.Model(&existingUser).Updates(updates)
	if result.Error != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
		return
	}

	var updatedUser User
	if err := db.First(&updatedUser, userID).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to fetch updated user"})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"message": "User updated successfully",
		"user": gin.H{
			"id":       updatedUser.ID,
			"role":     updatedUser.Role,
			"username": updatedUser.Username,
			"email":    updatedUser.Email,
		},
	})
}

// ==============================================

//func deleteSnmpV1V2cUser(c *gin.Context) {
//
//	id := c.Param("id")
//	var userToDelete SnmpV1V2cUser
//
//	if err := db.First(&userToDelete, id).Error; err != nil {
//		c.JSON(http.StatusNotFound, gin.H{"error": "SNMP user not found"})
//		return
//	}
//
//	if err := db.Delete(&userToDelete).Error; err != nil {
//		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
//		return
//	}
//
//	c.JSON(http.StatusCreated, gin.H{
//		"message": "User deleted successfully",
//	})
//
//}

// ======================================

func createSnmpTrap(c *gin.Context) {}
func readSnmpTrap(c *gin.Context)   {}
func updateSnmpTrap(c *gin.Context) {}
func deleteSnmpTrap(c *gin.Context) {}

func logoutHandler(c *gin.Context) {
	c.JSON(http.StatusOK, gin.H{"message": "Logged out successfully"})
}

func loginHandler(c *gin.Context) {
	var request LoginRequest

	if err := c.ShouldBindJSON(&request); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{
			"error":   "Invalid request format",
			"details": err.Error(),
		})
		fmt.Println(err.Error())
		return
	}

	var user User

	result := db.Where("username = ?", request.Username).First(&user)
	fmt.Println("result from db: ", result)
	if result.Error != nil {
		c.JSON(http.StatusUnauthorized, gin.H{
			"error": "Invalid username or password",
		})
		return
	}

	if !checkPasswordHash(request.Password, user.Password) {
		c.JSON(http.StatusUnauthorized, gin.H{
			"error": "Invalid username or password",
		})
		return
	}

	token, err := generateJWT(&user)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": "Failed to generate token",
		})
		return
	}

	response := LoginResponse{
		Token: token,
		User:  user,
	}

	c.JSON(http.StatusOK, response)
}

func authorizationMiddleware() gin.HandlerFunc {
	return func(c *gin.Context) {
		tokenString := c.GetHeader("Authorization")

		if tokenString == "" {
			c.JSON(http.StatusUnauthorized, gin.H{"error": "Missing Authorization header"})
			c.Abort()
			return
		}

		if len(tokenString) > 7 && tokenString[:7] == "Bearer " {
			tokenString = tokenString[7:]
		}

		claims := &Claims{}
		token, err := jwt.ParseWithClaims(tokenString, claims, func(token *jwt.Token) (interface{}, error) {
			return []byte(JWT_SECRET), nil
		})

		if err != nil || !token.Valid {
			c.JSON(http.StatusUnauthorized, gin.H{"error": "Invalid token"})
			c.Abort()
			return
		}
		c.Set("user_id", claims.UserID)
		c.Set("username", claims.Username)
		c.Next()
	}
}

func hashPassword(password string) (string, error) {
	bytes, err := bcrypt.GenerateFromPassword([]byte(password), bcrypt.DefaultCost)
	return string(bytes), err
}

func checkPasswordHash(password, hash string) bool {
	err := bcrypt.CompareHashAndPassword([]byte(hash), []byte(password))
	return err == nil
}

func generateJWT(user *User) (string, error) {
	claims := &Claims{
		UserID:   user.ID,
		UserRole: user.Role,
		Username: user.Username,
		RegisteredClaims: jwt.RegisteredClaims{
			ExpiresAt: jwt.NewNumericDate(time.Now().Add(2 * time.Hour)),
			IssuedAt:  jwt.NewNumericDate(time.Now()),
			NotBefore: jwt.NewNumericDate(time.Now()),
		},
	}

	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claims)
	return token.SignedString([]byte(JWT_SECRET))
}

var db *gorm.DB

func initDataBase() {

	var err error

	db, err = gorm.Open(sqlite.Open(DB_PATH), &gorm.Config{
		Logger: logger.Default.LogMode(logger.Info),
	})
	if err != nil {
		log.Fatal("Failed to connect to database: ", err)
	}

	err = db.AutoMigrate(&User{}, &SnmpV1V2cUser{}, &SnmpV3User{}, &Ntp{})

	if err != nil {
		log.Fatal("Failed to migrate database: ", err)
	}

	createDefaultUser()

	log.Println("Database initialized successfully")

}

func createDefaultUser() {

	var userCount int64
	db.Model(&User{}).Count(&userCount)

	if userCount == 0 {
		adminPassword, _ := bcrypt.GenerateFromPassword([]byte("admin"), bcrypt.DefaultCost)

		user := User{

			Username: "admin",
			Role:     "admin",
			Email:    "admin@novuspower.com",
			Password: string(adminPassword),
		}

		db.Create(&user)

		user = User{

			Username: "viewer",
			Role:     "viewer",
			Email:    "viewer@novuspower.com",
			Password: string(adminPassword),
		}

		db.Create(&user)

		user = User{

			Username: "factory",
			Role:     "admin",
			Email:    "factory@novuspower.com",
			Password: string(adminPassword),
		}

		db.Create(&user)

		snmpV1V2User := SnmpV1V2cUser{
			Version:   "v2c",
			GroupName: "read_write",
			Community: "myCommunity",
			//	IpVersion:  "ipv4",
			//	Ip4Address: "10.1.10.220",
		}

		db.Create(&snmpV1V2User)

	}

}

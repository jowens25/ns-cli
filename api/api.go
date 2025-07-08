package api

import (
	"NovusTimeServer/web"
	"errors"
	"fmt"
	"io/fs"
	"log"
	"net/http"
	"os/exec"
	"strings"
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
	JWT_SECRET = "your-secret-key-change-this-in-production"
	API_HOST   = "10.1.10.205"
	API_PORT   = ":5000"
	WEB_HOST   = "10.1.10.96"
	WEB_PORT   = ":3000"
	DB_PATH    = "./app.db"
)

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
	r.POST("/login", loginHandler)

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

		protected.GET("/snmp_v1v2c", readSnmpV1V2cUser)
		protected.POST("/snmp_v1v2c", createSnmpV1V2cUser)
		protected.PATCH("/snmp_v1v2c/:id", updateSnmpV1V2cUser)
		protected.DELETE("/snmp_v1v2c/:id", deleteSnmpV1V2cUser)

		protected.GET("/snmp_v3", readSnmpV3User)
		protected.POST("/snmp_v3", createSnmpV3User)
		protected.PATCH("/snmp_v3/:id", updateSnmpV3User)
		protected.DELETE("/snmp_v3/:id", deleteSnmpV3User)

		protected.GET("snmp/status", getSnmpStatusHandler)
		protected.POST("snmp/status", postSnmpStatusHandler)

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
	r.Run(API_HOST + API_PORT)
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

func getSnmpStatusHandler(c *gin.Context) {

	cmd := exec.Command("systemctl", "is-active", "snmpd")
	out, err := cmd.CombinedOutput()
	log.Println(err)
	log.Println("this the output: ", strings.TrimSpace(string(out)))

	status := strings.TrimSpace(string(out))

	c.JSON(http.StatusOK, gin.H{
		"status": status,
	})

}

func postSnmpStatusHandler(c *gin.Context) {

	var newSnmpStatus SnmpStatus

	if err := c.ShouldBindJSON(&newSnmpStatus); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	cmd := exec.Command("systemctl", newSnmpStatus.Status, "snmpd")
	out, err := cmd.CombinedOutput()
	log.Println(err)
	log.Println("this the output: ", strings.TrimSpace(string(out)))

	newSnmpStatus.Status = strings.TrimSpace(string(out))

	c.JSON(http.StatusOK, gin.H{
		"message": "Snmp enabled or disabled",
		"status": gin.H{
			"status": newSnmpStatus.Status,
		},
	})

}

// ==============================================

func createSnmpV1V2cUser(c *gin.Context) {
	var count int64
	var snmpV1V2cUser SnmpV1V2cUser

	if err := c.ShouldBindJSON(&snmpV1V2cUser); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	db.Model(&SnmpV1V2cUser{}).Count(&count)

	snmpV1V2cUser.ID = count + 1

	result := db.Create(&snmpV1V2cUser)
	if result.Error != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
		return
	}

	c.JSON(http.StatusCreated, gin.H{
		"message": "SNMP V1/V2c User Created",
		"snmp_v1_v2c": gin.H{
			"id":          snmpV1V2cUser.ID,
			"version":     snmpV1V2cUser.Version,
			"group_name":  snmpV1V2cUser.GroupName,
			"community":   snmpV1V2cUser.Community,
			"ip_version":  snmpV1V2cUser.IpVersion,
			"ip4_address": snmpV1V2cUser.Ip4Address,
			"ip6_address": snmpV1V2cUser.Ip6Address,
		},
	})
}

func readSnmpV1V2cUser(c *gin.Context) {

	var snmpV1V2cUsers []SnmpV1V2cUser

	result := db.Model(&SnmpV1V2cUser{}).Find(&snmpV1V2cUsers)

	if result.Error != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"snmp_v1v2c_users": snmpV1V2cUsers,
		"total_users":      len(snmpV1V2cUsers),
	})
}

func updateSnmpV1V2cUser(c *gin.Context) {

	fmt.Println("update snmp... ")
	id := c.Param("id")

	var existingUser SnmpV1V2cUser
	if err := db.First(&existingUser, id).Error; err != nil {
		if errors.Is(err, gorm.ErrRecordNotFound) {
			c.JSON(http.StatusNotFound, gin.H{"error": "User not found"})
		} else {
			c.JSON(http.StatusInternalServerError, gin.H{"error": "Database error"})
		}
		return
	}

	var updateData SnmpV1V2cUser
	if err := c.ShouldBindJSON(&updateData); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	updates := make(map[string]interface{})

	if updateData.Version != "" {
		if !(updateData.Version == "v1" || updateData.Version == "v2c") {
			c.JSON(http.StatusBadRequest, gin.H{"error": "wrong version entry"})
			return
		}
		updates["version"] = updateData.Version
	}

	if updateData.GroupName != "" {
		if !(updateData.GroupName == "read_only" || updateData.GroupName == "read_write") {
			c.JSON(http.StatusBadRequest, gin.H{"error": "wrong group name entry"})
			return
		}
		updates["group_name"] = updateData.GroupName
	}

	if updateData.Community != "" {
		updates["community"] = updateData.Community
	}

	if updateData.IpVersion != "" {
		if !(updateData.IpVersion == "ipv4" || updateData.IpVersion == "ipv6") {
			c.JSON(http.StatusBadRequest, gin.H{"error": "wrong ip version entry"})
			return
		}
		updates["ip_version"] = updateData.IpVersion
	}

	if updateData.Ip4Address != "" {
		updates["ip4_address"] = updateData.Ip4Address
	}

	if updateData.Ip6Address != "" {
		updates["ip6_address"] = updateData.Ip6Address
	}

	// Check if there's anything to update
	if len(updates) == 0 {
		c.JSON(http.StatusInternalServerError, gin.H{"message": "No update"})
		return
	}

	// Perform the update using the user ID
	result := db.Model(&existingUser).Updates(updates)
	if result.Error != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
		return
	}

	var updatedUser SnmpV1V2cUser
	if err := db.First(&updatedUser, id).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to fetch updated user"})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"message": "User updated successfully",
		"user": gin.H{
			"id":          updatedUser.ID,
			"version":     updatedUser.Version,
			"group_name":  updatedUser.GroupName,
			"community":   updatedUser.Community,
			"ip_version":  updatedUser.IpVersion,
			"ip4_address": updatedUser.Ip4Address,
			"ip6_address": updatedUser.Ip6Address,
		},
	})
}

func deleteSnmpV1V2cUser(c *gin.Context) {

	id := c.Param("id")
	var userToDelete SnmpV1V2cUser

	if err := db.First(&userToDelete, id).Error; err != nil {
		c.JSON(http.StatusNotFound, gin.H{"error": "SNMP user not found"})
		return
	}

	if err := db.Delete(&userToDelete).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	c.JSON(http.StatusCreated, gin.H{
		"message": "User deleted successfully",
	})

}

// ======================================

func createSnmpV3User(c *gin.Context) {
	var count int64
	var snmpV3User SnmpV3User

	if err := c.ShouldBindJSON(&snmpV3User); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	db.Model(&SnmpV3User{}).Count(&count)

	snmpV3User.ID = count + 1

	result := db.Create(&snmpV3User)
	if result.Error != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
		return
	}

	c.JSON(http.StatusCreated, gin.H{
		"message": "SNMP V3 User Created",
		"snmp_v3_user": gin.H{
			"id":              snmpV3User.ID,
			"user_name":       snmpV3User.UserName,
			"auth_type":       snmpV3User.AuthType,
			"auth_passphrase": snmpV3User.AuthPassphrase,
			"priv_type":       snmpV3User.PrivType,
			"priv_passphrase": snmpV3User.PrivPassphrase,
			"group_name":      snmpV3User.GroupName,
		},
	})

	// do the fun part

	addSnmpdV3Conf(snmpV3User)
}
func readSnmpV3User(c *gin.Context) {
	var snmpV3Users []SnmpV3User

	result := db.Model(&SnmpV3User{}).Find(&snmpV3Users)

	if result.Error != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"snmp_v3_users": snmpV3Users,
		"total_users":   len(snmpV3Users),
	})
}
func updateSnmpV3User(c *gin.Context) {
	fmt.Println("update snmp... ")
	id := c.Param("id")

	var existingUser SnmpV3User
	if err := db.First(&existingUser, id).Error; err != nil {
		if errors.Is(err, gorm.ErrRecordNotFound) {
			c.JSON(http.StatusNotFound, gin.H{"error": "User not found"})
		} else {
			c.JSON(http.StatusInternalServerError, gin.H{"error": "Database error"})
		}
		return
	}

	var updateData SnmpV3User
	if err := c.ShouldBindJSON(&updateData); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	updates := make(map[string]interface{})

	// user name
	if updateData.UserName != "" {
		updates["user_name"] = updateData.UserName
	}

	// auth type
	if updateData.AuthType != "" {
		if !(updateData.AuthType == "MD5" || updateData.AuthType == "SHA") {
			c.JSON(http.StatusBadRequest, gin.H{"error": "wrong auth entry"})
			return
		}
		updates["auth_type"] = updateData.AuthType
	}
	// auth pass
	if updateData.AuthPassphrase != "" {
		updates["auth_passphrase"] = updateData.AuthPassphrase
	}

	// priv type
	if updateData.PrivType != "" {
		if !(updateData.PrivType == "AES" || updateData.PrivType == "DES") {
			c.JSON(http.StatusBadRequest, gin.H{"error": "wrong priv entry"})
			return
		}
		updates["priv_type"] = updateData.PrivType
	}

	// priv pass
	if updateData.PrivPassphrase != "" {
		updates["priv_passphrase"] = updateData.PrivPassphrase
	}

	// priv type
	if updateData.GroupName != "" {
		if !(updateData.GroupName == "read_only" || updateData.GroupName == "read_write") {
			c.JSON(http.StatusBadRequest, gin.H{"error": "wrong group entry"})
			return
		}
		updates["group_name"] = updateData.GroupName
	}

	// Check if there's anything to update
	if len(updates) == 0 {
		c.JSON(http.StatusInternalServerError, gin.H{"message": "No update"})
		return
	}

	// Perform the update using the user ID
	result := db.Model(&existingUser).Updates(updates)
	if result.Error != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
		return
	}

	var updatedUser SnmpV3User
	if err := db.First(&updatedUser, id).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to fetch updated user"})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"message": "User updated successfully",
		"user": gin.H{
			"id":              updatedUser.ID,
			"user_name":       updatedUser.UserName,
			"auth_type":       updatedUser.AuthType,
			"auth_passphrase": updatedUser.AuthPassphrase,
			"priv_type":       updatedUser.PrivType,
			"priv_passphrase": updatedUser.PrivPassphrase,
			"group_name":      updatedUser.GroupName,
		},
	})
}
func deleteSnmpV3User(c *gin.Context) {
	id := c.Param("id")
	var userToDelete SnmpV3User

	if err := db.First(&userToDelete, id).Error; err != nil {
		c.JSON(http.StatusNotFound, gin.H{"error": "SNMP user not found"})
		return
	}

	if err := db.Delete(&userToDelete).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	c.JSON(http.StatusCreated, gin.H{
		"message": "User deleted successfully",
	})
}

// =============== snmp v3 linux management ======================
// net-snmp-create-v3-user [-ro] [-a authpass] [-x privpass] [-X DES|AES] [username]

func addSnmpdV3Conf(user SnmpV3User) {

	exec.Command("systemctl", "stop", "snmpd")

	if user.GroupName == "read_only" {
		cmd := exec.Command("net-snmp-create-v3-user", "-ro", "-a", user.AuthPassphrase, "-x", user.PrivPassphrase, "-X", user.PrivType, user.UserName)
		out, err := cmd.CombinedOutput()
		log.Println(err)
		log.Println("this the output: ", strings.TrimSpace(string(out)))
	}

	if user.GroupName == "read_write" {
		cmd := exec.Command("net-snmp-create-v3-user", "-a", user.AuthPassphrase, "-x", user.PrivPassphrase, "-X", user.PrivType, user.UserName)
		out, err := cmd.CombinedOutput()
		log.Println(err)
		log.Println("this the output: ", strings.TrimSpace(string(out)))
	}

	exec.Command("systemctl", "start", "snmpd")

}

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

	err = db.AutoMigrate(&User{}, &SnmpV1V2cUser{}, &SnmpV3User{})

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
			Version:    "v2c",
			GroupName:  "read_write",
			Community:  "myCommunity",
			IpVersion:  "ipv4",
			Ip4Address: "10.1.10.220",
		}

		db.Create(&snmpV1V2User)

	}

}

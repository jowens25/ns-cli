package api

import (
	"NovusTimeServer/web"
	"errors"
	"io/fs"
	"log"
	"net/http"
	"time"

	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
	"github.com/golang-jwt/jwt/v5"
	"golang.org/x/crypto/bcrypt"
	"gorm.io/gorm"
)

const (
	JWT_SECRET = "your-secret-key-change-this-in-production"
	API_HOST   = "10.1.10.205"
	API_PORT   = ":5000"
	WEB_HOST   = "10.1.10.96"
	WEB_PORT   = ":3000"
	DB_PATH    = "./app.db"
)

// JWT Claims
type Claims struct {
	UserID   uint   `json:"user_id"`
	UserRole string `json:"role"`
	Username string `json:"username"`
	jwt.RegisteredClaims
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

		protected.GET("/snmp/:version/:id", getSnmpUserHandler)
		protected.POST("/snmp/:version/:id", postSnmpUserHandler)
		protected.PATCH("/snmp/:version/:id", patchSnmpUserHandler)
		protected.DELETE("/snmp/:version/:id", deleteSnmpUserHandler)

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
	sqlDB, err := Db.DB()
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

	result := Db.Model(&User{}).Select("id, role, username, email").Find(&users)

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

	result := Db.Create(&newUser)
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

	if err := Db.First(&userToDelete, userID).Error; err != nil {
		c.JSON(http.StatusNotFound, gin.H{"error": "User not found"})
		return
	}

	if userToDelete.Role == "viewer" {
		if err := Db.Delete(&userToDelete).Error; err != nil {
			c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
			return
		}
	}

	if userToDelete.Role == "admin" {

		var count int64

		Db.Model(&User{}).Where("role = ?", "admin").Count(&count)

		if count > 1 {
			if err := Db.Delete(&userToDelete).Error; err != nil {
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
	if err := Db.First(&existingUser, userID).Error; err != nil {
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
	result := Db.Model(&existingUser).Updates(updates)
	if result.Error != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
		return
	}

	var updatedUser User
	if err := Db.First(&updatedUser, userID).Error; err != nil {
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

func getSnmpUserHandler(c *gin.Context) {

}
func postSnmpUserHandler(c *gin.Context) {

}
func patchSnmpUserHandler(c *gin.Context) {

}
func deleteSnmpUserHandler(c *gin.Context) {

}

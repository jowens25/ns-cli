package api

import (
	"log"
	"net/http"
	"time"

	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
	"github.com/golang-jwt/jwt/v5"
	"golang.org/x/crypto/bcrypt"
)

const (
	JWT_SECRET = "your-secret-key-change-this-in-production"
	PORT       = ":8080"
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

	initDataBase()
	r := gin.Default()
	r.SetTrustedProxies(nil)
	corsConfig := cors.DefaultConfig()
	corsConfig.AllowMethods = []string{"GET", "POST", "PUT", "DELETE", "OPTIONS"}
	corsConfig.AllowHeaders = []string{"Authorization", "Content-Type", "X-Request-ID"}
	corsConfig.AllowCredentials = true
	corsConfig.AllowOrigins = []string{"http://localhost:52374"}

	//r.Use(corsConfig)
	r.Use(cors.New(corsConfig))

	// middleware
	//r.Use(gin.Logger())
	r.Use(gin.Recovery())

	// api version group
	v1 := r.Group("/api/v1")

	// public routes
	r.POST("/login", loginHandler)
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
	r.Run(PORT)
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

	if err := Db.Delete(&userToDelete).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	c.JSON(http.StatusCreated, gin.H{
		"message": "User deleted successfully",
	})
}

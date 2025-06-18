package jwt

import (
	"fmt"
	"log"
	"net/http"
	"strconv"
	"time"

	"github.com/gin-gonic/gin"
	"github.com/golang-jwt/jwt/v5"
	"golang.org/x/crypto/bcrypt"
	"gorm.io/driver/sqlite"
	"gorm.io/gorm"
	"gorm.io/gorm/logger"
)

// models
// Database Models

type Post struct {
	ID        uint      `json:"id" gorm:"primaryKey"`
	Title     string    `json:"title" gorm:"not null"`
	Content   string    `json:"content" gorm:"type:text"`
	AuthorID  uint      `json:"author_id" gorm:"not null"`
	Author    User      `json:"author,omitempty" gorm:"foreignKey:AuthorID"`
	CreatedAt time.Time `json:"created_at"`
	UpdatedAt time.Time `json:"updated_at"`
}

// Request/Response DTOs
type LoginRequest struct {
	Username string `json:"username" binding:"required"`
	Password string `json:"password" binding:"required"`
}

type RegisterRequest struct {
	Username string `json:"username" binding:"required,min=3,max=50"`
	Email    string `json:"email" binding:"required,email"`
	Password string `json:"password" binding:"required,min=6"`
}

type CreatePostRequest struct {
	Title   string `json:"title" binding:"required,max=200"`
	Content string `json:"content" binding:"required"`
}

type UpdatePostRequest struct {
	Title   string `json:"title,omitempty"`
	Content string `json:"content,omitempty"`
}

type UpdateProfileRequest struct {
	Email string `json:"email,omitempty" binding:"omitempty,email"`
}

type ChangePasswordRequest struct {
	CurrentPassword string `json:"current_password" binding:"required"`
	NewPassword     string `json:"new_password" binding:"required,min=6"`
}

type LoginResponse struct {
	Token string `json:"token"`
	User  User   `json:"user"`
}

type PaginatedResponse struct {
	Data       interface{} `json:"data"`
	Page       int         `json:"page"`
	Limit      int         `json:"limit"`
	Total      int64       `json:"total"`
	TotalPages int         `json:"total_pages"`
}

// JWT Claims
//type Claims struct {
//	UserID   uint   `json:"user_id"`
//	Username string `json:"username"`
//	jwt.RegisteredClaims
//}

// Configuration
//const (
//	JWT_SECRET = "your-secret-key-change-this-in-production"
//	PORT       = ":8080"
//	DB_PATH    = "./app.db"
//)

// Global database instance
var db *gorm.DB

// Database initialization
func initDatabase() {
	var err error
	//Connect to SQLite database
	db, err = gorm.Open(sqlite.Open(DB_PATH), &gorm.Config{
		Logger: logger.Default.LogMode(logger.Info),
	})
	if err != nil {
		log.Fatal("Failed to connect to database:", err)
	}

	// Auto-migrate the schema
	err = db.AutoMigrate(&User{}, &Post{})
	if err != nil {
		log.Fatal("Failed to migrate database:", err)
	}

	// Create default admin user if not exists
	createDefaultUsers()

	log.Println("Database initialized successfully")
}

func createDefaultUsers() {
	var count int64
	db.Model(&User{}).Count(&count)

	if count == 0 {
		adminPassword, _ := bcrypt.GenerateFromPassword([]byte("admin123"), bcrypt.DefaultCost)
		userPassword, _ := bcrypt.GenerateFromPassword([]byte("user123"), bcrypt.DefaultCost)

		users := []User{
			{
				Username: "admin",
				Email:    "admin@example.com",
				Password: string(adminPassword),
			},
			{
				Username: "user",
				Email:    "user@example.com",
				Password: string(userPassword),
			},
		}

		for _, user := range users {
			db.Create(&user)
		}

		// Create sample posts
		var adminUser User
		db.Where("username = ?", "admin").First(&adminUser)

		posts := []Post{
			{
				Title:    "Welcome to our platform",
				Content:  "This is the first post on our platform. Welcome!",
				AuthorID: adminUser.ID,
			},
			{
				Title:    "Getting Started Guide",
				Content:  "Here's how to get started with our application...",
				AuthorID: adminUser.ID,
			},
		}

		for _, post := range posts {
			db.Create(&post)
		}

		log.Println("Default users and posts created")
	}
}

// Helper functions
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
		Username: user.Username,
		RegisteredClaims: jwt.RegisteredClaims{
			ExpiresAt: jwt.NewNumericDate(time.Now().Add(24 * time.Hour)),
			IssuedAt:  jwt.NewNumericDate(time.Now()),
			NotBefore: jwt.NewNumericDate(time.Now()),
		},
	}

	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claims)
	return token.SignedString([]byte(JWT_SECRET))
}

// Middleware
func jwtAuthMiddleware() gin.HandlerFunc {
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

// Authentication Handlers
//func loginHandler(c *gin.Context) {
//	var req LoginRequest
//
//	if err := c.ShouldBindJSON(&req); err != nil {
//		c.JSON(http.StatusBadRequest, gin.H{
//			"error":   "Invalid request format",
//			"details": err.Error(),
//		})
//		return
//	}
//
//	var user User
//	result := db.Where("username = ?", req.Username).First(&user)
//	if result.Error != nil {
//		c.JSON(http.StatusUnauthorized, gin.H{
//			"error": "Invalid username or password",
//		})
//		return
//	}
//
//	if !checkPasswordHash(req.Password, user.Password) {
//		c.JSON(http.StatusUnauthorized, gin.H{
//			"error": "Invalid username or password",
//		})
//		return
//	}
//
//	token, err := generateJWT(&user)
//	if err != nil {
//		c.JSON(http.StatusInternalServerError, gin.H{
//			"error": "Failed to generate token",
//		})
//		return
//	}
//
//	response := LoginResponse{
//		Token: token,
//		User:  user,
//	}
//
//	c.JSON(http.StatusOK, response)
//}

func registerHandler(c *gin.Context) {
	var req RegisterRequest

	if err := c.ShouldBindJSON(&req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{
			"error":   "Invalid request format",
			"details": err.Error(),
		})
		return
	}

	// Check if username already exists
	var existingUser User
	if err := db.Where("username = ? OR email = ?", req.Username, req.Email).First(&existingUser).Error; err == nil {
		c.JSON(http.StatusConflict, gin.H{
			"error": "Username or email already exists",
		})
		return
	}

	// Hash password
	hashedPassword, err := hashPassword(req.Password)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": "Failed to process password",
		})
		return
	}

	// Create new user
	newUser := User{
		Username: req.Username,
		Email:    req.Email,
		Password: hashedPassword,
	}

	if err := db.Create(&newUser).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": "Failed to create user",
		})
		return
	}

	c.JSON(http.StatusCreated, gin.H{
		"message": "User registered successfully",
		"user":    newUser,
	})
}

func logoutHandler(c *gin.Context) {
	// In a real application, you might blacklist the token
	c.JSON(http.StatusOK, gin.H{
		"message": "Logged out successfully",
	})
}

// User Profile Handlers
func getProfileHandler(c *gin.Context) {
	userID, _ := c.Get("user_id")

	var user User
	if err := db.First(&user, userID).Error; err != nil {
		c.JSON(http.StatusNotFound, gin.H{
			"error": "User not found",
		})
		return
	}

	c.JSON(http.StatusOK, user)
}

func updateProfileHandler(c *gin.Context) {
	var req UpdateProfileRequest
	userID, _ := c.Get("user_id")

	if err := c.ShouldBindJSON(&req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{
			"error":   "Invalid request format",
			"details": err.Error(),
		})
		return
	}

	var user User
	if err := db.First(&user, userID).Error; err != nil {
		c.JSON(http.StatusNotFound, gin.H{
			"error": "User not found",
		})
		return
	}

	// Update fields if provided
	if req.Email != "" {
		// Check if email is already taken by another user
		var existingUser User
		if err := db.Where("email = ? AND id != ?", req.Email, userID).First(&existingUser).Error; err == nil {
			c.JSON(http.StatusConflict, gin.H{
				"error": "Email already exists",
			})
			return
		}
		user.Email = req.Email
	}

	if err := db.Save(&user).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": "Failed to update profile",
		})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"message": "Profile updated successfully",
		"user":    user,
	})
}

func changePasswordHandler(c *gin.Context) {
	var req ChangePasswordRequest
	userID, _ := c.Get("user_id")

	if err := c.ShouldBindJSON(&req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{
			"error":   "Invalid request format",
			"details": err.Error(),
		})
		return
	}

	var user User
	if err := db.First(&user, userID).Error; err != nil {
		c.JSON(http.StatusNotFound, gin.H{
			"error": "User not found",
		})
		return
	}

	// Verify current password
	if !checkPasswordHash(req.CurrentPassword, user.Password) {
		c.JSON(http.StatusUnauthorized, gin.H{
			"error": "Current password is incorrect",
		})
		return
	}

	// Hash new password
	hashedPassword, err := hashPassword(req.NewPassword)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": "Failed to process new password",
		})
		return
	}

	// Update password
	user.Password = hashedPassword
	if err := db.Save(&user).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": "Failed to update password",
		})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"message": "Password changed successfully",
	})
}

// Post CRUD Handlers
func getPostsHandler(c *gin.Context) {
	page, _ := strconv.Atoi(c.DefaultQuery("page", "1"))
	limit, _ := strconv.Atoi(c.DefaultQuery("limit", "10"))

	if page < 1 {
		page = 1
	}
	if limit < 1 || limit > 100 {
		limit = 10
	}

	var posts []Post
	var total int64

	// Get total count
	db.Model(&Post{}).Count(&total)

	// Get paginated posts with author information
	offset := (page - 1) * limit
	if err := db.Preload("Author").Offset(offset).Limit(limit).Order("created_at DESC").Find(&posts).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": "Failed to fetch posts",
		})
		return
	}

	totalPages := int((total + int64(limit) - 1) / int64(limit))

	c.JSON(http.StatusOK, PaginatedResponse{
		Data:       posts,
		Page:       page,
		Limit:      limit,
		Total:      total,
		TotalPages: totalPages,
	})
}

func getPostHandler(c *gin.Context) {
	id, err := strconv.Atoi(c.Param("id"))
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{
			"error": "Invalid post ID",
		})
		return
	}

	var post Post
	if err := db.Preload("Author").First(&post, id).Error; err != nil {
		c.JSON(http.StatusNotFound, gin.H{
			"error": "Post not found",
		})
		return
	}

	c.JSON(http.StatusOK, post)
}

func createPostHandler(c *gin.Context) {
	var req CreatePostRequest
	userID, _ := c.Get("user_id")

	if err := c.ShouldBindJSON(&req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{
			"error":   "Invalid request format",
			"details": err.Error(),
		})
		return
	}

	newPost := Post{
		Title:    req.Title,
		Content:  req.Content,
		AuthorID: userID.(uint),
	}

	if err := db.Create(&newPost).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": "Failed to create post",
		})
		return
	}

	// Load author information
	db.Preload("Author").First(&newPost, newPost.ID)

	c.JSON(http.StatusCreated, gin.H{
		"message": "Post created successfully",
		"post":    newPost,
	})
}

func updatePostHandler(c *gin.Context) {
	var req UpdatePostRequest
	userID, _ := c.Get("user_id")

	id, err := strconv.Atoi(c.Param("id"))
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{
			"error": "Invalid post ID",
		})
		return
	}

	if err := c.ShouldBindJSON(&req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{
			"error":   "Invalid request format",
			"details": err.Error(),
		})
		return
	}

	var post Post
	if err := db.First(&post, id).Error; err != nil {
		c.JSON(http.StatusNotFound, gin.H{
			"error": "Post not found",
		})
		return
	}

	// Check if user owns the post
	if post.AuthorID != userID.(uint) {
		c.JSON(http.StatusForbidden, gin.H{
			"error": "You can only update your own posts",
		})
		return
	}

	// Update fields if provided
	if req.Title != "" {
		post.Title = req.Title
	}
	if req.Content != "" {
		post.Content = req.Content
	}

	if err := db.Save(&post).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": "Failed to update post",
		})
		return
	}

	// Load author information
	db.Preload("Author").First(&post, post.ID)

	c.JSON(http.StatusOK, gin.H{
		"message": "Post updated successfully",
		"post":    post,
	})
}

func deletePostHandler(c *gin.Context) {
	userID, _ := c.Get("user_id")

	id, err := strconv.Atoi(c.Param("id"))
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{
			"error": "Invalid post ID",
		})
		return
	}

	var post Post
	if err := db.First(&post, id).Error; err != nil {
		c.JSON(http.StatusNotFound, gin.H{
			"error": "Post not found",
		})
		return
	}

	// Check if user owns the post
	if post.AuthorID != userID.(uint) {
		c.JSON(http.StatusForbidden, gin.H{
			"error": "You can only delete your own posts",
		})
		return
	}

	if err := db.Delete(&post).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": "Failed to delete post",
		})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"message": "Post deleted successfully",
	})
}

func getUserPostsHandler(c *gin.Context) {
	userID, _ := c.Get("user_id")

	var posts []Post
	if err := db.Where("author_id = ?", userID).Order("created_at DESC").Find(&posts).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": "Failed to fetch user posts",
		})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"posts": posts,
		"count": len(posts),
	})
}

// Health and Info Handlers
func healthHandler(c *gin.Context) {
	// Test database connection
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

func statsHandler(c *gin.Context) {
	var userCount, postCount int64

	db.Model(&User{}).Count(&userCount)
	db.Model(&Post{}).Count(&postCount)

	c.JSON(http.StatusOK, gin.H{
		"total_users": userCount,
		"total_posts": postCount,
		"server_time": time.Now(),
		"database":    "SQLite",
	})
}

func main() {
	// Initialize database
	initDatabase()

	r := gin.Default()

	// Configure trusted proxies for nginx
	err := r.SetTrustedProxies([]string{
		"127.0.0.1",
		"172.17.0.0/16",
		"10.0.0.0/8",
		"172.16.0.0/12",
		"192.168.0.0/16",
	})
	if err != nil {
		log.Fatal("Failed to set trusted proxies:", err)
	}

	// Middleware
	r.Use(gin.Logger())
	r.Use(gin.Recovery())

	// Public routes
	r.POST("/register", registerHandler)
	r.POST("/login", loginHandler)
	r.GET("/health", healthHandler)

	// API v1 group
	v1 := r.Group("/api/v1")

	// Public API routes
	v1.GET("/posts", getPostsHandler)
	v1.GET("/posts/:id", getPostHandler)
	v1.GET("/stats", statsHandler)

	// Protected routes
	protected := v1.Group("/")
	protected.Use(jwtAuthMiddleware())
	{
		// Auth routes
		protected.POST("/logout", logoutHandler)

		// Profile routes
		protected.GET("/profile", getProfileHandler)
		protected.PUT("/profile", updateProfileHandler)
		protected.PUT("/profile/password", changePasswordHandler)

		// Post management routes
		protected.POST("/posts", createPostHandler)
		protected.PUT("/posts/:id", updatePostHandler)
		protected.DELETE("/posts/:id", deletePostHandler)
		protected.GET("/my-posts", getUserPostsHandler)
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

	fmt.Printf("Server starting on port %s\n", PORT)
	//fmt.Printf("Database: SQLite (%s)\n", DB_PATH)
	fmt.Println("Default users created:")
	fmt.Println("  Username: admin, Password: admin123")
	fmt.Println("  Username: user, Password: user123")
	fmt.Println("\nAvailable endpoints:")
	fmt.Println("POST   /register")
	fmt.Println("POST   /login")
	fmt.Println("GET    /health")
	fmt.Println("GET    /api/v1/posts")
	fmt.Println("GET    /api/v1/posts/:id")
	fmt.Println("GET    /api/v1/stats")
	fmt.Println("\nProtected endpoints (require JWT token):")
	fmt.Println("POST   /api/v1/logout")
	fmt.Println("GET    /api/v1/profile")
	fmt.Println("PUT    /api/v1/profile")
	fmt.Println("PUT    /api/v1/profile/password")
	fmt.Println("POST   /api/v1/posts")
	fmt.Println("PUT    /api/v1/posts/:id")
	fmt.Println("DELETE /api/v1/posts/:id")
	fmt.Println("GET    /api/v1/my-posts")

	r.Run(PORT)
}

/*
REQUIRED DEPENDENCIES:

go mod init your-app-name
go get github.com/gin-gonic/gin
go get github.com/golang-jwt/jwt/v5
go get golang.org/x/crypto/bcrypt
go get gorm.io/gorm
go get gorm.io/driver/sqlite

EXAMPLE go.mod file:

module your-app-name

go 1.21

require (
    github.com/gin-gonic/gin v1.9.1
    github.com/golang-jwt/jwt/v5 v5.2.0
    golang.org/x/crypto v0.17.0
    gorm.io/driver/sqlite v1.5.4
    gorm.io/gorm v1.25.5
)

DATABASE FEATURES:

1. SQLite database with GORM ORM
2. Auto-migration of schema
3. Password hashing with bcrypt
4. Foreign key relationships (User -> Posts)
5. Database connection health checks
6. Preloading of related data
7. Proper error handling for database operations
8. Default data seeding

TESTING COMMANDS:

# Register new user
curl -X POST http://localhost:8080/register \
  -H "Content-Type: application/json" \
  -d '{"username": "testuser", "email": "test@example.com", "password": "password123"}'

# Login
curl -X POST http://localhost:8080/login \
  -H "Content-Type: application/json" \
  -d '{"username": "admin", "password": "admin123"}'

# Get posts with pagination
curl "http://localhost:8080/api/v1/posts?page=1&limit=5"

# Create post (with JWT token)
curl -X POST http://localhost:8080/api/v1/posts \
  -H "Authorization: Bearer YOUR_JWT_TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"title": "My New Post", "content": "This is the content of my post"}'

# Check database stats
curl http://localhost:8080/api/v1/stats

DATABASE FILE:
The SQLite database file (app.db) will be created automatically in the same directory as your application.

PRODUCTION CONSIDERATIONS:
- Use environment variables for sensitive configuration
- Implement database connection pooling
- Add database backups
- Consider using PostgreSQL or MySQL for production
- Add database indexes for better performance
- Implement soft deletes if needed
*/

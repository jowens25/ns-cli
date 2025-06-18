package jwt

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

type UserLogin struct {
	User struct {
		Username string `json:"username"`
		Password string `json:"password"`
	} `json:"User"`
}

func _RunPrototype() {
	r := gin.Default()

	r.POST("/login", loginHandler)

	r.GET("/login", loginHandler)

	r.GET("/home", CookieAuthMiddleware(), homeHandler)

	r.Run(":8080")
}

func homeHandler(c *gin.Context) {
	c.String(200, "Welcome to the home page!")
}

//func loginHandler(c *gin.Context) {
//	var req UserLogin
//	if err := c.ShouldBindJSON(&req); err != nil {
//		c.JSON(http.StatusBadRequest, gin.H{"error": "Invalid request"})
//		return
//	}
//
//	username := req.User.Username
//	password := req.User.Password
//
//	// Simulate logic based on username/password
//	if username == "spadmin" && password == "default" {
//		// Default password case
//		setLoginCookie(c)
//		c.JSON(http.StatusOK, gin.H{"redirect": "/home?change-admin-password"})
//	} else if username == "admin" && password == "secret" {
//		// Valid login
//		setLoginCookie(c)
//		c.JSON(http.StatusOK, gin.H{"redirect": "/home"})
//	} else {
//		// Login failure
//		c.JSON(http.StatusOK, gin.H{
//			"banner": "<br><br>",
//			"error":  "Invalid username or password",
//		})
//	}
//}

func setLoginCookie(c *gin.Context) {
	c.SetCookie("spectracom", "example-session-token", 3600, "/", "", false, false)
}

func CookieAuthMiddleware() gin.HandlerFunc {
	return func(c *gin.Context) {
		cookie, err := c.Cookie("spectracom")
		if err != nil || cookie != "example-session-token" {
			c.AbortWithStatusJSON(http.StatusForbidden, gin.H{"error": "Forbidden: missing or invalid cookie"})
			return
		}
		c.Next()
	}
}

package lib

import (
	"NovusTimeServer/axi"
	"fmt"
	"log"
	"net/http"
	"os"
	"strconv"
	"sync"
	"syscall"
	"time"

	"github.com/gin-gonic/gin"
	"github.com/golang-jwt/jwt/v5"
)

const (
	JWT_SECRET = "your-secret-key"
)

var SerialMutex sync.Mutex

func StartApp() {

	pid := os.Getpid()
	os.WriteFile("server.pid", []byte(fmt.Sprintf("%d", pid)), 0644)

	//InitNginxConfig()

	createDefaultUser()
	axi.Init()
	//loadSystem()

	log.Println("Database initialized successfully")

	startApiServer()
}

func StopApp() {

	data, err := os.ReadFile("server.pid")
	if err != nil {
		log.Printf("Could not read PID file: %v", err)
	}
	pid, _ := strconv.Atoi(string(data))
	process, err := os.FindProcess(pid)
	if err != nil {
		log.Printf("Could not find process: %v", err)
	}
	process.Signal(syscall.SIGTERM)
}

func healthHandler(c *gin.Context) {

	c.JSON(http.StatusOK, gin.H{
		"status":    "healthy",
		"timestamp": time.Now(),
		"database":  "connected",
	})
}

func timeHandler(c *gin.Context) {
	c.JSON(http.StatusOK, gin.H{
		"time": GetTime(),
	})
}

func logoutHandler(c *gin.Context) {
	c.JSON(http.StatusOK, gin.H{"message": "Logged out successfully"})
}

func generateJWT(user *User) (string, error) {
	claims := &Claims{
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

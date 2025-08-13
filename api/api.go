package api

import (
	"fmt"
	"log"
	"net/http"
	"os"
	"strings"
	"sync"
	"time"

	"github.com/gin-gonic/gin"
	"github.com/golang-jwt/jwt/v5"
	"golang.org/x/crypto/bcrypt"
	"gorm.io/driver/sqlite"
	"gorm.io/gorm"
	"gorm.io/gorm/logger"
)

const (
	JWT_SECRET = "your-secret-key"
	API_HOST   = "API_HOST"
	API_PORT   = ":5000"
	WEB_HOST   = "WEB_HOST"
	WEB_PORT   = ":80"
	DB_PATH    = "./app.db"
)

var serialMutex sync.Mutex

func init() {
	os.Setenv(SNMP_CONFIG_PATH, "/etc/snmp/snmpd.conf")

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

func SetIp(ip string, gw string) {

	file, err := os.Open("/etc/network/interfaces")

	if err != nil {
		log.Println(err)
	}
	defer file.Close()

	var lines []string

	newAddressLine := fmt.Sprintf("address %s", ip)
	newGatewayLine := fmt.Sprintf("gateway %s", gw)

	lines = append(lines, []string{"allow-hotplug eth0"}...)
	lines = append(lines, []string{"no-auto-down eth0"}...)
	lines = append(lines, []string{"iface eth0 inet static"}...)
	lines = append(lines, []string{newAddressLine}...)
	lines = append(lines, []string{"netmask 255.255.255.0"}...)
	lines = append(lines, []string{newGatewayLine}...)
	lines = append(lines, []string{"dns-nameservers 8.8.8.8 8.8.4.4"}...)
	lines = append(lines, []string{"# Local loopback"}...)
	lines = append(lines, []string{"auto lo"}...)
	lines = append(lines, []string{"iface lo inet loopback"}...)

	err = os.WriteFile("/etc/network/interfaces", []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Fatal("failed to write file:", err)
	}

}

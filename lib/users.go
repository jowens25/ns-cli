package lib

import (
	"bufio"
	"errors"
	"fmt"
	"log"
	"net/http"
	"os"
	"os/exec"
	"os/user"
	"strings"
	"time"

	"github.com/gin-gonic/gin"
	"golang.org/x/crypto/bcrypt"
	"gorm.io/gorm"
)

const (
	DefaultUserHome = "/home/novus"
	AdminGroup      = "admin"
	UserGroup       = "user"
)

func readUsers(c *gin.Context) {

	requestID := c.GetHeader("X-Request-ID")
	//requestID := c.GetString("request_id")
	log.Printf("Request ID: %s\n", requestID)

	var users []User

	getAdminUsers()

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

func getAdminUsers() {
	///var users []User
	cmd := exec.Command("getent", "group")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal(err.Error())
	}

	lines := strings.SplitSeq(string(out), "\n")
	var newUsernames []string

	for line := range lines {
		if strings.HasPrefix(line, "admin:") {
			parts := strings.Split(line, ":")
			if len(parts) >= 4 && parts[3] != "" {
				usernames := strings.SplitSeq(parts[3], ",")
				for username := range usernames {
					var user User
					user.Role = "admin"
					user.Username = username

					// look up the user by user name
					result := db.Where("username = ?", user.Username).First(&User{})
					if result.Error == gorm.ErrRecordNotFound {
						// Create new user
						db.Create(&user)

					} else {
						// Update existing user
						db.Where("username = ?", user.Username).Updates(&user)
					}

					newUsernames = append(newUsernames, user.Username)

				}
				db.Where("username NOT IN ?", newUsernames).Delete(&User{})

			}
		}
	}

}

func writeUser(c *gin.Context) {

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

	//if !(newUser.Role == "admin" || newUser.Role == "viewer") {
	//	c.JSON(http.StatusBadRequest, gin.H{"error": "role must be 'admin' or 'viewer'"})
	//	return
	//}

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

	switch newUser.Role {
	case "admin":
		AddAdmin(newUser.Username, newUser.Password)
	case "viewer":
		AddUser(newUser.Username, newUser.Password)
	default:
		c.JSON(http.StatusBadRequest, gin.H{"error": "failed to add user"})
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

func deleteUser(c *gin.Context) {

	userID := c.Param("id")

	var userToDelete User

	if err := db.First(&userToDelete, userID).Error; err != nil {
		c.JSON(http.StatusNotFound, gin.H{"error": "User not found"})
		return
	}

	if userToDelete.Role == "viewer" {
		if err := db.Delete(&userToDelete).Error; err != nil {
			c.JSON(http.StatusConflict, gin.H{"error": err.Error()})
			return
		}
	}

	if userToDelete.Role == "admin" {

		var count int64

		db.Model(&User{}).Where("role = ?", "admin").Count(&count)

		if count > 1 {

			err := DeleteUser(userToDelete.Username)

			fmt.Println(err)

			if err != nil {
				c.JSON(http.StatusFailedDependency, gin.H{"error": err.Error()})
				return
			}

			if err := db.Delete(&userToDelete).Error; err != nil {
				c.JSON(http.StatusBadGateway, gin.H{"error": err.Error()})
				return
			}

			c.JSON(http.StatusCreated, gin.H{
				"message": "User deleted successfully",
			})

		} else {
			c.JSON(http.StatusOK, gin.H{
				"error": "Cannot delete the last admin account",
			})
			return
		}

	}

}

func editUser(c *gin.Context) {
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

func SetUserPermissions(username string) error {
	cmd := exec.Command("usermod", "-g", UserGroup, "-G", UserGroup, username)
	return cmd.Run()
}

func SetAdministratorPermissions(username string) error {
	cmd := exec.Command("usermod", "-g", AdminGroup, "-G", UserGroup+","+AdminGroup, username)
	return cmd.Run()
}

// IsUserAdmin returns true if user is admin-only (not factory)
func IsUserAdmin(username string) (bool, error) {
	usr, err := user.Lookup(username)
	if err != nil {
		return false, err
	}

	gids, err := usr.GroupIds()
	if err != nil {
		return false, err
	}

	var groups []string
	for _, gid := range gids {
		grp, err := user.LookupGroupId(gid)
		if err != nil {
			continue
		}
		groups = append(groups, grp.Name)
	}

	groupStr := " " + strings.Join(groups, " ") + " "

	isAdmin := !strings.Contains(groupStr, " "+AdminGroup+" ")
	isFactory := !strings.Contains(groupStr, " factory ")

	return !isAdmin && isFactory, nil
}

func AdminNumber() (int, error) {
	file, err := os.Open("/etc/group")
	if err != nil {
		return 0, err
	}
	defer file.Close()

	var adminUsers, factoryUsers []string
	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := scanner.Text()
		parts := strings.Split(line, ":")
		if len(parts) < 4 {
			continue
		}

		groupName := parts[0]
		users := parts[3]

		switch groupName {
		case AdminGroup:
			if users != "" {
				adminUsers = strings.Split(users, ",")
			}
		case "factory":
			if users != "" {
				factoryUsers = strings.Split(users, ",")
			}
		}
	}

	if err := scanner.Err(); err != nil {
		return 0, err
	}

	factoryMap := make(map[string]bool)
	for _, user := range factoryUsers {
		factoryMap[user] = true
	}

	adminOnlyCount := 0
	for _, user := range adminUsers {
		if !factoryMap[user] {
			adminOnlyCount++
		}
	}

	return adminOnlyCount, nil
}

func AddUser(username string, password string) error {
	cmd := exec.Command("useradd", "-M", "-N", "-p", password, "-g", UserGroup, "-d", DefaultUserHome, username)
	output, err := cmd.CombinedOutput()
	fmt.Println(string(output), err)
	return cmd.Run()
}

func AddAdmin(username string, password string) error {
	cmd := exec.Command("useradd", "-M", "-N", "-p", password, "-g", AdminGroup, "-G", UserGroup+","+AdminGroup, "-d", DefaultUserHome, username)
	output, err := cmd.CombinedOutput()
	fmt.Println(string(output), err)
	return cmd.Run()
}

func SetUsername(oldUsername, newUsername string) error {
	cmd := exec.Command("usermod", "-l", newUsername, oldUsername)
	return cmd.Run()
}

func SetGroupUser(username string) error {
	return SetUserPermissions(username)
}

func SetGroupAdmin(username string) error {
	return SetAdministratorPermissions(username)
}

func DeleteUser(username string) error {
	isAdmin, err := IsUserAdmin(username)
	if err != nil {
		return fmt.Errorf("failed to check if user is admin: %w", err)
	}

	if isAdmin {
		adminCount, err := AdminNumber()
		if err != nil {
			return fmt.Errorf("failed to get admin count: %w", err)
		}

		if adminCount <= 1 {
			return fmt.Errorf("ERROR: deleting last admin account")
		}
	}

	killCmd := exec.Command("pkill", "-u", username)
	_ = killCmd.Run() // Ignore error as user might not have running processes

	// Delete the user
	delCmd := exec.Command("userdel", username)
	err = delCmd.Run()
	return err
}

func CreatePassHistTemp() error {
	file, err := os.Create("/etc/security/nopasswd")
	if err != nil {
		return err
	}
	file.Close()

	// Set ownership to root:root
	chownCmd := exec.Command("chown", "root:root", "/etc/security/nopasswd")
	if err := chownCmd.Run(); err != nil {
		return err
	}

	// Set permissions to 660
	chmodCmd := exec.Command("chmod", "660", "/etc/security/nopasswd")
	return chmodCmd.Run()
}

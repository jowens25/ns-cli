package lib

import (
	"bufio"
	"fmt"
	"log"
	"net/http"
	"os"
	"os/exec"
	"os/user"
	"strings"

	"github.com/gin-gonic/gin"
	"gorm.io/gorm"
)

const (
	DefaultUserHome = "/home/novus"
	AdminGroup      = "novusadmin"
	UserGroup       = "novususer"
)

func readSystemUsers(c *gin.Context) {

	readUsersFromSystem()

	var users []User
	result := db.Find(&users)

	if result.Error != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"system_users": users,
	})

}

func writeSystemUser(c *gin.Context) {

	var user User

	if err := c.ShouldBindJSON(&user); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	result := db.Where("username = ?", user.Username).FirstOrCreate(&user)

	if result.Error != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
		return
	}

	passwordhint, err := addUserToSystem(user)
	fmt.Println(passwordhint)
	if err != nil {
		c.JSON(http.StatusOK, gin.H{
			"error":    err.Error(),
			"password": passwordhint,
		})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"message": "System user created",
		"user":    user,
	})

}

func removeUserFromSystem(user User) error {
	isAdmin, err := IsUserAdmin(user.Username)
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

	cmd := exec.Command("pkill", "-u", user.Username)
	out, err := cmd.CombinedOutput() // Ignore error as user might not have running processes
	log.Println(out)
	// Delete the user
	cmd = exec.Command("userdel", user.Username)
	out, err = cmd.CombinedOutput() // Ignore error as user might not have running processes
	log.Println(out)

	return nil

}

func deleteSystemUser(c *gin.Context) {
	userID := c.Param("id")

	var userToDelete User

	if err := db.First(&userToDelete, userID).Error; err != nil {
		c.JSON(http.StatusNotFound, gin.H{"error": "User not found"})
		return
	}

	// Remove from system first
	if err := removeUserFromSystem(userToDelete); err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": fmt.Sprintf("Failed to remove user from system: %v", err)})
		return
	}

	// Then remove from database
	if err := db.Delete(&userToDelete).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"message": "User removed successfully",
	})
}

//func editSystemUser(c *gin.Context) {//
//
//	id := c.Param("id")
//
//	var existingUser User
//
//	if err := db.First(&existingUser, id).Error; err != nil {
//		if errors.Is(err, gorm.ErrRecordNotFound) {
//			c.JSON(http.StatusNotFound, gin.H{"error": "User not found"})
//		} else {
//			c.JSON(http.StatusInternalServerError, gin.H{"error": "Database error"})
//		}
//		return
//	}
//
//	var updateData User
//	if err := c.ShouldBindJSON(&updateData); err != nil {
//		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
//		return
//	}
//
//	updates := make(map[string]any)
//
//	if updateData.Username != "" {
//		updates["username"] = updateData.Username
//	}
//
//	if updateData.Email != "" {
//		updates["email"] = updateData.Email
//	}
//
//	if updateData.Role != "" {
//		if !(updateData.Role == "admin" || updateData.Role == "viewer") {
//			c.JSON(http.StatusBadRequest, gin.H{"error": "role must be 'admin' or 'viewer'"})
//			return
//		}
//		updates["role"] = updateData.Role
//	}
//
//	if updateData.Password != "" {
//
//		changePassword()
//
//		passwordhint, err := changePassword(user)
//		fmt.Println(passwordhint)
//		if err != nil {
//			c.JSON(http.StatusOK, gin.H{
//				"error":    err.Error(),
//				"password": passwordhint,
//			})
//			return
//		}
//
//	}
//
//	// Check if there's anything to update
//	if len(updates) == 0 {
//		c.JSON(http.StatusOK, gin.H{"message": "No update"})
//		return
//	}
//
//	// Perform the update using the user ID
//	result := db.Model(&existingUser).Updates(updates)
//	if result.Error != nil {
//		c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
//		return
//	}
//
//	var updatedUser User
//	if err := db.First(&updatedUser, id).Error; err != nil {
//		c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to fetch updated user"})
//		return
//	}
//
//	c.JSON(http.StatusOK, gin.H{
//		"message": "User updated successfully",
//		"user": gin.H{
//			"id":       updatedUser.ID,
//			"role":     updatedUser.Role,
//			"username": updatedUser.Username,
//			"email":    updatedUser.Email,
//		},
//	})
//}

func readUsersFromSystem() {
	cmd := exec.Command("getent", "group")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal(err.Error())
	}

	lines := strings.Split(string(out), "\n")
	var newUsernames []string
	userRoleMap := make(map[string]string) // Track users and their roles

	for _, line := range lines {
		if strings.HasPrefix(line, "novusadmin:") || strings.HasPrefix(line, "novususer:") {
			parts := strings.Split(line, ":")
			role := strings.TrimPrefix(parts[0], "novus")

			if len(parts) >= 4 && parts[3] != "" {
				usernames := strings.Split(parts[3], ",")
				for _, username := range usernames {
					username = strings.TrimSpace(username)
					if username == "" {
						continue
					}

					// If user is in novusadmin, they are admin regardless of novususer membership
					if role == "admin" || userRoleMap[username] != "admin" {
						userRoleMap[username] = role
					}
				}
			}
		}
	}

	// Process all unique users
	for username, role := range userRoleMap {
		var user User
		user.Role = role
		user.Username = username

		// Look up the user by username
		result := db.Where("username = ?", user.Username).First(&User{})
		if result.Error == gorm.ErrRecordNotFound {
			// Create new user
			db.Create(&user)
		} else {
			// Update existing user's role
			db.Where("username = ?", user.Username).Updates(&user)
		}

		newUsernames = append(newUsernames, user.Username)
	}

	// Clean up users that no longer exist in the system (only execute once)
	if len(newUsernames) > 0 {
		db.Where("username NOT IN ?", newUsernames).Delete(&User{})
	} else {
		// If no users found, delete all
		db.Delete(&User{}, "1=1")
	}
}

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

	isInAdminGroup := strings.Contains(groupStr, " "+AdminGroup+" ")
	isInFactoryGroup := strings.Contains(groupStr, " factory ")

	// User is admin if they are in admin group but NOT in factory group
	return isInAdminGroup && !isInFactoryGroup, nil
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

func changePassword(user User) (string, error) {
	thiscmd := exec.Command("chpasswd")
	thiscmd.Stdin = strings.NewReader(fmt.Sprintf("%s:%s", user.Username, user.Password))
	output, err := thiscmd.CombinedOutput()
	out := string(output)
	if after, ok := strings.CutPrefix(out, "BAD PASSWORD:"); ok {
		//removeUserFromSystem(user)
		return after, fmt.Errorf("BAD PASSWORD")
	}

	if err != nil {
		fmt.Println(err.Error())
	}

	return out, nil
}

func addUserToSystem(user User) (string, error) {
	switch user.Role {
	case "admin":
		AddAdmin(user.Username, user.Password)

	case "viewer":
		AddUser(user.Username, user.Password)

	default:
		log.Println("not viewer or admin user")
	}

	thiscmd := exec.Command("chpasswd")
	thiscmd.Stdin = strings.NewReader(fmt.Sprintf("%s:%s", user.Username, user.Password))
	output, err := thiscmd.CombinedOutput()

	out := string(output)
	if after, ok := strings.CutPrefix(out, "BAD PASSWORD:"); ok {
		removeUserFromSystem(user)
		return after, fmt.Errorf("BAD PASSWORD")
	}

	if err != nil {
		fmt.Println(err.Error())
	}

	return out, nil
}

func AddUser(username string, password string) error {
	// Create user account
	cmd := exec.Command("useradd",
		"-M",            // Don't create home directory
		"-N",            // Don't create a group with the same name as the user
		"-g", UserGroup, // Primary group
		"-G", UserGroup, // Secondary groups
		"-d", DefaultUserHome, // Home directory
		"-s", "/bin/bash", // Shell
		username)

	output, err := cmd.CombinedOutput()
	if err != nil {
		return fmt.Errorf("failed to create user %s: %v\nOutput: %s", username, err, string(output))
	}

	fmt.Printf("Successfully created user: %s\n", username)
	return nil
}

// AddAdmin creates an admin account with PAM password policy enforcement
func AddAdmin(username string, password string) error {
	// Create admin account
	cmd := exec.Command("useradd",
		"-M",             // Don't create home directory
		"-N",             // Don't create a group with the same name
		"-g", AdminGroup, // Primary group
		"-G", UserGroup+","+AdminGroup, // Secondary groups
		"-d", DefaultUserHome, // Home directory
		"-s", "/bin/bash", // Shell
		username)

	output, err := cmd.CombinedOutput()
	if err != nil {
		return fmt.Errorf("failed to create admin %s: %v\nOutput: %s", username, err, string(output))
	}

	fmt.Printf("Successfully created admin: %s\n", username)
	return nil
}

func SetUsername(oldUsername, newUsername string) error {
	cmd := exec.Command("usermod", "-l", newUsername, oldUsername)
	output, err := cmd.CombinedOutput()
	fmt.Println(string(output), err)

	return err
}

func SetUserPermissions(username string) error {
	cmd := exec.Command("usermod", "-g", UserGroup, "-G", UserGroup, username)
	return cmd.Run()
}

func SetAdministratorPermissions(username string) error {
	cmd := exec.Command("usermod", "-g", AdminGroup, "-G", UserGroup+","+AdminGroup, username)
	return cmd.Run()
}

func addAdminGroup() {

	thiscmd := exec.Command("groupadd", "novusadmin")
	output, err := thiscmd.CombinedOutput()

	if err != nil {
		fmt.Println(err.Error())
	}
	fmt.Println(string(output))
}

func addUserGroup() {

	thiscmd := exec.Command("groupadd", "novususer")
	output, err := thiscmd.CombinedOutput()

	if err != nil {
		fmt.Println(err.Error())
	}
	fmt.Println(string(output))
}

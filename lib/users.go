package lib

import (
	"fmt"
	"log"
	"net/http"
	"os"
	"os/exec"
	"os/user"
	"strings"

	"github.com/gin-gonic/gin"
)

func readSystemUsers(c *gin.Context) {

	allUsers := readCombinedSystemUsers()

	fmt.Println(len(allUsers))

	if len(allUsers) < 1 {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "no users defined"})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"system_users": allUsers,
	})

}

func readCombinedSystemUsers() []User {

	admins := readSystemAdmins()

	users := readSystemViewers()

	var collatedUsers []User

	for _, user := range users {

		for _, admin := range admins {

			if user.Username == admin.Username {

				user.Role = "admin"
			}
		}

		collatedUsers = append(collatedUsers, user)
	}

	return collatedUsers
}

func lookUpSystemUser(user User) User {

	allUsers := readCombinedSystemUsers()

	for _, u := range allUsers {

		if u.Username == user.Username {
			return u
		}
	}

	return User{}
}

func writeSystemUser(c *gin.Context) {

	var user User

	if err := c.ShouldBindJSON(&user); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	addUserToSystem(user)

	c.JSON(http.StatusOK, gin.H{
		"message": "System user created",
		"user":    user,
	})

}

func editSystemUser(c *gin.Context) {

	var user User

	if err := c.ShouldBindJSON(&user); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	fmt.Println(user.Username)
	fmt.Println(user.Password)
	fmt.Println(user.Role)

	if user.Role == "admin" {
		MakeUserAdmin(user.Username)
	}

	if user.Role == "viewer" {
		MakeUserViewer(user.Username)
	}

	if user.Password != "" {
		ChangePassword(user)
		fmt.Println("password updated")
	}

	c.JSON(http.StatusOK, gin.H{
		"message": "System user modified",
		"user":    user,
	})
}

func removeUserFromSystem(user User) error {

	isAdmin := false

	if user.Role == "admin" {
		isAdmin = true
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
	if err != nil {
		fmt.Println(err.Error())
	}
	log.Println(string(out))
	// Delete the user
	cmd = exec.Command("userdel", "-r", user.Username)
	out, err = cmd.CombinedOutput() // Ignore error as user might not have running processes
	if err != nil {
		fmt.Println(err.Error())
	}
	log.Println(string(out))

	cmd = exec.Command("rm", "-r", "/home/"+user.Username)
	out, err = cmd.CombinedOutput() // Ignore error as user might not have running processes
	if err != nil {
		fmt.Println(err.Error())
	}
	log.Println(string(out))

	return nil

}

func IsAdminRoot() bool {

	currentUser, err := user.Current()
	if err != nil {
		log.Fatalf("Failed to get current user: %v", err)
	}

	if currentUser.Username == "root" {
		return true
	}

	admins := readSystemAdmins()

	for _, cu := range admins {
		if cu.Username == currentUser.Username {
			return true
		}
	}
	return false
}

func deleteSystemUser(c *gin.Context) {
	//userID := c.Param("id")

	var userToDelete User

	if err := c.ShouldBindJSON(&userToDelete); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	// Remove from system first
	if err := removeUserFromSystem(userToDelete); err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": fmt.Sprintf("Failed to remove user from system: %v", err)})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"message": "User removed successfully",
	})
}

func readSystemAdmins() []User {

	var currentUsers []User
	cmd := exec.Command("getent", "group", AppConfig.User.AdminGroup)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(err.Error())
	}

	lines := strings.Split(string(out), "\n")

	if len(lines) < 1 {
		return nil
	}

	for _, line := range lines {

		parts := strings.Split(line, ":")

		if len(parts) >= 4 && parts[3] != "" {

			usernames := strings.Split(parts[3], ",")

			for _, username := range usernames {
				username = strings.TrimSpace(username)

				var user User
				user.Username = username
				user.Role = "admin"

				currentUsers = append(currentUsers, user)
			}

		}
	}

	return currentUsers
}

func readSystemViewers() []User {

	var currentUsers []User
	cmd := exec.Command("getent", "group", AppConfig.User.UserGroup)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(err.Error())
	}

	lines := strings.Split(string(out), "\n")

	if len(lines) < 1 {
		return nil
	}

	for _, line := range lines {

		parts := strings.Split(line, ":")

		if len(parts) >= 4 && parts[3] != "" {

			usernames := strings.Split(parts[3], ",")

			for _, username := range usernames {
				username = strings.TrimSpace(username)

				var user User
				user.Username = username
				user.Role = "viewer"

				currentUsers = append(currentUsers, user)
			}

		}
	}

	return currentUsers
}

func AdminNumber() (int, error) {
	content, err := os.ReadFile(AppConfig.User.GroupPath)
	if err != nil {
		return 0, err
	}
	lines := strings.SplitSeq(string(content), "\n")

	for line := range lines {

		if strings.HasPrefix(line, AppConfig.User.AdminGroup) {

			parts := strings.Split(line, ":")

			if parts[3] != "" {
				return len(strings.Split(parts[3], ",")), nil

			} else {
				return 0, fmt.Errorf("no admins defined")
			}
		}
	}

	return 0, fmt.Errorf("no admins found")
}

func ChangePassword(user User) (string, error) {
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
		err := AddAdmin(user.Username, user.Password)
		if err != nil {
			fmt.Println(err.Error())
		}

	case "viewer":
		AddUser(user.Username, user.Password)

	default:
		log.Println("not viewer or admin user")
	}

	thiscmd := exec.Command("chpasswd")
	thiscmd.Stdin = strings.NewReader(fmt.Sprintf("%s:%s", user.Username, user.Password))
	output, err := thiscmd.CombinedOutput()

	return string(output), err
}

func AddUser(username string, password string) error {
	// Create user account
	cmd := exec.Command("useradd",
		"-m",                           // Create home directory
		"-N",                           // Don't create a group with the same name as the user
		"-g", AppConfig.User.UserGroup, // Primary group
		"-G", AppConfig.User.UserGroup, // Secondary groups
		"-s", "/bin/bash",

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
		"-m",                            // Create home directory
		"-N",                            // Don't create a group with the same name
		"-g", AppConfig.User.AdminGroup, // Primary group
		"-G", AppConfig.User.UserGroup+","+AppConfig.User.AdminGroup, // Secondary groups
		"-s", "/bin/bash",

		username)

	output, err := cmd.CombinedOutput()
	if err != nil {
		return fmt.Errorf("failed to create admin %s: %v\nOutput: %s", username, err, string(output))
	}

	fmt.Printf("Successfully created admin: %s\n", username)
	return nil
}

func MakeUserAdmin(username string) error {
	// Create admin account
	cmd := exec.Command("usermod", username,
		"-g", AppConfig.User.AdminGroup,
		"-G", AppConfig.User.UserGroup+","+AppConfig.User.AdminGroup)

	output, err := cmd.CombinedOutput()
	if err != nil {
		return fmt.Errorf("failed to mod user %s: %v\nOutput: %s", username, err, string(output))
	}

	fmt.Printf("Successfully made user admin user: %s\n", username)
	return nil
}
func MakeUserViewer(username string) error {
	// Create admin account
	cmd := exec.Command("usermod", username,
		"-g", AppConfig.User.UserGroup,
		"-G", AppConfig.User.UserGroup)

	output, err := cmd.CombinedOutput()
	if err != nil {
		return fmt.Errorf("failed to mod user %s: %v\nOutput: %s", username, err, string(output))
	}

	fmt.Printf("Successfully mod user: %s\n", username)
	return nil
}

func SetUsername(oldUsername, newUsername string) error {
	cmd := exec.Command("usermod", "-l", newUsername, oldUsername)
	output, err := cmd.CombinedOutput()
	fmt.Println(string(output), err)

	return err
}

func SetUserPermissions(username string) error {
	cmd := exec.Command("usermod", "-g", AppConfig.User.UserGroup, "-G", AppConfig.User.UserGroup, username)
	return cmd.Run()
}

func SetAdministratorPermissions(username string) error {
	cmd := exec.Command("usermod", "-g", AppConfig.User.AdminGroup, "-G", AppConfig.User.UserGroup+","+AppConfig.User.AdminGroup, username)
	return cmd.Run()
}

func addAdminGroup() {

	thiscmd := exec.Command("groupadd", "novusadmin")
	output, err := thiscmd.CombinedOutput()

	if err != nil {
		fmt.Println(err.Error())
		fmt.Println(string(output))

	}
}

func addUserGroup() {

	thiscmd := exec.Command("groupadd", "novususer")
	output, err := thiscmd.CombinedOutput()

	if err != nil {
		fmt.Println(err.Error())
		fmt.Println(string(output))

	}
}

func ResetUsers() {

	users := readSystemViewers()

	for _, u := range users {
		removeUserFromSystem(u)
	}

	createDefaultUser()

}

func createDefaultUser() {

	var user User
	user.Username = AppConfig.User.DefaultUsername
	user.Password = AppConfig.User.DefaultPassword
	user.Role = "admin"

	addAdminGroup()
	addUserGroup()

	SetPasswordEnforcement(false)

	warning, err := addUserToSystem(user)
	if err != nil {
		fmt.Println(warning)
		fmt.Println(err.Error())
	}

}

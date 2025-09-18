package lib

import (
	"bufio"
	"errors"
	"fmt"
	"log"
	"net/http"
	"os"
	"strings"

	"github.com/gin-gonic/gin"
	"gorm.io/gorm"
)

var v3_users []SnmpV3User

func readSnmpV3Users(c *gin.Context) {
	//StopSnmpd()
	readSnmpUsersFromFile()
	//StartSnmpd()
	var users []SnmpV3User
	result := db.Find(&users)

	if result.Error != nil {
		log.Println(result.Error)
	}

	c.JSON(http.StatusOK, gin.H{

		"v3_users": users,
	})

}

// writeSnmpV3User writes lines in the snmpd.conf for a v3 user
// Returns "messge" and "v3_user"
func writeSnmpV3User(c *gin.Context) {
	var user SnmpV3User

	if err := c.ShouldBindJSON(&user); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	result := db.Where("user_name = ?", user.UserName).FirstOrCreate(&user)

	if result.Error != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
		return
	}

	StopSnmpd()
	addSnmpV3UserToFile(user)
	StartSnmpd()

	c.JSON(http.StatusOK, gin.H{
		"message": "SNMP V3 User Created",
		"v3_user": user,
	})
}

func addSnmpV3UserToFile(user SnmpV3User) {
	file, err := os.Open(AppConfig.Snmp.Path)

	if err != nil {
		log.Println(err)
	}
	defer file.Close()

	lineCount := 0
	createUserIndex := -1
	groupIndex := -1

	var lines []string
	scanner := bufio.NewScanner(file)
	// read all the lines, find placements
	for scanner.Scan() {
		line := scanner.Text()

		if strings.Contains(line, "#createUser") {
			createUserIndex = lineCount + 2 // skip the header and blank line
		}
		if strings.Contains(line, "#group") {
			groupIndex = lineCount + 2
		}

		lines = append(lines, line)
		lineCount++
	}

	newUserLine := fmt.Sprintf("createUser %s %s %s %s %s", user.UserName, user.AuthType, user.AuthPassphrase, user.PrivType, user.PrivPassphrase)
	newGroupLine := fmt.Sprintf("group %s %s %s", user.GroupName, user.Version, user.UserName)

	if createUserIndex < 0 {
		lines = append(lines, []string{"#-------------------------------------------------------------------------------"}...)
		lines = append(lines, []string{"#createUser username [MD5|SHA] [passphrase] [DES] [passphrase]"}...)
		lines = append(lines, []string{"#-------------------------------------------------------------------------------"}...)
		lines = append(lines, []string{newUserLine}...)
	} else {
		lines = append(lines[:createUserIndex], append([]string{newUserLine}, lines[createUserIndex:]...)...)
	}

	if groupIndex < 0 {
		lines = append(lines, []string{"#-------------------------------------------------------------------------------"}...)
		lines = append(lines, []string{"#group  group name      sec.model  sec.name"}...)
		lines = append(lines, []string{"#-------------------------------------------------------------------------------"}...)
		lines = append(lines, []string{newGroupLine}...)

	} else {
		lines = append(lines[:groupIndex], append([]string{newGroupLine}, lines[groupIndex:]...)...)
	}

	err = os.WriteFile(AppConfig.Snmp.Path, []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Println("failed to write file:", err)
	}
}

func removeSnmpV3UserFromFile(user SnmpV3User) {

	UserLineToDelete := fmt.Sprintf("createUser %s %s %s %s %s", user.UserName, user.AuthType, user.AuthPassphrase, user.PrivType, user.PrivPassphrase)
	GroupLineToDelete := fmt.Sprintf("group %s %s %s", user.GroupName, user.Version, user.UserName)
	log.Println(UserLineToDelete)
	log.Println(GroupLineToDelete)
	file, err := os.Open(AppConfig.Snmp.Path)

	if err != nil {
		log.Println(err)
	}
	defer file.Close()

	var lines []string
	scanner := bufio.NewScanner(file)
	// read all the lines, find placements
	for scanner.Scan() {
		line := scanner.Text()

		if strings.Contains(line, UserLineToDelete) {
			continue
		}
		if strings.Contains(line, GroupLineToDelete) {
			continue
		}

		lines = append(lines, line)

	}

	err = os.WriteFile(AppConfig.Snmp.Path, []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Println("failed to write file:", err)
	}
}

func deleteSnmpV3User(c *gin.Context) {

	id := c.Param("id")

	var userToDelete SnmpV3User

	if err := db.First(&userToDelete, id).Error; err != nil {
		c.JSON(http.StatusNotFound, gin.H{"error": "User not found"})
		return
	}
	StopSnmpd()
	removeSnmpV3UserFromFile(userToDelete)
	StartSnmpd()
	if err := db.Delete(&userToDelete).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"message": "User removed successfully",
	})
}

func editSnmpV3User(c *gin.Context) {
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
	StopSnmpd()
	removeSnmpV3UserFromFile(existingUser)
	StartSnmpd()
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
		if !(updateData.GroupName == "roprivgroup" || updateData.GroupName == "rwprivgroup") {
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

	addSnmpV3UserToFile(updatedUser)

	c.JSON(http.StatusOK, gin.H{
		"message": "User updated successfully",
		"v3_user": updatedUser,
	})

}

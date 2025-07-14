package api

import (
	"bufio"
	"errors"
	"fmt"
	"log"
	"net/http"
	"os"
	"strconv"
	"strings"

	"github.com/gin-gonic/gin"
	"gorm.io/gorm"
)

// writeSnmpV1V2cUser creates entries in the snmpd config file for v1 and v2c users using a templated config file
// Returns message v1v2c_user json

var v1v2c_users []SnmpV1V2cUser

func readSnmpV1V2cUsers(c *gin.Context) {

	readSnmpUsersFromFile()

	var users []SnmpV1V2cUser
	result := db.Find(&users)

	if result.Error != nil {
		log.Println(result.Error)
	}

	c.JSON(http.StatusOK, gin.H{

		"v1v2c_users": users,
	})

}

func writeSnmpV1V2cUser(c *gin.Context) {
	var user SnmpV1V2cUser

	if err := c.ShouldBindJSON(&user); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	user.ComNumber = strconv.FormatInt(int64(len(v1v2c_users)), 10)

	result := db.Where("community = ?", user.Community).FirstOrCreate(&user)

	if result.Error != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
		return
	}

	StopSnmpd()
	addSnmpV1V2cUserToFile(user)
	StartSnmpd()

	c.JSON(http.StatusOK, gin.H{
		"message":    "SNMP V1/V2c User Created",
		"v1v2c_user": user,
	})
}

func addSnmpV1V2cUserToFile(user SnmpV1V2cUser) {
	file, err := os.Open(os.Getenv(SNMP_CONFIG_PATH))

	if err != nil {
		log.Println(err)
	}
	defer file.Close()

	lineCount := 0
	userIndex := -1
	groupIndex := -1

	var lines []string
	scanner := bufio.NewScanner(file)
	// read all the lines, find placements
	for scanner.Scan() {
		line := scanner.Text()

		if strings.Contains(line, "#com2sec") {
			userIndex = lineCount + 2 // skip the header and blank line
		}

		if strings.Contains(line, "#group") {
			groupIndex = lineCount + 3
		}

		lines = append(lines, line)
		lineCount++
	}

	// seemingly no header found...

	newUserLine := fmt.Sprintf("com2sec comuser_%s %s %s", user.ComNumber, user.Source, user.Community)
	newGroupLine := fmt.Sprintf("group %s %s comuser_%s", user.GroupName, user.Version, user.ComNumber)

	if userIndex < 0 {
		lines = append(lines, []string{"#-------------------------------------------------------------------------------"}...)
		lines = append(lines, []string{"#com2sec sec.name source community"}...)
		lines = append(lines, []string{"#-------------------------------------------------------------------------------"}...)
		lines = append(lines, []string{newUserLine}...)
	} else {
		lines = append(lines[:userIndex], append([]string{newUserLine}, lines[userIndex:]...)...)
	}

	if groupIndex < 0 {
		lines = append(lines, []string{"#-------------------------------------------------------------------------------"}...)
		lines = append(lines, []string{"#group  group name      sec.model  sec.name"}...)
		lines = append(lines, []string{"#-------------------------------------------------------------------------------"}...)
		lines = append(lines, []string{newGroupLine}...)

	} else {
		lines = append(lines[:groupIndex], append([]string{newGroupLine}, lines[groupIndex:]...)...)
	}

	err = os.WriteFile(os.Getenv(SNMP_CONFIG_PATH), []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Fatal("failed to write file:", err)
	}
}

func removeSnmpV1V2cUserFromFile(user SnmpV1V2cUser) {

	UserLineToDelete := fmt.Sprintf("com2sec comuser_%s %s %s", user.ComNumber, user.Source, user.Community)
	GroupLineToDelete := fmt.Sprintf("group %s %s comuser_%s", user.GroupName, user.Version, user.ComNumber)
	log.Println(UserLineToDelete)
	log.Println(GroupLineToDelete)
	file, err := os.Open(os.Getenv(SNMP_CONFIG_PATH))

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

	err = os.WriteFile(os.Getenv(SNMP_CONFIG_PATH), []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Fatal("failed to write file:", err)
	}
}

func deleteSnmpV1V2cUser(c *gin.Context) {

	id := c.Param("id")

	var userToDelete SnmpV1V2cUser

	if err := db.First(&userToDelete, id).Error; err != nil {
		c.JSON(http.StatusNotFound, gin.H{"error": "User not found"})
		return
	}

	removeSnmpV1V2cUserFromFile(userToDelete)

	if err := db.Delete(&userToDelete).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"message": "User removed successfully",
	})
}

func editSnmpV1V2cUser(c *gin.Context) {

	id := c.Param("id")

	var existingUser SnmpV1V2cUser
	if err := db.First(&existingUser, id).Error; err != nil {
		if errors.Is(err, gorm.ErrRecordNotFound) {
			c.JSON(http.StatusNotFound, gin.H{"error": "User not found"})
		} else {
			c.JSON(http.StatusInternalServerError, gin.H{"error": "Database error"})
		}
		return
	}

	removeSnmpV1V2cUserFromFile(existingUser)

	var updateData SnmpV1V2cUser
	if err := c.ShouldBindJSON(&updateData); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	updates := make(map[string]interface{})

	if updateData.Version != "" {
		if !(updateData.Version == "v1" || updateData.Version == "v2c") {
			c.JSON(http.StatusBadRequest, gin.H{"error": "wrong version entry"})
			return
		}
		updates["version"] = updateData.Version
	}

	if updateData.GroupName != "" {
		if !(updateData.GroupName == "ronoauthgroup" || updateData.GroupName == "rwnoauthgroup") {
			c.JSON(http.StatusBadRequest, gin.H{"error": "wrong group name entry"})
			return
		}
		updates["group_name"] = updateData.GroupName
	}

	if updateData.Community != "" {
		updates["community"] = updateData.Community
	}

	//if updateData.IpVersion != "" {
	//	if !(updateData.IpVersion == "ipv4" || updateData.IpVersion == "ipv6") {
	//		c.JSON(http.StatusBadRequest, gin.H{"error": "wrong ip version entry"})
	//		return
	//	}
	//	updates["ip_version"] = updateData.IpVersion
	//}

	if updateData.Source != "" {
		updates["source"] = updateData.Source
	}

	//if updateData.Ip6Address != "" {
	//	updates["ip6_address"] = updateData.Ip6Address
	//}

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

	var updatedUser SnmpV1V2cUser
	if err := db.First(&updatedUser, id).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to fetch updated user"})
		return
	}
	addSnmpV1V2cUserToFile(updatedUser)

	c.JSON(http.StatusOK, gin.H{
		"message":    "User updated successfully",
		"v1v2c_user": updatedUser,
	})
}

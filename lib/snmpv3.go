package lib

import (
	"fmt"
	"log"
	"net/http"
	"os"
	"slices"
	"strings"

	"github.com/gin-gonic/gin"
)

func readSnmpV3Users(c *gin.Context) {
	_, v3, err := readSnmpUsersFromFile()
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"err": err,
		})
		return
	}
	c.JSON(http.StatusOK, gin.H{
		"v3_users": v3,
	})
}

func writeSnmpV3User(c *gin.Context) {
	var newUser SnmpV3User

	if err := c.ShouldBindJSON(&newUser); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	StopSnmpd()
	addSnmpV3UserToFile(newUser)
	StartSnmpd()

	c.JSON(http.StatusOK, gin.H{
		"message": "SNMP V3 User Created",
		"v3_user": newUser,
	})
}

func editSnmpV3User(c *gin.Context) {

	var userToEdit SnmpV3User
	if err := c.ShouldBindJSON(&userToEdit); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	existingUser := lookUpSnmpV3User(userToEdit)

	if existingUser == nil {
		c.JSON(http.StatusUnprocessableEntity, gin.H{
			"message": "user not found",
		})
		return
	}

	StopSnmpd()

	removeSnmpV3UserFromFile(*existingUser)

	addSnmpV3UserToFile(userToEdit)

	StartSnmpd()

	c.JSON(http.StatusOK, gin.H{
		"message": "User updated successfully",
		"v3_user": userToEdit,
	})

	return

}

func deleteSnmpV3User(c *gin.Context) {

	var userToDelete SnmpV3User

	if err := c.ShouldBindJSON(&userToDelete); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	existingUser := lookUpSnmpV3User(userToDelete)

	if existingUser == nil {
		c.JSON(http.StatusUnprocessableEntity, gin.H{
			"message": "user not found",
		})
		return
	}

	StopSnmpd()

	removeSnmpV3UserFromFile(*existingUser)

	StartSnmpd()

	c.JSON(http.StatusOK, gin.H{
		"message": "User removed successfully",
	})
}

func addSnmpV3UserToFile(user SnmpV3User) {

	content, err := os.ReadFile(AppConfig.Snmp.Path)

	if err != nil {
		log.Println(err)
	}

	lineCount := 0
	createUserIndex := -1
	groupIndex := -1

	lines := strings.Split(string(content), "\n")

	for _, line := range lines {

		if strings.Contains(line, "#createUser") {
			createUserIndex = lineCount + 2 // skip the header and blank line
		}
		if strings.Contains(line, "#group") {
			groupIndex = lineCount + 2
		}

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

	content, err := os.ReadFile(AppConfig.Snmp.Path)

	if err != nil {
		log.Println(err)
	}

	lines := strings.Split(string(content), "\n")

	if slices.Contains(lines, UserLineToDelete) && slices.Contains(lines, GroupLineToDelete) {

		lines = slices.DeleteFunc(lines, func(item string) bool {
			return item == UserLineToDelete
		})

		lines = slices.DeleteFunc(lines, func(item string) bool {
			return item == GroupLineToDelete
		})
	}

	err = os.WriteFile(AppConfig.Snmp.Path, []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Println("failed to write file:", err)
	}
}

func lookUpSnmpV3User(userToFind SnmpV3User) *SnmpV3User {

	_, v3, err := readSnmpUsersFromFile()

	if err != nil {
		fmt.Println("unable to read users", err.Error())
	}

	for i := range v3 {
		if userToFind.UserName == v3[i].UserName {
			return &v3[i]
		}
	}

	return nil
}

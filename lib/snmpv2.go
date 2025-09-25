package lib

import (
	"fmt"
	"log"
	"net/http"
	"os"
	"slices"
	"strconv"
	"strings"

	"github.com/gin-gonic/gin"
)

func readSnmpV2Users(c *gin.Context) {
	v2, _, err := readSnmpUsersFromFile()
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"err": err,
		})
		return
	}
	c.JSON(http.StatusOK, gin.H{
		"v1v2c_users": v2,
	})
}

func writeSnmpV2User(c *gin.Context) {
	var newUser SnmpV2User

	if err := c.ShouldBindJSON(&newUser); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	v2, _, _ := readSnmpUsersFromFile()
	newUser.ComNumber = strconv.FormatInt(int64(len(v2)), 10)

	StopSnmpd()
	addSnmpV2UserToFile(newUser)
	StartSnmpd()

	c.JSON(http.StatusOK, gin.H{
		"message":    "SNMP V1/V2c User Created",
		"v1v2c_user": newUser,
	})
}

func editSnmpV2User(c *gin.Context) {

	var userToEdit SnmpV2User
	if err := c.ShouldBindJSON(&userToEdit); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	existingUser := lookUpSnmpV2User(userToEdit)

	if existingUser == nil {
		c.JSON(http.StatusUnprocessableEntity, gin.H{
			"message": "user not found",
		})
		return
	}

	StopSnmpd()

	removeSnmpV2UserFromFile(*existingUser)

	addSnmpV2UserToFile(userToEdit)

	StartSnmpd()

	c.JSON(http.StatusOK, gin.H{
		"message":    "User updated successfully",
		"v1v2c_user": userToEdit,
	})

}

func deleteSnmpV2User(c *gin.Context) {

	var userToDelete SnmpV2User

	if err := c.ShouldBindJSON(&userToDelete); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	existingUser := lookUpSnmpV2User(userToDelete)

	if existingUser == nil {
		c.JSON(http.StatusUnprocessableEntity, gin.H{
			"message": "user not found",
		})
		return
	}

	StopSnmpd()

	removeSnmpV2UserFromFile(*existingUser)

	StartSnmpd()

	c.JSON(http.StatusOK, gin.H{
		"message": "User removed successfully",
	})
}

// files stuff

func addSnmpV2UserToFile(user SnmpV2User) {

	content, err := os.ReadFile(AppConfig.Snmp.Path)

	if err != nil {
		log.Println(err)
	}

	lineCount := 0
	userIndex := -1
	groupIndex := -1

	lines := strings.Split(string(content), "\n")

	for _, line := range lines {

		if strings.HasPrefix(line, "#com2sec") {
			userIndex = lineCount + 2 // skip the header and blank line
		}

		if strings.HasPrefix(line, "#group") {
			groupIndex = lineCount + 3
		}

		lineCount++
	}

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

	err = os.WriteFile(AppConfig.Snmp.Path, []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Println("failed to write file:", err)
	}
}

func removeSnmpV2UserFromFile(user SnmpV2User) {

	UserLineToDelete := fmt.Sprintf("com2sec %s %s %s", user.SecName, user.Source, user.Community)
	GroupLineToDelete := fmt.Sprintf("group %s %s %s", user.GroupName, user.Version, user.SecName)
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

// by sec name
func lookUpSnmpV2User(userToFind SnmpV2User) *SnmpV2User {

	v2, _, err := readSnmpUsersFromFile()

	if err != nil {
		fmt.Println("unable to read users", err.Error())
	}

	for i := range v2 {
		if userToFind.SecName == v2[i].SecName {
			return &v2[i]
		}
	}

	return nil
}

package api

import (
	"bufio"
	"fmt"
	"log"
	"net/http"
	"os"
	"os/exec"
	"strconv"
	"strings"

	"github.com/gin-gonic/gin"
)

const (
	SNMP_CONFIG_PATH = "SNMP_CONFIG_PATH"
)

func createSnmpStatus(c *gin.Context) {

	var snmpStatus Snmp

	if err := c.ShouldBindJSON(&snmpStatus); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	result := db.Create(&snmpStatus)

	if result.Error != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
		return
	}

	c.JSON(http.StatusCreated, gin.H{
		"message":     "SNMP Status Initialized",
		"snmp_status": snmpStatus,
	})
}

func readSnmpStatus(c *gin.Context) {

	var snmpStatus Snmp

	cmd := exec.Command("systemctl", "is-active", "snmpd")
	out, err := cmd.CombinedOutput()
	log.Println(err)
	log.Println("this the output: ", strings.TrimSpace(string(out)))

	snmpStatus.Status = strings.TrimSpace(string(out))

	c.JSON(http.StatusOK, gin.H{
		"message":     "Read SNMP Status",
		"snmp_status": snmpStatus.Status,
	})

}
func updateSnmpStatus(c *gin.Context) {
	var newSnmpStatus Snmp

	if err := c.ShouldBindJSON(&newSnmpStatus); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	cmd := exec.Command("systemctl", newSnmpStatus.Status, "snmpd")
	out, err := cmd.CombinedOutput()
	log.Println(err)
	log.Println("this the output: ", strings.TrimSpace(string(out)))

	newSnmpStatus.Status = strings.TrimSpace(string(out))

	c.JSON(http.StatusOK, gin.H{
		"message": "SNMP Status Updated",
		"status": gin.H{
			"status": newSnmpStatus.Status,
		},
	})
}
func deleteSnmpStatus(c *gin.Context) {}

// =============== snmp status ==================================

func getSnmpStatus(c *gin.Context) {

	cmd := exec.Command("systemctl", "is-active", "snmpd")
	out, err := cmd.CombinedOutput()
	log.Println(err)
	log.Println("this the output: ", strings.TrimSpace(string(out)))

	status := strings.TrimSpace(string(out))

	c.JSON(http.StatusOK, gin.H{
		"message": "Read SNMP Status",
		"status": gin.H{
			"status": status,
		},
	})

}

func setSnmpStatus(c *gin.Context) {

	var newSnmpStatus Snmp

	if err := c.ShouldBindJSON(&newSnmpStatus); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	cmd := exec.Command("systemctl", newSnmpStatus.Status, "snmpd")
	out, err := cmd.CombinedOutput()
	log.Println(err)
	log.Println("this the output: ", strings.TrimSpace(string(out)))

	newSnmpStatus.Status = strings.TrimSpace(string(out))

	c.JSON(http.StatusOK, gin.H{
		"message": "SNMP Status Updated",
		"status": gin.H{
			"status": newSnmpStatus.Status,
		},
	})

}

// =============== end snmp status ==================================
// =============== snmp system conf =================================

func readSnmpSysDetails(c *gin.Context) {

	var details Snmp
	file, err := os.Open(os.Getenv(SNMP_CONFIG_PATH))
	if err != nil {
		log.Fatal("failed to open config file")
	}
	scanner := bufio.NewScanner(file)

	// Iterate and print each line
	for scanner.Scan() {
		line := scanner.Text()

		fields := strings.Fields(line)
		if len(fields) > 1 {

			if strings.Contains(fields[0], "sysObjectID") {
				details.SysObjId = strings.Join(fields[1:], " ")
			}

			if strings.Contains(fields[0], "sysDescription") {
				details.SysDescription = strings.Join(fields[1:], " ")
			}

			if strings.Contains(fields[0], "sysLocation") {
				details.SysLocation = strings.Join(fields[1:], " ")
			}

			if strings.Contains(fields[0], "sysContact") {
				details.SysContact = strings.Join(fields[1:], " ")
			}
		}

	}

	c.JSON(http.StatusOK, gin.H{
		"snmp_sys_details": details,
	})

}

func updateSnmpSysDetails(c *gin.Context) {

	var details Snmp
	if err := c.ShouldBindJSON(&details); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}
	fmt.Println("HELP")
	fmt.Println(details)

	file, err := os.Open(os.Getenv(SNMP_CONFIG_PATH))
	if err != nil {
		log.Fatal("failed to open config file:", err)
	}
	defer file.Close()

	var lines []string
	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := scanner.Text()

		fields := strings.Fields(line)
		if len(fields) > 0 {

			switch fields[0] {
			case "sysObjectID":
				line = "sysObjectID " + details.SysObjId
			case "sysDescription":
				line = "sysDescription " + details.SysDescription
			case "sysLocation":
				line = "sysLocation " + details.SysLocation
			case "sysContact":
				line = "sysContact " + details.SysContact
			}
		}

		lines = append(lines, line)

	}

	if err := scanner.Err(); err != nil {
		log.Fatal("error reading file:", err)
	}

	// Write back to the file
	err = os.WriteFile(os.Getenv(SNMP_CONFIG_PATH), []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Fatal("failed to write file:", err)
	}

	c.JSON(http.StatusOK, gin.H{
		"snmp_sys_details": details,
	})
}

func createSnmpV1V2cUser(c *gin.Context) {
	var count int64
	var snmpV1V2cUser SnmpV1V2cUser

	if err := c.ShouldBindJSON(&snmpV1V2cUser); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	db.Model(&SnmpV1V2cUser{}).Count(&count)

	snmpV1V2cUser.ID = count + 1

	result := db.Create(&snmpV1V2cUser)
	if result.Error != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
		return
	}

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

	id := strconv.FormatInt(snmpV1V2cUser.ID, 10)

	newUserLine := []string{"com2sec " + "comuser_" + id + " " + snmpV1V2cUser.Source + " " + snmpV1V2cUser.Community}

	lines = append(lines[:userIndex], append(newUserLine, lines[userIndex:]...)...)

	newGroupLine := []string{"group " + convertGroups(snmpV1V2cUser.GroupName) + " " + snmpV1V2cUser.Version + " comuser_" + id}

	lines = append(lines[:groupIndex], append(newGroupLine, lines[groupIndex:]...)...)

	err = os.WriteFile(os.Getenv(SNMP_CONFIG_PATH), []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Fatal("failed to write file:", err)
	}

	c.JSON(http.StatusCreated, gin.H{
		"message":     "SNMP V1/V2c User Created",
		"snmp_v1_v2c": snmpV1V2cUser,
	})

}

func convertGroups(group string) string {
	if group == "read_only" {
		return "ronoauthgroup"
	}
	if group == "read_write" {
		return "rwnoauthgroup"
	}

	return "convert groups error"
}

func readSnmpV1V2cUser(c *gin.Context) {

	var users []SnmpV1V2cUser
	var groups []map[string]string
	var communities []map[string]string
	file, err := os.Open(os.Getenv(SNMP_CONFIG_PATH))
	if err != nil {
		log.Fatal("failed to open config file")
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	// Iterate and print each line
	for scanner.Scan() {
		line := scanner.Text()

		fields := strings.Fields(line)
		group := make(map[string]string)
		community := make(map[string]string)

		if len(fields) > 0 {
			// group - group name - sec.model - sec.name
			if strings.Contains(fields[0], "group") {
				group["group_name"] = strings.TrimSpace(fields[1])
				group["version"] = strings.TrimSpace(fields[2])
				group["sec_name"] = strings.TrimSpace(fields[3])
				groups = append(groups, group)
			}

			if strings.Contains(fields[0], "com2sec") {
				community["sec_name"] = strings.TrimSpace(fields[1])
				community["source"] = strings.TrimSpace(fields[2])
				community["community"] = strings.TrimSpace(fields[3])
				communities = append(communities, community)
			}
		}
	}

	for _, group := range groups {
		for _, community := range communities {
			if community["sec_name"] == group["sec_name"] {
				var user SnmpV1V2cUser
				user.Version = group["version"]
				user.GroupName = group["group_name"]
				user.Community = community["community"]
				user.Source = community["source"]
				user.SecName = community["sec_name"]

				users = append(users, user)
			}
		}
	}

	c.JSON(http.StatusOK, gin.H{
		"snmp_v1v2c_users": users,
		"total_users":      len(users),
	})

}

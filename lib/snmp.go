package lib

import (
	"bufio"
	"fmt"
	"log"
	"net/http"
	"os"
	"os/exec"
	"strings"

	"github.com/gin-gonic/gin"
)

func readSnmpUsersFromFile() ([]SnmpV2User, []SnmpV3User, error) {

	content, err := os.ReadFile(AppConfig.Snmp.Path)
	if err != nil {
		log.Println("failed to read config file", AppConfig.Snmp.Path)
		return nil, nil, err
	}

	lines := strings.Split(string(content), "\n")

	var groups []SnmpGroup
	var v2Users []SnmpV2User
	var v3Users []SnmpV3User

	for _, line := range lines {

		if strings.HasPrefix(line, "group") {
			var g SnmpGroup
			fields := strings.Fields(line)
			if len(fields) == 4 {
				g.GroupName = fields[1]
				g.Version = fields[2]
				g.SecName = fields[3]
				groups = append(groups, g)
			} else {
				log.Println("incomplete group entry")
			}
		}

		if strings.HasPrefix(line, "com2sec") {
			var v2 SnmpV2User
			fields := strings.Fields(line)
			if len(fields) == 4 {
				v2.SecName = fields[1]
				v2.Source = fields[2]
				v2.Community = fields[3]

				v2Users = append(v2Users, v2)
			} else {
				log.Println("incomplete v2 entry")
			}
		}

		if strings.HasPrefix(line, "createUser") {
			var v3 SnmpV3User
			fields := strings.Fields(line)
			if len(fields) == 6 {
				v3.UserName = fields[1]
				v3.AuthType = fields[2]
				v3.AuthPassphrase = fields[3]
				v3.PrivType = fields[4]
				v3.PrivPassphrase = fields[5]
				v3Users = append(v3Users, v3)
			} else {
				log.Println("incomplete v3 entry")
			}

		}

	}

	// collate

	for _, g := range groups {
		for i, v2 := range v2Users {
			if g.SecName == v2.SecName {
				v2Users[i].SecName = g.SecName
				v2Users[i].GroupName = g.GroupName
				v2Users[i].Version = g.Version
			}
		}

		for i := range v3Users {
			//v3Users[i]. = g.SecName
			v3Users[i].GroupName = g.GroupName
			v3Users[i].Version = g.Version
		}
	}

	return v2Users, v3Users, nil

}

func resetSnmpConfig(c *gin.Context) {

	StopSnmpd()

	CopySnmpdConfig()

	RestartSnmpd()

}

func ActiveOrInactive(inp string) bool {
	temp := strings.TrimSpace(inp)

	if temp == "active" || temp == "inactive" {
		return true
	}
	return false
}

func GetSnmpdStatus() string {
	cmd := exec.Command("systemctl", "is-active", "snmpd")
	output, err := cmd.CombinedOutput()
	out := string(output)
	out = strings.TrimSuffix(out, "\n")
	if ActiveOrInactive(out) {
		return out
	}
	return "error: " + err.Error()
}

func StopSnmpd() {

	DisablePort("161")
	DisablePort("162")

	cmd := exec.Command("systemctl", "stop", "snmpd")
	out, _ := cmd.CombinedOutput()
	fmt.Println(out)
	log.Println("SNMPD: ", GetSnmpdStatus())
}

func StartSnmpd() {

	EnablePort("161")
	EnablePort("162")

	cmd := exec.Command("systemctl", "start", "snmpd")
	out, _ := cmd.CombinedOutput()
	fmt.Println(out)
	log.Println("SNMPD: ", GetSnmpdStatus())
}

func RestartSnmpd() {
	cmd := exec.Command("systemctl", "restart", "snmpd")
	out, _ := cmd.CombinedOutput()
	fmt.Println(out)
	log.Println("SNMPD: ", GetSnmpdStatus())
}

func CopySnmpdConfig() {

	log.Println(AppConfig.Snmp.Path)

	cmd := exec.Command("cp", AppConfig.App.DefaultConfigs+"snmpd.conf", AppConfig.Snmp.Path)
	out, err := cmd.CombinedOutput()
	if err != nil {
		fmt.Println(err.Error())
	}
	log.Println(strings.TrimSpace(string(out)))
}

func readSnmpInfo(c *gin.Context) {

	var snmp Snmp
	file, err := os.Open(AppConfig.Snmp.Path)
	if err != nil {
		log.Println("failed to open config file", AppConfig.Snmp.Path)
	}

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()

		fields := strings.Fields(line)
		if len(fields) > 1 {

			if strings.Contains(fields[0], "sysObjectID") {
				snmp.SysObjId = strings.Join(fields[1:], " ")
			}

			if strings.Contains(fields[0], "sysDescr") {
				snmp.SysDescription = strings.Join(fields[1:], " ")
			}

			if strings.Contains(fields[0], "sysLocation") {
				snmp.SysLocation = strings.Join(fields[1:], " ")
			}

			if strings.Contains(fields[0], "sysContact") {
				snmp.SysContact = strings.Join(fields[1:], " ")
			}
		}
	}

	snmp.Status = GetSnmpdStatus()

	c.JSON(http.StatusOK, gin.H{
		"info": snmp,
	})

}

func writeSnmpInfo(c *gin.Context) {
	var snmp Snmp
	if err := c.ShouldBindJSON(&snmp); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	if snmp.Action == "start" {
		StartSnmpd()
	}

	if snmp.Action == "stop" {
		StopSnmpd()
	}

	snmp.Status = GetSnmpdStatus()

	file, err := os.Open(AppConfig.Snmp.Path)
	if err != nil {
		log.Println("failed to open config file:", AppConfig.Snmp.Path)
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
				line = "sysObjectID " + snmp.SysObjId
			case "sysDescription":
				line = "sysDescription " + snmp.SysDescription
			case "sysLocation":
				line = "sysLocation " + snmp.SysLocation
			case "sysContact":
				line = "sysContact " + snmp.SysContact
			}
		}

		lines = append(lines, line)

	}

	if err := scanner.Err(); err != nil {
		log.Println("error reading file:", err)
	}

	// Write back to the file
	err = os.WriteFile(AppConfig.Snmp.Path, []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Println("failed to write file:", err)
	}

	c.JSON(http.StatusOK, gin.H{
		"info": snmp,
	})
}

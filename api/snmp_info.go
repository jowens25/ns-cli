package api

import (
	"bufio"
	"log"
	"net/http"
	"os"
	"strings"

	"github.com/gin-gonic/gin"
)

// =============================== INFO ========================================
// readSnmpInfo read the status and general config of the snmp daemon
// Returns snmp_info json
func readSnmpInfo(c *gin.Context) {

	var snmp Snmp
	file, err := os.Open(os.Getenv(SNMP_CONFIG_PATH))
	if err != nil {
		log.Fatal("failed to open config file")
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

// writeSnmpInfo writes general values in the snmp daemon config and sets state of daemon
// Returns snmp_info json
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
		log.Fatal("error reading file:", err)
	}

	// Write back to the file
	err = os.WriteFile(os.Getenv(SNMP_CONFIG_PATH), []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Fatal("failed to write file:", err)
	}

	c.JSON(http.StatusOK, gin.H{
		"info": snmp,
	})
}

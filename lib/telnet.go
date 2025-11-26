package lib

import (
	"fmt"
	"log"
	"net/http"
	"os/exec"
	"strings"

	"github.com/gin-gonic/gin"
)

func InitTelnetConfig() {

	cmd := exec.Command("cp", AppConfig.App.DefaultConfigs+"telnet", AppConfig.Xinetd.TelnetPath)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}

	log.Println(strings.TrimSpace(string(out)))

}

func CleanTelnetSessions() {

	cmd := exec.Command("pkill", "-f", "telnetd")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println("telnet sessions clean up failed", string(out), err)
		return
	}

	log.Println("telnet clean up finished", strings.TrimSpace(string(out)))
}

func DisableTelnet() {
	conf := AppConfig.Xinetd.TelnetPath

	var lines []string

	for _, line := range OpenConfigFile(conf) {

		if strings.Contains(strings.TrimSpace(line), "disable = no") {
			line = "    disable = yes"
		}
		lines = append(lines, line)
	}

	SaveConfigFile(conf, lines)

	CleanTelnetSessions()

	RestartXinetd()

	log.Println(GetTelnetStatus())

}

func EnableTelnet() {
	conf := AppConfig.Xinetd.TelnetPath

	var lines []string

	for _, line := range OpenConfigFile(conf) {
		if strings.Contains(strings.TrimSpace(line), "disable = yes") {
			line = "    disable = no"
		}
		lines = append(lines, line)
	}

	SaveConfigFile(conf, lines)

	RestartXinetd()
	fmt.Println(GetTelnetStatus())

}

func GetTelnetStatus() string {

	conf := AppConfig.Xinetd.TelnetPath

	for _, line := range OpenConfigFile(conf) {

		if strings.Contains(line, "disable = yes") {
			return "inactive"
		} else if strings.Contains(line, "disable = no") {
			return "active"
		}
	}
	return "failed to get telnet status"
}

func getTelnetStatusHandler(c *gin.Context) {

	var telnet Telnet

	telnet.Status = GetTelnetStatus()

	c.JSON(http.StatusOK, gin.H{
		"info": telnet,
	})

}

func setTelnetStatusHandler(c *gin.Context) {
	var telnet Telnet
	if err := c.ShouldBindJSON(&telnet); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	if telnet.Action == "start" {
		EnableTelnet()
	}

	if telnet.Action == "stop" {
		DisableTelnet()
	}

	telnet.Status = GetTelnetStatus()

	c.JSON(http.StatusOK, gin.H{
		"info": telnet,
	})
}

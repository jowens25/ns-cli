package api

import (
	"fmt"
	"log"
	"net/http"
	"os/exec"
	"strings"

	"github.com/gin-gonic/gin"
)

func InitTelnetConfig() {

	cmd := exec.Command("cp", "telnet.socket", "/etc/systemd/system/telnet.socket")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}

	cmd = exec.Command("cp", "telnet@.service", "/etc/systemd/system/telnet@.service")
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}

	log.Println(strings.TrimSpace(string(out)))

}

func StopTelnet() {

	DisablePort("23")

	cmd := exec.Command("systemctl", "stop", "telnet.socket")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}
	fmt.Println("telnet: ", GetTelnetStatus())

}

func StartTelnet() {

	EnablePort("23")

	cmd := exec.Command("systemctl", "start", "telnet.socket")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}
	fmt.Println("telnet: ", GetTelnetStatus())

}

func GetTelnetStatus() string {
	cmd := exec.Command("systemctl", "is-active", "telnet.socket")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}
	return strings.TrimSpace(string(out))
}

func readTelnetStatus(c *gin.Context) {

	var telnet Telnet

	telnet.Status = GetTelnetStatus()

	c.JSON(http.StatusOK, gin.H{
		"info": telnet,
	})

}

func writeTelnetStatus(c *gin.Context) {
	var telnet Telnet
	if err := c.ShouldBindJSON(&telnet); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	if telnet.Action == "start" {
		StartTelnet()
	}

	if telnet.Action == "stop" {
		StopTelnet()
	}

	telnet.Status = GetTelnetStatus()

	c.JSON(http.StatusOK, gin.H{
		"info": telnet,
	})
}

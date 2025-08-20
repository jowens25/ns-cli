package lib

import (
	"fmt"
	"log"
	"net/http"
	"os/exec"
	"strings"

	"github.com/gin-gonic/gin"
)

func StopSsh() {

	DisablePort("22")

	cmd := exec.Command("systemctl", "stop", "ssh")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Print(string(out), err)
	}

	cmd = exec.Command("systemctl", "stop", "ssh.socket")
	out, err = cmd.CombinedOutput()

	if err != nil {
		log.Print(string(out), err)
	}

	fmt.Print("ssh: ", GetSshStatus())

}

func StartSsh() {

	EnablePort("22")

	cmd := exec.Command("systemctl", "start", "ssh")
	out, err := cmd.CombinedOutput()

	if err != nil {
		log.Print(string(out), err)
	}

	cmd = exec.Command("systemctl", "start", "ssh.socket")
	out, err = cmd.CombinedOutput()

	if err != nil {
		log.Print(string(out), err)
	}

	fmt.Print("ssh: ", GetSshStatus())

}

func ActiveOrInactive(inp []byte) bool {
	temp := strings.TrimSpace(string(inp))

	if temp == "active" || temp == "inactive" {
		return true
	}
	return false
}

func GetSshStatus() string {
	cmd := exec.Command("systemctl", "is-active", "ssh")
	output, err := cmd.CombinedOutput()
	if ActiveOrInactive(output) {
		return string(output)
	}
	return "error: " + err.Error()
}

func readSshStatus(c *gin.Context) {

	var ssh Ssh

	ssh.Status = GetSshStatus()

	c.JSON(http.StatusOK, gin.H{
		"info": ssh,
	})

}

func writeSshStatus(c *gin.Context) {
	var ssh Ssh
	if err := c.ShouldBindJSON(&ssh); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	if ssh.Action == "start" {
		StartSsh()
	}

	if ssh.Action == "stop" {
		StopSsh()
	}

	ssh.Status = GetSshStatus()

	c.JSON(http.StatusOK, gin.H{
		"info": ssh,
	})
}

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
		log.Println(string(out), err)
	}

	cmd = exec.Command("systemctl", "stop", "ssh.socket")
	out, err = cmd.CombinedOutput()

	if err != nil {
		log.Println(string(out), err)
	}

	fmt.Println("ssh: ", GetSshStatus())

}

func StartSsh() {

	EnablePort("22")

	cmd := exec.Command("systemctl", "start", "ssh")
	out, err := cmd.CombinedOutput()

	if err != nil {
		log.Println(string(out), err)
	}

	cmd = exec.Command("systemctl", "start", "ssh.socket")
	out, err = cmd.CombinedOutput()

	if err != nil {
		log.Println(string(out), err)
	}

	fmt.Println("ssh: ", GetSshStatus())

}

func GetSshStatus() string {
	cmd := exec.Command("systemctl", "is-active", "ssh")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}
	return strings.TrimSpace(string(out))
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

package lib

import (
	"fmt"
	"log"
	"net/http"
	"os/exec"
	"strings"

	"github.com/gin-gonic/gin"
)

func InitSshConfig() {

	cmd := exec.Command("cp", AppConfig.App.DefaultConfigs+"ssh", AppConfig.Xinetd.SshPath)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}

	log.Println(strings.TrimSpace(string(out)))

}

func CleanSshSessions() {

	cmd := exec.Command("pkill", "-f", "sshd: [a-zA-Z].*")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println("ssh sessions clean up failed", string(out), err)
		return
	}

	log.Println("ssh clean up finished", strings.TrimSpace(string(out)))

}

func DisableSsh() {
	conf := AppConfig.Xinetd.SshPath

	var lines []string

	for _, line := range OpenConfigFile(conf) {

		if strings.Contains(strings.TrimSpace(line), "disable = no") {
			line = "    disable = yes"
		}
		lines = append(lines, line)
	}

	SaveConfigFile(conf, lines)

	CleanSshSessions()

	RestartXinetd()

	log.Println(GetSshStatus())

}

func EnableSsh() {
	conf := AppConfig.Xinetd.SshPath

	var lines []string

	for _, line := range OpenConfigFile(conf) {
		if strings.Contains(strings.TrimSpace(line), "disable = yes") {
			line = "    disable = no"
		}
		lines = append(lines, line)
	}

	SaveConfigFile(conf, lines)

	RestartXinetd()
	fmt.Println(GetSshStatus())

}

func GetSshStatus() string {

	conf := AppConfig.Xinetd.SshPath

	for _, line := range OpenConfigFile(conf) {

		if strings.Contains(line, "disable = yes") {
			return "inactive"
		} else if strings.Contains(line, "disable = no") {
			return "active"
		}
	}
	return "failed to get ssh status"
}

func getSshStatusHandler(c *gin.Context) {

	var ssh Ssh

	ssh.Status = GetSshStatus()

	c.JSON(http.StatusOK, gin.H{
		"info": ssh,
	})

}

func setSshStatusHandler(c *gin.Context) {
	var ssh Ssh
	if err := c.ShouldBindJSON(&ssh); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	if ssh.Action == "start" {
		EnableSsh()
	}

	if ssh.Action == "stop" {
		DisableSsh()
	}

	ssh.Status = GetSshStatus()

	c.JSON(http.StatusOK, gin.H{
		"info": ssh,
	})
}

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

func InitSshConfig() {

	cmd := exec.Command("cp", "ssh", AppConfig.Xinetd.SshPath)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}

	log.Println(strings.TrimSpace(string(out)))

}

func DisableSsh() {
	file, err := os.Open(AppConfig.Xinetd.SshPath)
	if err != nil {
		log.Fatal("failed to open ssh file", file.Name())
	}
	defer file.Close()

	var lines []string

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()

		if strings.Contains(strings.TrimSpace(line), "disable = no") {
			line = "    disable = yes"
		}
		lines = append(lines, line)
	}
	err = os.WriteFile(AppConfig.Xinetd.SshPath, []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Fatal("failed to hosts file:", err)
	}

	RestartXinetd()

	fmt.Println(GetSshStatus())

}

func EnableSsh() {
	file, err := os.Open(AppConfig.Xinetd.SshPath)
	if err != nil {
		log.Fatal("failed to open ssh file", file.Name())
	}
	defer file.Close()

	var lines []string

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		if strings.Contains(strings.TrimSpace(line), "disable = yes") {
			line = "    disable = no"
		}
		lines = append(lines, line)
	}

	err = os.WriteFile(AppConfig.Xinetd.SshPath, []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Fatal("failed to ssh file:", err)
	}

	RestartXinetd()
	fmt.Println(GetSshStatus())

}

func GetSshStatus() string {
	file, err := os.Open(AppConfig.Xinetd.SshPath)
	if err != nil {
		log.Fatal("failed to open ssh file", file.Name())
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()

		if strings.Contains(line, "disable = yes") {
			return "inactive"
		} else if strings.Contains(line, "disable = no") {
			return "active"
		}
	}
	return "failed to get ssh status"
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

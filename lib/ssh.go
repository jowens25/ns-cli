package lib

import (
	"bufio"
	"log"
	"net/http"
	"os"
	"os/exec"
	"strings"

	"github.com/gin-gonic/gin"
)

func InitSshConfig() {

	cmd := exec.Command("cp", "ssh", "/etc/xinetd.d/ssh")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}

	log.Println(strings.TrimSpace(string(out)))

}

func DisableSsh() {
	sshFile := "/etc/xinetd.d/ssh"
	file, err := os.Open(sshFile)
	if err != nil {
		log.Fatal("failed to open ssh file", file.Name())
	}
	defer file.Close()

	var lines []string

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()

		if strings.Contains(line, "disable = no") {
			line = "\tdisable = yes"
		}
		lines = append(lines, line)
	}

	err = os.WriteFile(sshFile, []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Fatal("failed to hosts file:", err)
	}

	RestartXinetd()

}

func EnableSsh() {
	sshFile := "/etc/xinetd.d/ssh"
	file, err := os.Open(sshFile)
	if err != nil {
		log.Fatal("failed to open ssh file", file.Name())
	}
	defer file.Close()

	var lines []string

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()

		if strings.Contains(line, "disable = yes") {
			line = "\tdisable = no"
		}
		lines = append(lines, line)
	}

	err = os.WriteFile(sshFile, []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Fatal("failed to ssh file:", err)
	}

	RestartXinetd()

}

func GetSshStatus() string {
	sshFile := "/etc/xinetd.d/ssh"
	file, err := os.Open(sshFile)
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

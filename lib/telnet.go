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

func InitTelnetConfig() {

	cmd := exec.Command("cp", AppConfig.App.DefaultConfigs+"telnet", AppConfig.Xinetd.TelnetPath)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}

	log.Println(strings.TrimSpace(string(out)))

}

func DisableTelnet() {
	file, err := os.Open(AppConfig.Xinetd.TelnetPath)
	if err != nil {
		log.Println("failed to open telnet file", AppConfig.Xinetd.TelnetPath)
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
	err = os.WriteFile(AppConfig.Xinetd.TelnetPath, []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Println("failed to hosts file:", err)
	}

	RestartXinetd()

	fmt.Println(GetTelnetStatus())

}

func EnableTelnet() {
	file, err := os.Open(AppConfig.Xinetd.TelnetPath)
	if err != nil {
		log.Println("failed to open telnet file", AppConfig.Xinetd.TelnetPath)
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

	err = os.WriteFile(AppConfig.Xinetd.TelnetPath, []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Println("failed to telnet file:", err)
	}

	RestartXinetd()
	fmt.Println(GetTelnetStatus())

}

func GetTelnetStatus() string {

	file, err := os.Open(AppConfig.Xinetd.TelnetPath)
	if err != nil {
		log.Println("failed to open telnet file", AppConfig.Xinetd.TelnetPath)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()

		if strings.Contains(strings.TrimSpace(line), "disable = yes") {
			return "inactive"
		} else if strings.Contains(strings.TrimSpace(line), "disable = no") {
			return "active"
		}
	}
	return "failed to get telnet status"
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

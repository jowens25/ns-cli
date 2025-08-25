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

func InitTelnetConfig() {

	cmd := exec.Command("cp", "telnet", "/etc/xinetd.d/telnet")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}

	log.Println(strings.TrimSpace(string(out)))

}

func DisableTelnet() {
	telnetFile := "/etc/xinetd.d/telnet"
	file, err := os.Open(telnetFile)
	if err != nil {
		log.Fatal("failed to open telnet file", file.Name())
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
	err = os.WriteFile(telnetFile, []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Fatal("failed to hosts file:", err)
	}

	RestartXinetd()

}

func EnableTelnet() {
	telnetFile := "/etc/xinetd.d/telnet"
	file, err := os.Open(telnetFile)
	if err != nil {
		log.Fatal("failed to open telnet file", file.Name())
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

	err = os.WriteFile(telnetFile, []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Fatal("failed to telnet file:", err)
	}

	RestartXinetd()

}

func GetTelnetStatus() string {
	telnetFile := "/etc/xinetd.d/telnet"
	file, err := os.Open(telnetFile)
	if err != nil {
		log.Fatal("failed to open telnet file", file.Name())
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

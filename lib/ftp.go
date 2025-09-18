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

func InitFtpConfig() {

	cmd := exec.Command("cp", "ftp", "/etc/xinetd.d/ftp")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}

	log.Println(strings.TrimSpace(string(out)))

}

func DisableFtp() {
	ftpFile := "/etc/xinetd.d/ftp"
	file, err := os.Open(ftpFile)
	if err != nil {
		log.Println("failed to open ftp file", ftpFile)
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
	err = os.WriteFile(ftpFile, []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Println("failed to hosts file:", err)
	}

	RestartXinetd()

	fmt.Println(GetFtpStatus())

}

func EnableFtp() {
	ftpFile := "/etc/xinetd.d/ftp"
	file, err := os.Open(ftpFile)
	if err != nil {
		log.Println("failed to open ftp file", ftpFile)
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

	err = os.WriteFile(ftpFile, []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Println("failed to ftp file:", err)
	}

	RestartXinetd()

	fmt.Println(GetFtpStatus())

}

func GetFtpStatus() string {
	ftpFile := "/etc/xinetd.d/ftp"
	file, err := os.Open(ftpFile)
	if err != nil {
		log.Println("failed to open ftp file", ftpFile)
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
	return "failed to get ftp status"
}

func readFtpStatus(c *gin.Context) {

	var ftp Ftp

	ftp.Status = GetFtpStatus()

	c.JSON(http.StatusOK, gin.H{
		"info": ftp,
	})

}

func writeFtpStatus(c *gin.Context) {
	var ftp Ftp
	if err := c.ShouldBindJSON(&ftp); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	if ftp.Action == "start" {
		EnableFtp()
	}

	if ftp.Action == "stop" {
		DisableFtp()
	}

	ftp.Status = GetFtpStatus()

	c.JSON(http.StatusOK, gin.H{
		"info": ftp,
	})
}

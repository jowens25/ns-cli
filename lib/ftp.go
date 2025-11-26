package lib

import (
	"log"
	"net/http"
	"os/exec"
	"strings"

	"github.com/gin-gonic/gin"
)

func InitFtpConfig() {

	cmd := exec.Command("cp", AppConfig.App.DefaultConfigs+"ftp", AppConfig.Xinetd.FtpPath)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}

	log.Println(strings.TrimSpace(string(out)))

}

func CleanFtpSessions() {
	cmd := exec.Command("pkill", "-f", "pure-ftpd -E")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println("ftp sessions clean up failed", string(out), err)
		return
	}

	log.Println("ftp clean up finished", strings.TrimSpace(string(out)))
}

func DisableFtp() {

	conf := AppConfig.Xinetd.FtpPath

	var lines []string

	for _, line := range OpenConfigFile(conf) {

		if strings.Contains(strings.TrimSpace(line), "disable = no") {
			line = "    disable = yes"
		}
		lines = append(lines, line)
	}

	SaveConfigFile(conf, lines)

	CleanFtpSessions()

	RestartXinetd()

	log.Println(GetFtpStatus())

}

func EnableFtp() {
	conf := AppConfig.Xinetd.FtpPath

	var lines []string

	for _, line := range OpenConfigFile(conf) {
		if strings.Contains(strings.TrimSpace(line), "disable = yes") {
			line = "    disable = no"
		}
		lines = append(lines, line)
	}

	SaveConfigFile(conf, lines)

	RestartXinetd()

	log.Println(GetFtpStatus())

}

func GetFtpStatus() string {
	conf := AppConfig.Xinetd.FtpPath

	for _, line := range OpenConfigFile(conf) {

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

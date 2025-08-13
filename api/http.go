package api

import (
	"fmt"
	"log"
	"net/http"
	"os"
	"os/exec"
	"strconv"
	"strings"
	"syscall"

	"github.com/gin-gonic/gin"
)

func StopHttp() {

	DisablePort(WEB_PORT)
	DisablePort(API_PORT)

	data, err := os.ReadFile("server.pid")
	if err != nil {
		log.Fatalf("Could not read PID file: %v", err)
	}
	pid, _ := strconv.Atoi(string(data))
	process, err := os.FindProcess(pid)
	if err != nil {
		log.Fatalf("Could not find process: %v", err)
	}
	process.Signal(syscall.SIGTERM)

}

func StartHttp() {

	EnablePort(WEB_PORT)
	EnablePort(API_PORT)

	pid := os.Getpid()
	os.WriteFile("server.pid", []byte(fmt.Sprintf("%d", pid)), 0644)

	startJsServer()

	initDataBase()

	startApiServer()

}

func GetHttpStatus() string {

	data, err := os.ReadFile("server.pid")
	if err != nil {
		log.Fatalf("Could not read PID file: %v", err)
	}

	pid, _ := strconv.Atoi(string(data))

	cmdStr := fmt.Sprintf("[ -d /proc/%d ] && echo active", pid)
	cmd := exec.Command("bash", "-c", cmdStr)

	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(err)
	}
	return strings.TrimSpace(string(out))
}

func readHttpStatus(c *gin.Context) {

	var myhttp Http

	myhttp.Status = GetHttpStatus()

	c.JSON(http.StatusOK, gin.H{
		"info": myhttp,
	})

}

func writeHttpStatus(c *gin.Context) {
	var myhttp Http
	if err := c.ShouldBindJSON(&myhttp); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	if myhttp.Action == "start" {
		StartHttp()
	}

	if myhttp.Action == "stop" {
		StopHttp()
	}

	myhttp.Status = GetHttpStatus()

	c.JSON(http.StatusOK, gin.H{
		"info": myhttp,
	})
}

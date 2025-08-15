package api

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

func StopHttp() {

	DisablePort("80")

}

func StartHttp() {

	EnablePort("80")

}

func GetHttpStatus() string {

	return GetPortStatus("80")
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

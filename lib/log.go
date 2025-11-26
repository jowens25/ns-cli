package lib

import (
	"fmt"
	"net/http"
	"os"

	"github.com/gin-gonic/gin"
)

func readLogHandler(c *gin.Context) {

	content, err := os.ReadFile(AppConfig.App.Log)
	if err != nil {
		fmt.Println("log open error")
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"log": string(content),
	})
}

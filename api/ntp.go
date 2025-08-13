package api

import (
	"NovusTimeServer/axi"
	"log"
	"net/http"

	"github.com/gin-gonic/gin"
)

func readNtpProperty(c *gin.Context) {
	serialMutex.Lock()
	defer serialMutex.Unlock()
	property := c.Param("prop")
	operation := "read"
	module := "ntp"
	value := ""

	err := axi.Operate(&operation, &module, &property, &value)

	if err != nil {
		log.Println("axi operate error in ntp read")
		log.Println(err.Error())

		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	c.JSON(http.StatusOK, gin.H{property: value})
}

func writeNtpProperty(c *gin.Context) {
	serialMutex.Lock()
	defer serialMutex.Unlock()
	var data map[string]string
	if err := c.ShouldBindJSON(&data); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}
	operation := "write"
	module := "ntp"
	property := c.Param("prop")
	//value := ""
	value := data[property]

	err := axi.Operate(&operation, &module, &property, &value)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusOK, gin.H{property: value})

}

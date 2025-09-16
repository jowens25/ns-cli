package lib

import (
	"NovusTimeServer/axi"
	"net/http"

	"github.com/gin-gonic/gin"
)

func getDeviceProperty(property string) string {
	switch property {
	case "baudrate":

		return ReadWriteMicro("$BAUDNV")

	default:
		return ""
	}
}

func readDeviceProperty(c *gin.Context) {
	serialMutex.Lock()
	defer serialMutex.Unlock()

	property := c.Param("prop")

	value := getDeviceProperty(property)

	c.JSON(http.StatusOK, gin.H{property: value})
}

func writeDeviceProperty(c *gin.Context) {
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

	err := axi.Operation(&operation, &module, &property, &value)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusOK, gin.H{property: value})

}

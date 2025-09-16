package lib

import (
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

func setDeviceProperty(property string, value string) {
	switch property {

	case "save_flash":
		ReadWriteMicro("$SAVEFL")

	case "reset_flash":
		ReadWriteMicro("$RESETALL")

	case "input_priority":
		ReadWriteMicro("$INP=" + value)

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

	property := c.Param("prop")
	value := data[property]

	setDeviceProperty(property, value)

	c.JSON(http.StatusOK, gin.H{property: value})

}

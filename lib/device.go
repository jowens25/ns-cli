package lib

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

func getDeviceProperty(property string) (string, error) {
	switch property {

	case "baudrate":
		return ReadWriteMicro("$BAUDNV")

	case "input_priority":
		return ReadWriteMicro("$INP")

	case "fault_threshold_a":
		return ReadWriteMicro("$FLTTHRA")

	case "fault_threshold_b":
		return ReadWriteMicro("$FLTTHRB")

	case "input_low_threshold_0":
		return ReadWriteMicro("$INPTHR0")

	case "input_low_threshold_1":
		return ReadWriteMicro("$INPTHR1")

	default:
		return "", nil
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

	value, err := getDeviceProperty(property)

	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{property: value})

	}

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

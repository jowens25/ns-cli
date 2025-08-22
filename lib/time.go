package lib

import (
	"os/exec"
	"strings"
)

func GetTime() string {

	myCmd := exec.Command("date", "+%T")
	out, err := myCmd.CombinedOutput()
	if err != nil {
		return err.Error()
	}

	return strings.TrimSpace(string(out))

}

func SetTime(date []string) string {

	if len(date) != 3 {
		return "error"
	}
	hour := date[0]
	minute := date[1]
	second := date[2]

	myCmd := exec.Command("date", "-s", hour+":"+minute+":"+second)
	_, err := myCmd.CombinedOutput()
	if err != nil {
		return err.Error()
	}

	return GetTime()
}

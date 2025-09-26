package lib

import (
	"log"
	"os/exec"
	"strings"
)

func GetDate() string {

	myCmd := exec.Command("date")
	out, err := myCmd.CombinedOutput()
	if err != nil {
		return err.Error()
	}
	fields := strings.Fields(strings.TrimSpace(string(out)))

	return fields[2] + " " + fields[1] + " " + fields[3] + " " + fields[6]
}

func SetLatest() string {
	cmd := exec.Command("bash", "-c", `date -s "$(wget --method=HEAD -qSO- --max-redirect=0 google.com 2>&1 | sed -n 's/^ *Date: *//p')"`)

	out, err := cmd.CombinedOutput()
	if err != nil {
		return err.Error()
	}
	log.Println(string(out))

	return GetDate()
}

func SetDate(date []string) string {

	if len(date) != 3 {
		return "error"
	}
	year := date[0]
	month := date[1]
	day := date[2]

	myCmd := exec.Command("date", "-s", year+"-"+month+"-"+day)
	out, err := myCmd.CombinedOutput()
	if err != nil {
		return err.Error()
	}
	fields := strings.Fields(strings.TrimSpace(string(out)))

	return fields[2] + " " + fields[1] + " " + fields[6]
}

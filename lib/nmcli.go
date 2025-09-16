package lib

import (
	"log"
	"os/exec"
	"strings"
)

func GetNmcliField(f string, i string) string {

	cmd := exec.Command("nmcli", "-f", f, "device", "show", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal(err.Error())
	}

	if strings.Contains(string(out), f) {
		return string(out)
	}

	return "--"
}

func GetNmcliConnectionField(f string, c string) string {
	cmd := exec.Command("nmcli", "-f", f, "connection", "show", c)

	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal(err.Error())
	}

	if strings.Contains(string(out), f) {
		return string(out)
	}

	return "--"
}

// status: up / down
func SetNmcliConnectionStatus(c string, s string) {
	cmd := exec.Command("nmcli", "connection", s, c)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(err.Error())
	}
	log.Println(string(out))

}

// connection, setting, property
func SetNmcliField(c string, s string, p string) {
	cmd := exec.Command("nmcli", "connection", "modify", c, s, p)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(err.Error())
	}
	log.Println(string(out))
}

func GetConnectionNameFromDevice(i string) string {

	cmd := exec.Command("nmcli", "-f", "GENERAL.CONNECTION", "dev", "show", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal(err.Error())
	}

	fields := strings.Split(string(out), ":")

	if len(fields) == 2 {

		return strings.TrimSpace(fields[1])
	}

	return "--"
}

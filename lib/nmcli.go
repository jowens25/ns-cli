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

// connection, setting, property
func EditNmcliConnection(c string, s string, p string) {
	cmd := exec.Command("nmcli", "connection", "modify", c, s, p)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(err.Error())
		log.Println(string(out))
	}
}

// status: up / down
func SetNmcliConnectionStatus(c string, s string) {
	cmd := exec.Command("nmcli", "connection", s, c)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(err.Error())
		log.Println(string(out))
	}
}

// interface, setting, property
func SetNmcliField(i string, s string, p string) {
	// 1. get connection from device
	// 2. use con modify
	// 3. bring up
	connection := GetConnectionNameFromDevice(i)
	EditNmcliConnection(connection, s, p)
	SetNmcliConnectionStatus(connection, "up")

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

	return "eth0"
}

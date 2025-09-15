package lib

import (
	"fmt"
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
		fmt.Println(err.Error())
	}
	fmt.Println(string(out))

}

// status: up / down
func SetNmcliConnectionStatus(c string, s string) {
	cmd := exec.Command("nmcli", "connection", s, c)
	out, err := cmd.CombinedOutput()
	if err != nil {
		fmt.Println(err.Error())
	}
	fmt.Println(string(out))

}

// interface, setting, property
func SetNmcliField(i string, s string, p string) {
	connection := GetConnectionNameFromDevice(i)
	//SetNmcliConnectionStatus(connection, "down")

	// 1. get connection from device
	// 2. use con modify
	// 3. bring up
	EditNmcliConnection(connection, s, p)
	//time.Sleep(1 * time.Second)

	//SetNmcliConnectionStatus(connection, "up")

	ReapplyNmcli(i)

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

func ReapplyNmcli(i string) {
	// Try reapply first (faster than restart)
	cmd := exec.Command("nmcli", "device", "reapply", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Printf("Device reapply failed, trying connection restart: %v", err)
		log.Printf("Reapply output: %s", string(out))

	}
}

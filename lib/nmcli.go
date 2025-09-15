package lib

import (
	"fmt"
	"log"
	"os/exec"
	"strings"
	"time"
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
	SetNmcliConnectionStatus(connection, "down")

	// 1. set connection down
	// 2. get connection from device
	// 3. use con modify
	// 4. bring up
	time.Sleep(500 * time.Millisecond)

	EditNmcliConnection(connection, s, p)
	time.Sleep(500 * time.Millisecond)

	SetNmcliConnectionStatus(connection, "up")

	//ReapplyNmcli(i)

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

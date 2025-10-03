package lib

import (
	"fmt"
	"log"
	"os/exec"
	"strings"
)

// connection, setting, return prop
func GetNmcliField(c string, s string) string {
	cmd := exec.Command("nmcli", "-f", s, "connection", "show", c)

	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(err.Error())
	}

	if strings.Contains(string(out), s) {
		return string(out)
	}

	return "--"
}

func GetNmcliInterfaceField(i string, f string) string {

	cmd := exec.Command("nmcli", "-f", f, "device", "show", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(err.Error())
	}

	if strings.Contains(string(out), f) {
		return string(out)
	}

	return "--"
}

// connection, setting, property
func SetNmcliField(c string, s string, p string) {
	cmd := exec.Command("nmcli", "con", "mod", c, s, p)
	out, err := cmd.CombinedOutput()
	if err != nil {
		fmt.Println(err.Error())
		fmt.Println(string(out))

	}

}

func ClearNmcliField(c string, s string) {
	SetNmcliField(c, s, "")
}

func GetConnectionNameFromDevice(i string) (string, bool) {

	cmd := exec.Command("nmcli", "-f", "GENERAL.CONNECTION", "dev", "show", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(err.Error())
	}

	fields := strings.Split(string(out), ":")

	if len(fields) == 2 {

		return strings.TrimSpace(fields[1]), true
	}

	return "--", false
}

func GetInterfaceNameFromConnection(c string) string {

	cmd := exec.Command("nmcli", "-f", "connection.interface-name", "con", "show", c)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(err.Error())
	}

	fields := strings.Split(string(out), ":")

	if len(fields) == 2 {

		return strings.TrimSpace(fields[1])
	}

	return "--"
}

// status: up / down
func SetNmcliConnectionStatus(c string, s string) {
	cmd := exec.Command("nmcli", "connection", s, c)
	out, err := cmd.CombinedOutput()
	if err != nil {
		fmt.Println(err.Error())
		fmt.Println(string(out))

	}

}

func DisableNetworking() {

	cmd := exec.Command("systemctl", "disable", "networking")
	out, err := cmd.CombinedOutput()
	if err != nil {
		fmt.Println(err.Error())

	}
	fmt.Println(string(out))
}

func MakeDefaultNmcliConnection() {

	DeleteDefaultNmcliConnection(AppConfig.Network.DefaultConnectionName)

	cmd := exec.Command("nmcli", "connection", "add", "type", "ethernet", "con-name", AppConfig.Network.DefaultConnectionName, "ifname", AppConfig.Network.Interface)
	out, err := cmd.CombinedOutput()
	if err != nil {
		fmt.Println(err.Error())

	}
	fmt.Println(string(out))

}

func DeleteDefaultNmcliConnection(name string) {

	cmd := exec.Command("nmcli", "connection", "delete", name)
	out, err := cmd.CombinedOutput()
	if err != nil {
		fmt.Println(err.Error())
		fmt.Println(string(out))

	}

}

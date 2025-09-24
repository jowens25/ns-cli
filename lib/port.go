package lib

import (
	"fmt"
	"log"
	"os/exec"
	"strings"
)

func GetPortPhysicalStatus(i string) string {

	cmd := exec.Command("nmcli", "device", "show", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		return err.Error()
	}
	lines := strings.SplitSeq(strings.TrimSpace(string(out)), "\n")

	for line := range lines {
		fields := strings.Fields(line)

		if len(fields) != 2 {
			continue
		}

		if strings.Contains(strings.TrimSpace(fields[0]), "WIRED-PROPERTIES") {
			switch fields[1] {
			case "on":
				return "connected"
			case "off":
				return "disconnected"
			default:
				return "unable to determine physical status"
			}
		}

	}

	return "unable to determine physical status"
}

func GetPortConnectionStatus(i string) string {

	cmd := exec.Command("nmcli", "device", "show", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		return err.Error()
	}
	lines := strings.SplitSeq(strings.TrimSpace(string(out)), "\n")

	for line := range lines {
		fields := strings.Fields(line)

		if len(fields) != 3 {
			continue
		}

		//fmt.Println(fields[2])

		if strings.Contains(strings.TrimSpace(fields[0]), "GENERAL.STATE") {
			switch fields[2] {
			case "(connected)":
				return "up"
			case "(disconnected)":
				return "down"
			case "(unavailable)":
				return "unavailable"
			default:
				return "not found"
			}
		}

	}

	return "not found"
}

func PortConnect(device string) error {
	cmd := exec.Command("nmcli", "device", "connect", device)
	_, err := cmd.CombinedOutput()
	return err
}

func PortDisconnect(device string) error {
	cmd := exec.Command("nmcli", "device", "disconnect", device)
	_, err := cmd.CombinedOutput()
	return err
}

func GetPortSpeed(i string) string {
	cmd := exec.Command("nmcli", "-f", "CAPABILITIES.SPEED", "dev", "show", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(err.Error())
	}

	fields := strings.Split(string(out), ":")

	if len(fields) == 2 {
		return strings.TrimSpace(fields[1])
	}

	return "speed not available"
}

func HasInterface(i string) bool {
	cmd := exec.Command("nmcli", "dev", "show", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		fmt.Println(string(out))
		return false
	}
	return true
}

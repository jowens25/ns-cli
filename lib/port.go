package lib

import (
	"fmt"
	"log"
	"os/exec"
	"strings"
	"time"
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
				return "connection: up"
			case "(disconnected)":
				return "connection: down"
			case "(unavailable)":
				return "connection: unavailable"
			default:
				return "unable to determine connection state"
			}
		}

	}

	return "unable to determine connection state"
}

func GetConnections() string {
	cmd := exec.Command("nmcli", "connection", "show")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal(err.Error())
	}

	return string(out)
}

func waitingDots() {
	fmt.Print("Please wait")

	for range 5 {
		fmt.Print(".")
		time.Sleep(1 * time.Second)
	}
	fmt.Println()
}

func PortConnect(i string) {
	cmd := exec.Command("nmcli", "device", "connect", i)
	_, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal(err.Error())
	}
}

func PortDisconnect(i string) {
	cmd := exec.Command("nmcli", "device", "disconnect", i)
	_, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal(err.Error())
	}
}

func GetPortSpeed(i string) string {
	cmd := exec.Command("nmcli", "-f", "CAPABILITIES.SPEED", "dev", "show", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal(err.Error())
	}

	fields := strings.Split(string(out), ":")

	if len(fields) == 2 {
		return strings.TrimSpace(fields[1])
	}

	return "speed not available"
}

package lib

import (
	"bufio"
	"bytes"
	"fmt"
	"log"
	"os/exec"
	"strings"
	"time"
)

func GetInterfacePhysicalStatus(myInterface string) string {
	cmd := exec.Command("ethtool", myInterface)
	out, err := cmd.CombinedOutput()

	if strings.Contains(string(out), "Link detected: yes") {
		status := myInterface
		scanner := bufio.NewScanner(bytes.NewReader(out))
		for scanner.Scan() {
			line := scanner.Text()
			if strings.Contains(line, "Speed: ") {
				status = status + " (" + line[8:]
			}
			if strings.Contains(line, "Duplex: ") {
				status = status + line[8:] + ")"
			}
		}
		return status
	} else if strings.Contains(string(out), "Link detected: no") {
		return myInterface + " (Unplugged)"
	} else {
		return err.Error()
	}
}

func waitingDots() {
	fmt.Print("Please wait")

	for range 5 {
		fmt.Print(".")
		time.Sleep(1 * time.Second)
	}
	fmt.Println()
}

func DisableInterface(i string) string {
	cmd := exec.Command("nmcli", "connection", "modify", i, "connection.autoconnect", "no")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println("Failed to disable autoconnect:", string(out), err)
	}

	cmd = exec.Command("nmcli", "connection", "down", i)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println("Failed to bring down connection:", string(out), err)
	}

	return GetInterfaceNetworkStatus(i)
}

func EnableInterface(i string) string {

	cmd := exec.Command("nmcli", "-t", "-f", "NAME", "connection", "show", i)
	out, err := cmd.CombinedOutput()

	if err != nil || strings.TrimSpace(string(out)) == "" {
		cmd = exec.Command("nmcli", "connection", "add", "type", "ethernet", "ifname", i, "con-name", i)
		out, err = cmd.CombinedOutput()
		if err != nil {
			log.Println("Failed to create connection:", string(out), err)
			return "failed to create connection"
		}
	}

	cmd = exec.Command("nmcli", "connection", "modify", i, "connection.autoconnect", "yes")
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println("Failed to enable autoconnect:", string(out), err)
	}

	cmd = exec.Command("nmcli", "connection", "up", i)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println("Failed to bring up connection:", string(out), err)
	}

	waitingDots()

	return GetInterfaceNetworkStatus(i)
}

func GetInterfaceNetworkStatus(i string) string {
	cmd := exec.Command("nmcli", "-t", "-f", "connection.autoconnect", "connection", "show", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		return "inactive"
	}

	autoconnect := strings.TrimSpace(string(out))
	if autoconnect == "yes" {
		cmd = exec.Command("nmcli", "-t", "-f", "DEVICE,STATE", "device", "status")
		out, err = cmd.CombinedOutput()
		if err != nil {
			return "active" // autoconnect is yes, assume active even if we can't check device state
		}

		lines := strings.Split(string(out), "\n")
		for _, line := range lines {
			parts := strings.Split(line, ":")
			if len(parts) >= 2 && parts[0] == i {
				state := parts[1]
				if state == "connected" || state == "connecting" {
					return "active"
				}
			}
		}

		// Connection exists and autoconnect is yes, but device might be down
		return "active"
	}

	return "inactive"
}

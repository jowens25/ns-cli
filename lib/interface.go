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

// DisableInterface - Disables both IPv4 and IPv6 on the interface
func DisableInterface(i string) string {
	// First, ensure the connection exists (create if needed)
	ensureConnectionExists(i)

	// Disable IPv4
	cmd := exec.Command("nmcli", "connection", "modify", i, "ipv4.method", "disabled")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println("Failed to disable IPv4:", string(out), err)
	}

	// Disable IPv6
	cmd = exec.Command("nmcli", "connection", "modify", i, "ipv6.method", "disabled")
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println("Failed to disable IPv6:", string(out), err)
	}

	// Disable autoconnect
	cmd = exec.Command("nmcli", "connection", "modify", i, "connection.autoconnect", "no")
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println("Failed to disable autoconnect:", string(out), err)
	}

	// Bring down the connection to apply changes
	cmd = exec.Command("nmcli", "connection", "down", i)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println("Failed to bring down connection:", string(out), err)
	}

	return GetInterfaceNetworkStatus(i)
}

// EnableInterface - Enables both IPv4 and IPv6 on the interface
func EnableInterface(i string) string {
	// Ensure the connection exists (create if needed)
	ensureConnectionExists(i)

	// Enable IPv4 with automatic configuration (DHCP)
	cmd := exec.Command("nmcli", "connection", "modify", i, "ipv4.method", "auto")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println("Failed to enable IPv4:", string(out), err)
	}

	// Enable IPv6 with automatic configuration
	cmd = exec.Command("nmcli", "connection", "modify", i, "ipv6.method", "auto")
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println("Failed to enable IPv6:", string(out), err)
	}

	// Enable autoconnect
	cmd = exec.Command("nmcli", "connection", "modify", i, "connection.autoconnect", "yes")
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println("Failed to enable autoconnect:", string(out), err)
	}

	// Bring up the connection
	cmd = exec.Command("nmcli", "connection", "up", i)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println("Failed to bring up connection:", string(out), err)
	}

	waitingDots()

	return GetInterfaceNetworkStatus(i)
}

// Helper function to ensure connection exists
func ensureConnectionExists(i string) {
	cmd := exec.Command("nmcli", "-t", "-f", "NAME", "connection", "show", i)
	out, err := cmd.CombinedOutput()

	if err != nil || strings.TrimSpace(string(out)) == "" {
		cmd = exec.Command("nmcli", "connection", "add", "type", "ethernet", "ifname", i, "con-name", i)
		out, err = cmd.CombinedOutput()
		if err != nil {
			log.Println("Failed to create connection:", string(out), err)
		}
	}
}

// Enhanced status function that shows IPv4/IPv6 status
func GetInterfaceNetworkStatus(i string) string {
	// Check if connection exists
	cmd := exec.Command("nmcli", "-t", "-f", "NAME", "connection", "show", i)
	out, err := cmd.CombinedOutput()
	if err != nil || strings.TrimSpace(string(out)) == "" {
		return "inactive (no connection)"
	}

	// Get IPv4 and IPv6 methods
	cmd = exec.Command("nmcli", "-t", "-f", "ipv4.method,ipv6.method,connection.autoconnect", "connection", "show", i)
	out, err = cmd.CombinedOutput()
	if err != nil {
		return "inactive (cannot read connection)"
	}

	lines := strings.Split(strings.TrimSpace(string(out)), "\n")
	var ipv4Method, ipv6Method, autoconnect string

	for _, line := range lines {
		if strings.HasPrefix(line, "ipv4.method:") {
			ipv4Method = strings.TrimPrefix(line, "ipv4.method:")
		} else if strings.HasPrefix(line, "ipv6.method:") {
			ipv6Method = strings.TrimPrefix(line, "ipv6.method:")
		} else if strings.HasPrefix(line, "connection.autoconnect:") {
			autoconnect = strings.TrimPrefix(line, "connection.autoconnect:")
		}
	}

	// Check device state
	cmd = exec.Command("nmcli", "-t", "-f", "DEVICE,STATE", "device", "status")
	out, err = cmd.CombinedOutput()
	deviceState := "unknown"

	if err == nil {
		lines := strings.Split(string(out), "\n")
		for _, line := range lines {
			parts := strings.Split(line, ":")
			if len(parts) >= 2 && parts[0] == i {
				deviceState = parts[1]
				break
			}
		}
	}

	// Determine overall status
	if ipv4Method == "disabled" && ipv6Method == "disabled" {
		return "disabled (IPv4: disabled, IPv6: disabled)"
	}

	if autoconnect == "no" {
		return fmt.Sprintf("inactive (IPv4: %s, IPv6: %s, autoconnect: no)", ipv4Method, ipv6Method)
	}

	if deviceState == "connected" {
		return fmt.Sprintf("active (IPv4: %s, IPv6: %s, device: %s)", ipv4Method, ipv6Method, deviceState)
	}

	return fmt.Sprintf("inactive (IPv4: %s, IPv6: %s, device: %s)", ipv4Method, ipv6Method, deviceState)
}

// Additional utility functions for more granular control

// DisableIPv4Only - Disables only IPv4, leaves IPv6 enabled
func DisableIPv4Only(i string) string {
	ensureConnectionExists(i)
	cmd := exec.Command("nmcli", "connection", "modify", i, "ipv4.method", "disabled")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println("Failed to disable IPv4:", string(out), err)
	}

	// Restart connection to apply changes
	exec.Command("nmcli", "connection", "down", i).Run()
	exec.Command("nmcli", "connection", "up", i).Run()

	return GetInterfaceNetworkStatus(i)
}

// DisableIPv6Only - Disables only IPv6, leaves IPv4 enabled
func DisableIPv6Only(i string) string {
	ensureConnectionExists(i)
	cmd := exec.Command("nmcli", "connection", "modify", i, "ipv6.method", "disabled")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println("Failed to disable IPv6:", string(out), err)
	}

	// Restart connection to apply changes
	exec.Command("nmcli", "connection", "down", i).Run()
	exec.Command("nmcli", "connection", "up", i).Run()

	return GetInterfaceNetworkStatus(i)
}

func GetInterfaces() string {
	cmd := exec.Command("nmcli", "device")
	out, err := cmd.CombinedOutput()
	if err != nil {
		return "err"
	}
	return string(out)
}

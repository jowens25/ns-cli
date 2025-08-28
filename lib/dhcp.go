package lib

import (
	"fmt"
	"log"
	"os/exec"
	"strings"
)

func GetIpv4DhcpState(i string) string {
	connection := GetConnectionNameFromDevice(i)

	cmd := exec.Command("nmcli", "-f", "ipv4.method", "con", "show", connection)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal(err.Error())
	}

	fields := strings.Fields(string(out))

	if len(fields) == 2 {
		return fields[1]
	}

	return "dhcp state unknown"
}

// Set DHCP enabled/disabled for IPv4
func EnableDhcp4(i string) error {

	if !HasInterface(i) {
		return fmt.Errorf("interface %q not found", i)
	}

	connection := GetConnectionNameFromDevice(i)

	if connection == "interface not found" {
		return nil
	}

	// Enable DHCP
	cmd := exec.Command("nmcli", "connection", "modify", connection, "ipv4.method", "auto")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to enable DHCP: %s %v", string(out), err)
		return err
	}

	// Clear static settings
	cmd = exec.Command("nmcli", "connection", "modify", connection, "ipv4.addresses", "")
	cmd.CombinedOutput()
	cmd = exec.Command("nmcli", "connection", "modify", connection, "ipv4.gateway", "")
	cmd.CombinedOutput()

	// Apply changes
	cmd = exec.Command("nmcli", "connection", "up", connection)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to bring up connection: %s %v", string(out), err)
		return err
	}

	return nil
}

// Set DHCP enabled/disabled for IPv4
func DisableDhcp4(i string) error {

	if !HasInterface(i) {
		return fmt.Errorf("interface %q not found", i)
	}

	currentIp := GetIpv4Address(i)
	currentGate := GetIpv4Gateway(i)

	SetIpv4Address(i, currentIp)
	SetIpv4Gateway(i, currentGate)

	connection := GetConnectionNameFromDevice(i)

	// Apply changes
	cmd := exec.Command("nmcli", "connection", "up", connection)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to bring up connection: %s %v", string(out), err)
		return err
	}

	return nil
}

// Set DHCP enabled/disabled for IPv4
func RestartDhcp4() error {

	// Disable DHCP (set to manual)
	cmd := exec.Command("systemctl", "restart", "NetworkManager")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to restart DHCP: %s %v", string(out), err)
		return err
	}

	return nil
}

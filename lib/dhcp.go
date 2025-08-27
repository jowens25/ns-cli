package lib

import (
	"fmt"
	"log"
	"os/exec"
)

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

	connection := GetConnectionNameFromDevice(i)

	// Disable DHCP (set to manual)
	cmd := exec.Command("nmcli", "connection", "modify", connection, "ipv4.method", "manual")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to disable DHCP: %s %v", string(out), err)
		return err
	}

	// Apply changes
	cmd = exec.Command("nmcli", "connection", "up", connection)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to bring up connection: %s %v", string(out), err)
		return err
	}

	return nil
}

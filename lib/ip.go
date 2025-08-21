package lib

import (
	"bufio"
	"bytes"
	"log"
	"os/exec"
	"strings"
)

func GetIPv4Address(i string) string {
	cmd := exec.Command("ip", "a", "show", "dev", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal("ip a err")
	}

	if strings.Contains(string(out), i) {
		scanner := bufio.NewScanner(bytes.NewReader(out))

		for scanner.Scan() {
			line := scanner.Text()
			if strings.Contains(strings.TrimSpace(line), "inet") && !strings.Contains(line, "inet6") {
				fields := strings.Fields(line)
				if len(fields) > 1 {
					return fields[1]
				}
			}
		}
	}
	return "ipv4 error"
}

func GetIPv6Address(i string) string {
	cmd := exec.Command("ip", "a", "show", "dev", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal("ip a err")
	}

	if strings.Contains(string(out), i) {
		scanner := bufio.NewScanner(bytes.NewReader(out))

		for scanner.Scan() {
			line := scanner.Text()
			if strings.Contains(line, "inet6") && !strings.Contains(line, "fe80") { // Skip link-local
				fields := strings.Fields(line)
				if len(fields) > 1 {
					return fields[1]
				}
			}
		}
	}
	return "ipv6 error"
}

// Set static IPv4 address using nmcli
func SetStaticIPv4(interfaceName, ipAddress, gateway, dns string) error {
	// Check if connection exists, create if it doesn't
	cmd := exec.Command("nmcli", "-t", "-f", "NAME", "connection", "show", interfaceName)
	out, err := cmd.CombinedOutput()

	if err != nil || strings.TrimSpace(string(out)) == "" {
		// Connection doesn't exist, create it
		cmd = exec.Command("nmcli", "connection", "add", "type", "ethernet", "ifname", interfaceName, "con-name", interfaceName)
		out, err = cmd.CombinedOutput()
		if err != nil {
			log.Printf("Failed to create connection: %s %v", string(out), err)
			return err
		}
	}

	// Set static IP configuration
	cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv4.method", "manual")
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to set manual method: %s %v", string(out), err)
		return err
	}

	// Set IP address
	cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv4.addresses", ipAddress)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to set IP address: %s %v", string(out), err)
		return err
	}

	// Set gateway if provided
	if gateway != "" {
		cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv4.gateway", gateway)
		out, err = cmd.CombinedOutput()
		if err != nil {
			log.Printf("Failed to set gateway: %s %v", string(out), err)
			return err
		}
	}

	// Set DNS if provided
	if dns != "" {
		cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv4.dns", dns)
		out, err = cmd.CombinedOutput()
		if err != nil {
			log.Printf("Failed to set DNS: %s %v", string(out), err)
			return err
		}
	}

	// Bring up the connection to apply changes
	cmd = exec.Command("nmcli", "connection", "up", interfaceName)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to bring up connection: %s %v", string(out), err)
		return err
	}

	return nil
}

// Set static IPv6 address using nmcli
func SetStaticIPv6(interfaceName, ipv6Address, gateway, dns string) error {
	// Check if connection exists, create if it doesn't
	cmd := exec.Command("nmcli", "-t", "-f", "NAME", "connection", "show", interfaceName)
	out, err := cmd.CombinedOutput()

	if err != nil || strings.TrimSpace(string(out)) == "" {
		// Connection doesn't exist, create it
		cmd = exec.Command("nmcli", "connection", "add", "type", "ethernet", "ifname", interfaceName, "con-name", interfaceName)
		out, err = cmd.CombinedOutput()
		if err != nil {
			log.Printf("Failed to create connection: %s %v", string(out), err)
			return err
		}
	}

	// Set static IPv6 configuration
	cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv6.method", "manual")
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to set IPv6 manual method: %s %v", string(out), err)
		return err
	}

	// Set IPv6 address
	cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv6.addresses", ipv6Address)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to set IPv6 address: %s %v", string(out), err)
		return err
	}

	// Set IPv6 gateway if provided
	if gateway != "" {
		cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv6.gateway", gateway)
		out, err = cmd.CombinedOutput()
		if err != nil {
			log.Printf("Failed to set IPv6 gateway: %s %v", string(out), err)
			return err
		}
	}

	// Set IPv6 DNS if provided
	if dns != "" {
		cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv6.dns", dns)
		out, err = cmd.CombinedOutput()
		if err != nil {
			log.Printf("Failed to set IPv6 DNS: %s %v", string(out), err)
			return err
		}
	}

	// Bring up the connection to apply changes
	cmd = exec.Command("nmcli", "connection", "up", interfaceName)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to bring up connection: %s %v", string(out), err)
		return err
	}

	return nil
}

// Set DHCP for IPv4
func SetDHCPv4(interfaceName string) error {
	// Check if connection exists, create if it doesn't
	cmd := exec.Command("nmcli", "-t", "-f", "NAME", "connection", "show", interfaceName)
	out, err := cmd.CombinedOutput()

	if err != nil || strings.TrimSpace(string(out)) == "" {
		// Connection doesn't exist, create it
		cmd = exec.Command("nmcli", "connection", "add", "type", "ethernet", "ifname", interfaceName, "con-name", interfaceName)
		out, err = cmd.CombinedOutput()
		if err != nil {
			log.Printf("Failed to create connection: %s %v", string(out), err)
			return err
		}
	}

	// Set DHCP for IPv4
	cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv4.method", "auto")
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to set DHCP: %s %v", string(out), err)
		return err
	}

	// Clear any static addresses
	cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv4.addresses", "")
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to clear static addresses: %s %v", string(out), err)
		return err
	}

	// Clear gateway
	cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv4.gateway", "")
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to clear gateway: %s %v", string(out), err)
		return err
	}

	// Clear DNS
	cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv4.dns", "")
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to clear DNS: %s %v", string(out), err)
		return err
	}

	// Bring up the connection to apply changes
	cmd = exec.Command("nmcli", "connection", "up", interfaceName)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to bring up connection: %s %v", string(out), err)
		return err
	}

	return nil
}

// Get current IP configuration method
func GetIPMethod(interfaceName string) (ipv4Method, ipv6Method string) {
	// Get IPv4 method
	cmd := exec.Command("nmcli", "-t", "-f", "ipv4.method", "connection", "show", interfaceName)
	out, err := cmd.CombinedOutput()
	if err != nil {
		ipv4Method = "unknown"
	} else {
		ipv4Method = strings.TrimSpace(string(out))
	}

	// Get IPv6 method
	cmd = exec.Command("nmcli", "-t", "-f", "ipv6.method", "connection", "show", interfaceName)
	out, err = cmd.CombinedOutput()
	if err != nil {
		ipv6Method = "unknown"
	} else {
		ipv6Method = strings.TrimSpace(string(out))
	}

	return ipv4Method, ipv6Method
}

// Get DHCP enabled status for IPv4
func GetDHCPEnabled(interfaceName string) bool {
	cmd := exec.Command("nmcli", "-t", "-f", "ipv4.method", "connection", "show", interfaceName)
	out, err := cmd.CombinedOutput()
	if err != nil {
		return false
	}
	return strings.TrimSpace(string(out)) == "auto"
}

// Set DHCP enabled/disabled for IPv4
func SetDHCPEnabled(interfaceName string, enabled bool) error {
	// Check if connection exists, create if it doesn't
	cmd := exec.Command("nmcli", "-t", "-f", "NAME", "connection", "show", interfaceName)
	out, err := cmd.CombinedOutput()

	if err != nil || strings.TrimSpace(string(out)) == "" {
		// Connection doesn't exist, create it
		cmd = exec.Command("nmcli", "connection", "add", "type", "ethernet", "ifname", interfaceName, "con-name", interfaceName)
		out, err = cmd.CombinedOutput()
		if err != nil {
			log.Printf("Failed to create connection: %s %v", string(out), err)
			return err
		}
	}

	if enabled {
		// Enable DHCP
		cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv4.method", "auto")
		out, err = cmd.CombinedOutput()
		if err != nil {
			log.Printf("Failed to enable DHCP: %s %v", string(out), err)
			return err
		}

		// Clear static settings
		cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv4.addresses", "")
		cmd.CombinedOutput()
		cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv4.gateway", "")
		cmd.CombinedOutput()
	} else {
		// Disable DHCP (set to manual)
		cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv4.method", "manual")
		out, err = cmd.CombinedOutput()
		if err != nil {
			log.Printf("Failed to disable DHCP: %s %v", string(out), err)
			return err
		}
	}

	// Apply changes
	cmd = exec.Command("nmcli", "connection", "up", interfaceName)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to bring up connection: %s %v", string(out), err)
		return err
	}

	return nil
}

// Get domain/search domains
func GetDomain(interfaceName string) []string {
	cmd := exec.Command("nmcli", "-t", "-f", "ipv4.dns-search", "connection", "show", interfaceName)
	out, err := cmd.CombinedOutput()
	if err != nil {
		return []string{}
	}

	domainStr := strings.TrimSpace(string(out))
	if domainStr == "" || domainStr == "--" {
		return []string{}
	}

	// Split by comma and clean up
	domains := strings.Split(domainStr, ",")
	var cleanDomains []string
	for _, domain := range domains {
		domain = strings.TrimSpace(domain)
		if domain != "" {
			cleanDomains = append(cleanDomains, domain)
		}
	}
	return cleanDomains
}

// Set domain/search domains
func SetDomain(interfaceName string, domains []string) error {
	// Check if connection exists, create if it doesn't
	cmd := exec.Command("nmcli", "-t", "-f", "NAME", "connection", "show", interfaceName)
	out, err := cmd.CombinedOutput()

	if err != nil || strings.TrimSpace(string(out)) == "" {
		// Connection doesn't exist, create it
		cmd = exec.Command("nmcli", "connection", "add", "type", "ethernet", "ifname", interfaceName, "con-name", interfaceName)
		out, err = cmd.CombinedOutput()
		if err != nil {
			log.Printf("Failed to create connection: %s %v", string(out), err)
			return err
		}
	}

	// Join domains with comma
	domainStr := strings.Join(domains, ",")

	cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv4.dns-search", domainStr)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to set domain: %s %v", string(out), err)
		return err
	}

	// Apply changes
	cmd = exec.Command("nmcli", "connection", "up", interfaceName)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to bring up connection: %s %v", string(out), err)
		return err
	}

	return nil
}

// Get primary and secondary DNS servers
func GetDNSServers(interfaceName string) (primary, secondary string) {
	cmd := exec.Command("nmcli", "-t", "-f", "ipv4.dns", "connection", "show", interfaceName)
	out, err := cmd.CombinedOutput()
	if err != nil {
		return "", ""
	}

	dnsStr := strings.TrimSpace(string(out))
	if dnsStr == "" || dnsStr == "--" {
		return "", ""
	}

	// Split by comma or space
	dnsServers := strings.FieldsFunc(dnsStr, func(c rune) bool {
		return c == ',' || c == ' '
	})

	// Clean up and assign
	var cleanServers []string
	for _, server := range dnsServers {
		server = strings.TrimSpace(server)
		if server != "" {
			cleanServers = append(cleanServers, server)
		}
	}

	if len(cleanServers) >= 1 {
		primary = cleanServers[0]
	}
	if len(cleanServers) >= 2 {
		secondary = cleanServers[1]
	}

	return primary, secondary
}

// Set primary and secondary DNS servers
func SetDNSServers(interfaceName, primary, secondary string) error {
	// Check if connection exists, create if it doesn't
	cmd := exec.Command("nmcli", "-t", "-f", "NAME", "connection", "show", interfaceName)
	out, err := cmd.CombinedOutput()

	if err != nil || strings.TrimSpace(string(out)) == "" {
		// Connection doesn't exist, create it
		cmd = exec.Command("nmcli", "connection", "add", "type", "ethernet", "ifname", interfaceName, "con-name", interfaceName)
		out, err = cmd.CombinedOutput()
		if err != nil {
			log.Printf("Failed to create connection: %s %v", string(out), err)
			return err
		}
	}

	// Build DNS string
	var dnsStr string
	if primary != "" && secondary != "" {
		dnsStr = primary + "," + secondary
	} else if primary != "" {
		dnsStr = primary
	} else {
		dnsStr = "" // Clear DNS
	}

	cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv4.dns", dnsStr)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to set DNS servers: %s %v", string(out), err)
		return err
	}

	// Apply changes
	cmd = exec.Command("nmcli", "connection", "up", interfaceName)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to bring up connection: %s %v", string(out), err)
		return err
	}

	return nil
}

// Set only primary DNS server
func SetPrimaryDNS(interfaceName, primaryDNS string) error {
	// Get current secondary DNS to preserve it
	_, secondary := GetDNSServers(interfaceName)
	return SetDNSServers(interfaceName, primaryDNS, secondary)
}

// Set only secondary DNS server
func SetSecondaryDNS(interfaceName, secondaryDNS string) error {
	// Get current primary DNS to preserve it
	primary, _ := GetDNSServers(interfaceName)
	return SetDNSServers(interfaceName, primary, secondaryDNS)
}

// Get primary DNS server only
func GetPrimaryDNS(interfaceName string) string {
	primary, _ := GetDNSServers(interfaceName)
	return primary
}

// Get secondary DNS server only
func GetSecondaryDNS(interfaceName string) string {
	_, secondary := GetDNSServers(interfaceName)
	return secondary
}

// Get gateway
func GetGateway(interfaceName string) string {
	cmd := exec.Command("nmcli", "-t", "-f", "ipv4.gateway", "connection", "show", interfaceName)
	out, err := cmd.CombinedOutput()
	if err != nil {
		return ""
	}

	gateway := strings.TrimSpace(string(out))
	if gateway == "" || gateway == "--" {
		return ""
	}

	return gateway
}

// Set gateway
func SetGateway(interfaceName, gateway string) error {
	// Check if connection exists, create if it doesn't
	cmd := exec.Command("nmcli", "-t", "-f", "NAME", "connection", "show", interfaceName)
	out, err := cmd.CombinedOutput()

	if err != nil || strings.TrimSpace(string(out)) == "" {
		// Connection doesn't exist, create it
		cmd = exec.Command("nmcli", "connection", "add", "type", "ethernet", "ifname", interfaceName, "con-name", interfaceName)
		out, err = cmd.CombinedOutput()
		if err != nil {
			log.Printf("Failed to create connection: %s %v", string(out), err)
			return err
		}
	}

	cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv4.gateway", gateway)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to set gateway: %s %v", string(out), err)
		return err
	}

	// Apply changes
	cmd = exec.Command("nmcli", "connection", "up", interfaceName)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Printf("Failed to bring up connection: %s %v", string(out), err)
		return err
	}

	return nil
}

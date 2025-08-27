package lib

import (
	"bufio"
	"bytes"
	"fmt"
	"log"
	"net"
	"os"
	"os/exec"
	"sort"
	"strconv"
	"strings"
)

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

func GetIPv6Netmask(iface string) string {
	cmd := exec.Command("ip", "a", "show", "dev", iface)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatalf("failed to run ip command: %v", err)
	}

	scanner := bufio.NewScanner(bytes.NewReader(out))
	for scanner.Scan() {
		line := scanner.Text()
		if strings.Contains(line, "inet6") {
			fields := strings.Fields(line)
			if len(fields) > 1 {
				_, ipnet, err := net.ParseCIDR(fields[1])
				if err != nil {
					return "parse error"
				}
				mask := ipnet.Mask
				return net.IP(mask).String()
			}
		}
	}
	return "ipv4 error"
}

// Set static IPv4 address using nmcli
func SetStaticIPv4(interfaceName, ipAddress, netmask, gateway string, dns ...string) error {

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
	if len(dns) != 0 {
		dnsString := strings.Join(dns, " ")
		cmd = exec.Command("nmcli", "connection", "modify", interfaceName, "ipv4.dns", dnsString)
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
		return "err", "1"
	}

	dnsStr := strings.TrimSpace(string(out))
	if dnsStr == "" || dnsStr == "--" {
		return "none", "none"
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
func GetIpGateway(interfaceName string) string {
	cmd := exec.Command("nmcli", "dev", "show", interfaceName)
	out, err := cmd.CombinedOutput()
	if err != nil {
		return string(out)
	}

	lines := strings.SplitSeq(string(out), "\n")

	for line := range lines {

		if strings.Contains(line, "IP4.GATEWAY:") {
			fields := strings.Fields(line)
			return fields[1]
		}
	}
	return "not found"

}

func GetIp6Gateway(interfaceName string) string {
	cmd := exec.Command("nmcli", "dev", "show", interfaceName)
	out, err := cmd.CombinedOutput()
	if err != nil {
		return string(out)
	}

	lines := strings.SplitSeq(string(out), "\n")

	for line := range lines {

		if strings.Contains(line, "IP6.GATEWAY:") {
			fields := strings.Fields(line)
			return fields[1]
		}
	}
	return "not found"

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

type Route struct {
	Destination string
	Gateway     string
	Interface   string
	Metric      int
}

// getNetworkInterfaces returns a list of all network interfaces
func getNetworkInterfaces() ([]string, error) {
	interfaces, err := net.Interfaces()
	if err != nil {
		return nil, err
	}

	var ifaceNames []string
	for _, iface := range interfaces {
		// Skip loopback interfaces
		if iface.Flags&net.FlagLoopback == 0 {
			ifaceNames = append(ifaceNames, iface.Name)
		}
	}
	return ifaceNames, nil
}

// isInterfaceActive checks if an interface has an IP address assigned
func isInterfaceActive(ifaceName string) bool {
	iface, err := net.InterfaceByName(ifaceName)
	if err != nil {
		return false
	}

	addrs, err := iface.Addrs()
	if err != nil {
		return false
	}

	// Check if interface has any non-loopback addresses
	for _, addr := range addrs {
		if ipnet, ok := addr.(*net.IPNet); ok && !ipnet.IP.IsLoopback() {
			if ipnet.IP.To4() != nil { // IPv4 address
				return true
			}
		}
	}
	return false
}

// parseRoutingTable reads and parses /proc/net/route
func parseRoutingTable() (map[string][]Route, error) {
	file, err := os.Open("/proc/net/route")
	if err != nil {
		return nil, err
	}
	defer file.Close()

	routes := make(map[string][]Route)
	scanner := bufio.NewScanner(file)

	// Skip header line
	scanner.Scan()

	for scanner.Scan() {
		fields := strings.Fields(scanner.Text())
		if len(fields) < 8 {
			continue
		}

		iface := fields[0]
		destHex := fields[1]
		gwHex := fields[2]
		metricStr := fields[6]

		// Convert hex destination to IP
		dest, err := hexToIP(destHex)
		if err != nil {
			continue
		}

		// Convert hex gateway to IP
		gateway, err := hexToIP(gwHex)
		if err != nil {
			continue
		}

		// Parse metric
		metric, err := strconv.Atoi(metricStr)
		if err != nil {
			metric = 0
		}

		// Determine CIDR notation for destination
		var destCIDR string
		if dest == "0.0.0.0" {
			destCIDR = "0.0.0.0/0" // Default route
		} else {
			// For simplicity, assume /32 for host routes and /24 for network routes
			// In a real implementation, you'd parse the netmask from field[7]
			if strings.HasSuffix(dest, ".0") {
				destCIDR = dest + "/24"
			} else {
				destCIDR = dest + "/32"
			}
		}

		route := Route{
			Destination: destCIDR,
			Gateway:     gateway,
			Interface:   iface,
			Metric:      metric,
		}

		routes[iface] = append(routes[iface], route)
	}

	return routes, scanner.Err()
}

// hexToIP converts a hex string to IP address string
func hexToIP(hexStr string) (string, error) {
	if len(hexStr) != 8 {
		return "", fmt.Errorf("invalid hex string length")
	}

	// Convert hex to bytes (little-endian format in /proc/net/route)
	var bytes []byte
	for i := len(hexStr); i > 0; i -= 2 {
		b, err := strconv.ParseUint(hexStr[i-2:i], 16, 8)
		if err != nil {
			return "", err
		}
		bytes = append(bytes, byte(b))
	}

	ip := net.IP(bytes)
	return ip.String(), nil
}

// displayRoutingTable formats and displays the routing table like routes4 command
func displayRoutingTable() error {
	// Get all network interfaces
	interfaces, err := getNetworkInterfaces()
	if err != nil {
		return fmt.Errorf("failed to get network interfaces: %v", err)
	}

	// Parse routing table
	routes, err := parseRoutingTable()
	if err != nil {
		return fmt.Errorf("failed to parse routing table: %v", err)
	}

	// Sort interfaces for consistent output
	sort.Strings(interfaces)

	// Display routing table for each interface
	for _, iface := range interfaces {
		fmt.Printf("%s routing table:\n", iface)

		if !isInterfaceActive(iface) {
			fmt.Println("Interface not activated")
			fmt.Println()
			continue
		}

		ifaceRoutes, exists := routes[iface]
		if !exists || len(ifaceRoutes) == 0 {
			fmt.Println("No routes found")
			fmt.Println()
			continue
		}

		// Display routes for this interface
		for i, route := range ifaceRoutes {
			fmt.Printf("%d: %s via %s dev %s metric %d\n",
				i, route.Destination, route.Gateway, route.Interface, route.Metric)
		}
		fmt.Println()
	}

	return nil
}

// Routes4 is the main function that mimics the routes4 command
func Routes4() {
	if err := displayRoutingTable(); err != nil {
		fmt.Fprintf(os.Stderr, "Error: %v\n", err)
		os.Exit(1)
	}
}

type Route6 struct {
	Destination string
	Gateway     string
	Interface   string
	Metric      int
}

// isInterfaceActiveIPv6 checks if an interface has an IPv6 address assigned
func isInterfaceActiveIPv6(ifaceName string) bool {
	iface, err := net.InterfaceByName(ifaceName)
	if err != nil {
		return false
	}

	addrs, err := iface.Addrs()
	if err != nil {
		return false
	}

	// Check if interface has any IPv6 addresses (excluding link-local)
	for _, addr := range addrs {
		if ipnet, ok := addr.(*net.IPNet); ok {
			if ipnet.IP.To16() != nil && ipnet.IP.To4() == nil {
				// Skip link-local addresses (fe80::/10)
				if !ipnet.IP.IsLinkLocalUnicast() {
					return true
				}
			}
		}
	}
	return false
}

// getInterfaceByIndex returns interface name by index
func getInterfaceByIndex(index int) string {
	iface, err := net.InterfaceByIndex(index)
	if err != nil {
		return fmt.Sprintf("if%d", index)
	}
	return iface.Name
}

// parseIPv6RoutingTable reads and parses /proc/net/ipv6_route
func parseIPv6RoutingTable() (map[string][]Route6, error) {
	file, err := os.Open("/proc/net/ipv6_route")
	if err != nil {
		return nil, err
	}
	defer file.Close()

	routes := make(map[string][]Route6)
	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		fields := strings.Fields(scanner.Text())
		if len(fields) < 10 {
			continue
		}

		destHex := fields[0]
		destPrefixLen := fields[1]
		//srcHex := fields[2]
		//srcPrefixLen := fields[3]
		nextHopHex := fields[4]
		metricHex := fields[5]
		//refCnt := fields[6]
		//useCnt := fields[7]
		flags := fields[8]
		ifaceIndex := fields[9]

		// Skip routes with certain flags (e.g., cached routes)
		if strings.Contains(flags, "00000001") { // RTF_UP flag should be set
			// Convert interface index to name
			ifaceIdx, err := strconv.Atoi(ifaceIndex)
			if err != nil {
				continue
			}
			ifaceName := getInterfaceByIndex(ifaceIdx)

			// Convert hex destination to IPv6
			dest, err := hexToIPv6(destHex)
			if err != nil {
				continue
			}

			// Convert prefix length
			prefixLen, err := strconv.Atoi(destPrefixLen)
			if err != nil {
				prefixLen = 128
			}

			// Convert hex next hop to IPv6
			nextHop, err := hexToIPv6(nextHopHex)
			if err != nil {
				continue
			}

			// Convert metric from hex
			metric, err := strconv.ParseInt(metricHex, 16, 32)
			if err != nil {
				metric = 0
			}

			// Format destination with prefix
			var destCIDR string
			if dest == "::" && prefixLen == 0 {
				destCIDR = "::/0" // Default route
			} else {
				destCIDR = fmt.Sprintf("%s/%d", dest, prefixLen)
			}

			// Format gateway (next hop)
			gateway := nextHop
			if gateway == "::" {
				gateway = "::" // On-link route
			}

			route := Route6{
				Destination: destCIDR,
				Gateway:     gateway,
				Interface:   ifaceName,
				Metric:      int(metric),
			}

			routes[ifaceName] = append(routes[ifaceName], route)
		}
	}

	return routes, scanner.Err()
}

// hexToIPv6 converts a 32-character hex string to IPv6 address
func hexToIPv6(hexStr string) (string, error) {
	if len(hexStr) != 32 {
		return "", fmt.Errorf("invalid hex string length: expected 32, got %d", len(hexStr))
	}

	// Convert hex string to bytes
	var bytes []byte
	for i := 0; i < len(hexStr); i += 2 {
		b, err := strconv.ParseUint(hexStr[i:i+2], 16, 8)
		if err != nil {
			return "", err
		}
		bytes = append(bytes, byte(b))
	}

	ip := net.IP(bytes)
	return ip.String(), nil
}

// displayIPv6RoutingTable formats and displays the IPv6 routing table
func displayIPv6RoutingTable() error {
	// Get all network interfaces
	interfaces, err := getNetworkInterfaces()
	if err != nil {
		return fmt.Errorf("failed to get network interfaces: %v", err)
	}

	// Parse IPv6 routing table
	routes, err := parseIPv6RoutingTable()
	if err != nil {
		return fmt.Errorf("failed to parse IPv6 routing table: %v", err)
	}

	// Sort interfaces for consistent output
	sort.Strings(interfaces)

	// Display routing table for each interface
	for _, iface := range interfaces {
		fmt.Printf("%s IPv6 routing table:\n", iface)

		if !isInterfaceActiveIPv6(iface) {
			fmt.Println("Interface not activated")
			fmt.Println()
			continue
		}

		ifaceRoutes, exists := routes[iface]
		if !exists || len(ifaceRoutes) == 0 {
			fmt.Println("No IPv6 routes found")
			fmt.Println()
			continue
		}

		// Display routes for this interface
		for i, route := range ifaceRoutes {
			fmt.Printf("%d: %s via %s dev %s metric %d\n",
				i, route.Destination, route.Gateway, route.Interface, route.Metric)
		}
		fmt.Println()
	}

	return nil
}

// Routes6 is the main function that displays IPv6 routing tables
func Routes6() {
	if err := displayIPv6RoutingTable(); err != nil {
		fmt.Fprintf(os.Stderr, "Error: %v\n", err)
		os.Exit(1)
	}
}

// Combined function to show both IPv4 and IPv6 routes
func RoutesAll() {
	fmt.Println("=== IPv4 Routing Tables ===")
	if err := displayRoutingTable(); err != nil {
		fmt.Fprintf(os.Stderr, "IPv4 Error: %v\n", err)
	}

	fmt.Println("\n=== IPv6 Routing Tables ===")
	if err := displayIPv6RoutingTable(); err != nil {
		fmt.Fprintf(os.Stderr, "IPv6 Error: %v\n", err)
	}
}

func main() {
	if len(os.Args) > 1 && os.Args[1] == "all" {
		RoutesAll()
	} else {
		Routes6()
	}
}

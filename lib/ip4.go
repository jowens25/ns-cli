package lib

import (
	"fmt"
	"log"
	"net"
	"os/exec"
	"strings"
)

func GetIpv4Address(i string) string {
	connection := GetConnectionNameFromDevice(i)
	cmd := exec.Command("nmcli", "-f", "ipv4.addresses,IP4.ADDRESS", "con", "show", connection)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal(err.Error())
	}

	for line := range strings.SplitSeq(string(out), "\n") {
		fields := strings.Split(line, ":")

		if len(fields) == 2 {
			ip, _, err := net.ParseCIDR(strings.TrimSpace(fields[1]))
			if err != nil {
				fmt.Printf("Error parsing CIDR: %v\n", err)
				continue
			}
			return ip.String()

		}
	}

	return "ipv4 address not available"
}

func GetIpv4Netmask(i string) string {
	connection := GetConnectionNameFromDevice(i)
	cmd := exec.Command("nmcli", "-f", "ipv4.addresses,IP4.ADDRESS", "con", "show", connection)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal(err.Error())
	}

	for line := range strings.SplitSeq(string(out), "\n") {
		fields := strings.Split(line, ":")

		if len(fields) == 2 {
			_, ipnet, err := net.ParseCIDR(strings.TrimSpace(fields[1]))
			if err != nil {
				fmt.Printf("Error parsing CIDR: %v\n", err)
				continue
			}

			return net.IP(ipnet.Mask).String()

		}
	}

	return "ipv4 address not available"
}

func GetIpv4Gateway(i string) string {
	cmd := exec.Command("nmcli", "dev", "show", i)
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

func GetIpv4MacAddress(i string) string {
	cmd := exec.Command("nmcli", "-f", "GENERAL.HWADDR", "dev", "show", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal(err.Error())
	}

	fields := strings.Fields(string(out))

	if len(fields) == 2 {
		mac, err := net.ParseMAC(fields[1])
		if err != nil {
			fmt.Printf("Error parsing mac: %v\n", err)
		}
		return mac.String()
	}

	return "mac address not available"
}

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

	return "eth0"
}

func GetIpv4Dns1(i string) string {
	cmd := exec.Command("nmcli", "-f", "IP4.DNS", "dev", "show", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal(err.Error())
	}

	lines := strings.SplitSeq(string(out), "\n")

	for line := range lines {

		if strings.Contains(line, "IP4.DNS[1]:") {

			fields := strings.Fields(line)

			if len(fields) == 2 {

				return strings.TrimSpace(fields[1])
			}
		}

	}

	return "dns 1 not found"
}

func GetIpv4Dns2(i string) string {
	cmd := exec.Command("nmcli", "-f", "IP4.DNS", "dev", "show", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal(err.Error())
	}

	lines := strings.SplitSeq(string(out), "\n")

	for line := range lines {

		if strings.Contains(line, "IP4.DNS[2]") {

			fields := strings.Fields(line)

			if len(fields) == 2 {

				return strings.TrimSpace(fields[1])
			}
		}

	}

	return "dns 2 not found"
}

func SetIpv4Dns(i string, dns ...string) {
	connection := GetConnectionNameFromDevice(i)

	if len(dns) == 0 {
		log.Fatal("no DNS servers provided")
	}

	// Join multiple DNS addresses into a comma-separated string
	dnsArg := dns[0]
	if len(dns) > 1 {
		dnsArg = dns[0] + "," + dns[1]
	}

	// Modify DNS
	cmd := exec.Command("nmcli", "con", "modify", connection, "ipv4.dns", dnsArg)
	if out, err := cmd.CombinedOutput(); err != nil {
		log.Fatalf("failed to set dns: %v\n%s", err, string(out))
	}

	// Bring connection up
	cmd = exec.Command("nmcli", "con", "up", connection)
	if out, err := cmd.CombinedOutput(); err != nil {
		log.Fatalf("failed to bring up connection: %v\n%s", err, string(out))
	}
}

func SetIpv4Address(i string, address string) {

	connection := GetConnectionNameFromDevice(i)

	ip1 := net.ParseIP(address)
	if ip1 != nil {
		cmd := exec.Command("nmcli", "con", "modify", connection,
			"ipv4.addresses", ip1.String(),
			"ipv4.method", "manual")
		if out, err := cmd.CombinedOutput(); err != nil {
			log.Fatalf("failed to set ipv4 address: %v\n%s", err, string(out))
		}

		// Bring connection up
		cmd = exec.Command("nmcli", "con", "up", connection)
		if out, err := cmd.CombinedOutput(); err != nil {
			log.Fatalf("failed to bring up connection: %v\n%s", err, string(out))
		}
	}

}

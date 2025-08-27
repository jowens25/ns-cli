package lib

import (
	"fmt"
	"log"
	"net"
	"os/exec"
	"strings"
)

func GetIpv6Address(i string) string {
	cmd := exec.Command("nmcli", "-f", "IP6.ADDRESS", "dev", "show", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal(err.Error())
	}

	fields := strings.Fields(string(out))

	if len(fields) == 2 {
		ip, _, err := net.ParseCIDR(fields[1])
		if err != nil {
			fmt.Printf("Error parsing CIDR: %v\n", err)
		}
		return ip.String()
	}

	return "ipv6 address not available"
}

func GetIpv6Gateway(i string) string {
	cmd := exec.Command("nmcli", "dev", "show", i)
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

func GetIpv6DhcpState(i string) string {
	connection := GetConnectionNameFromDevice(i)

	cmd := exec.Command("nmcli", "-f", "ipv6.method", "con", "show", connection)
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

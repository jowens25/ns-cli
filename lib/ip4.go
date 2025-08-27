package lib

import (
	"fmt"
	"log"
	"net"
	"os/exec"
	"strings"
)

func GetIpv4Address(i string) string {
	cmd := exec.Command("nmcli", "-f", "IP4.ADDRESS", "dev", "show", i)
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

	return "ipv4 address not available"
}

func GetIpv4Netmask(i string) string {
	cmd := exec.Command("nmcli", "-f", "IP4.ADDRESS", "dev", "show", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Fatal(err.Error())
	}

	fields := strings.Fields(string(out))

	if len(fields) == 2 {
		_, ipnet, err := net.ParseCIDR(fields[1])
		if err != nil {
			fmt.Printf("Error parsing CIDR: %v\n", err)
		}
		return ipnet.Mask.String()
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

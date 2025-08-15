package api

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"os/exec"
	"strings"
)

// Get preferred outbound ip of this machine
func GetOutboundIP() net.IP {
	conn, err := net.Dial("udp", "8.8.8.8:80") // doesn't make a connection so it should be ok
	if err != nil {
		log.Fatal(err)
	}
	defer conn.Close()

	localAddr := conn.LocalAddr().(*net.UDPAddr)

	return localAddr.IP
}

func SetIpAndGw(ip string, gw ...string) {

	file, err := os.Open("/etc/network/interfaces")

	if err != nil {
		log.Println(err)
	}
	defer file.Close()

	var lines []string
	scanner := bufio.NewScanner(file)
	// read all the lines, find placements
	for scanner.Scan() {
		line := scanner.Text()

		if strings.Contains(line, "address") {
			line = fmt.Sprintf("address %s", ip)
		}

		if len(gw) != 0 {
			if strings.Contains(line, "gateway") {
				line = fmt.Sprintf("gateway %s", gw[0])
			}

		}

		lines = append(lines, line)

	}

	err = os.WriteFile("/etc/network/interfaces", []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Fatal("failed to write file:", err)
	}

	cmd := exec.Command("systemctl", "restart", "networking")
	out, _ := cmd.CombinedOutput()
	if err != nil {
		log.Println(err, string(out))
	}

}

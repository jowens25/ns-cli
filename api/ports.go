package api

import (
	"bufio"
	"log"
	"os/exec"
	"strings"
)

func GetPortStatus(port string) string {
	port = strings.TrimSpace(port)
	cmd := exec.Command("ufw", "status")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}
	scanner := bufio.NewScanner(strings.NewReader(string(out)))

	for scanner.Scan() {

		line := scanner.Text()

		if strings.Contains(line, port) {

			if strings.Contains(line, "ALLOW") {
				return "active"
			} else if strings.Contains(line, "DENY") {
				return "inactive"
			} else {
				return "port misconfigured"
			}

		} else {
			continue
		}
	}

	return "port not found"
}

func DisablePort(port string) string {
	cmd := exec.Command("ufw", "deny", port)
	out, _ := cmd.CombinedOutput()
	return strings.TrimSpace(string(out))
}

func EnablePort(port string) string {
	cmd := exec.Command("ufw", "allow", port)
	out, _ := cmd.CombinedOutput()
	return strings.TrimSpace(string(out))
}

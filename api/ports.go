package api

import (
	"log"
	"os/exec"
	"strings"
)

func DisablePort(port string) string {
	cmd := exec.Command("ufw", "deny", port)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(err)
	}

	return strings.TrimSpace(string(out))
}

func EnablePort(port string) string {
	cmd := exec.Command("ufw", "allow", port)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(err)
	}

	return strings.TrimSpace(string(out))
}

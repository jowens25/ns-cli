package lib

import (
	"log"
	"os/exec"
)

func RestartXinetd() {
	cmd := exec.Command("systemctl", "restart", "xinetd")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}
}

package lib

import (
	"os/exec"
)

func GetHostname() string {
	cmd := exec.Command("hostname")
	out, err := cmd.CombinedOutput()
	if err != nil {
		return err.Error()
	}
	return string(out)
}

func SetHostname(name string) {
	cmd := exec.Command("hostnamectl", "set-hostname", name)
	cmd.CombinedOutput()
}

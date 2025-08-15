package api

import (
	"log"
	"os/exec"
)

func DisableInterface(i string) {
	cmd := exec.Command("ip", "link", "set", i, "down")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}

}

func EnableInterface(i string) {
	cmd := exec.Command("ip", "link", "set", i, "up")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}
}

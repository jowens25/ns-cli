package lib

import (
	"fmt"
	"os/exec"
)

func ToggleNtpSync(state string) {

	myCmd := exec.Command("timedatectl", "set-ntp", state)
	out, err := myCmd.CombinedOutput()
	if err != nil {
		fmt.Println(string(out), err)
	}
	fmt.Print(string(out))
}

package lib

import (
	"fmt"
	"log"
	"os/exec"
)

func RestartNginx() {
	cmd := exec.Command("systemctl", "restart", "nginx")
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}
}

func InitNginxConfig() {

	cmd := exec.Command("systemctl", "stop", "nginx")
	out, err := cmd.CombinedOutput()
	if err != nil {
		fmt.Println(string(out), err)
	}
	fmt.Println(string(out), err)

	cmd = exec.Command("cp", "selfsigned.key", "/etc/nginx/ssl/selfsigned.key")

	out, err = cmd.CombinedOutput()
	if err != nil {
		fmt.Println(string(out), err)
	}
	fmt.Println(string(out), err)

	cmd = exec.Command("cp", "nginx.conf", AppConfig.Nginx.Config)
	out, err = cmd.CombinedOutput()
	if err != nil {
		fmt.Println(string(out), err)
	}
	fmt.Println(string(out), err)

	cmd = exec.Command("systemctl", "start", "nginx")
	out, err = cmd.CombinedOutput()
	if err != nil {
		fmt.Println(string(out), err)
	}
	fmt.Println(string(out), err)

}

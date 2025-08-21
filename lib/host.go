package lib

import (
	"bufio"
	"log"
	"os"
	"os/exec"
	"strings"
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

	replaceHostname(name)
	cmd := exec.Command("hostnamectl", "set-hostname", name)
	cmd.CombinedOutput()
}

func replaceHostname(newHostname string) {
	hostFile := "/etc/hosts"
	file, err := os.Open(hostFile)
	if err != nil {
		log.Fatal("failed to open hosts file", file.Name())
	}
	defer file.Close()

	currentHost := GetHostname()
	var lines []string

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()

		if strings.Contains(line, currentHost) {
			fields := strings.Fields(line)
			if len(fields) == 2 {
				// host and ip
				line = fields[0] + "\t" + newHostname
			}
		}
		lines = append(lines, line)
	}

	err = os.WriteFile(hostFile, []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Fatal("failed to hosts file:", err)
	}
}

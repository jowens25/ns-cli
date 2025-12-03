package lib

import (
	"os"
	"os/exec"
	"strings"
)

func GetHostname() string {
	name, err := os.Hostname()
	if err != nil {
		Print("%s", err.Error())
	}
	return name
}

func SetHostname(name string) {

	replaceHostname(name)
	cmd := exec.Command("hostnamectl", "set-hostname", name)
	cmd.CombinedOutput()
}

func replaceHostname(newHostname string) {
	hostFile := "/etc/hosts"
	content, err := os.ReadFile(hostFile)
	if err != nil {
		Print("failed to read hosts file %s", hostFile)
	}

	currentHost := strings.TrimSpace(GetHostname())
	var lines []string

	for line := range strings.SplitSeq(string(content), "\n") {
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
		Print("failed to update hosts file: %s", err)
	}
}

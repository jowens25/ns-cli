package api

import (
	"bufio"
	"log"
	"os"
	"os/exec"
	"strings"
)

func DisableInterface(i string) {
	cmd := exec.Command("ifdown", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}

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

		if strings.Contains(line, "auto") {
			line = "# " + line
		}
		if strings.Contains(line, "allow-auto") {
			line = "# " + line
		}
		if strings.Contains(line, "allow-hotplug") {
			line = "# " + line
		}
		if strings.Contains(line, "allow-class") {
			line = "# " + line
		}

		lines = append(lines, line)

	}

	err = os.WriteFile("/etc/network/interfaces", []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Fatal("failed to write file:", err)
	}

	cmd = exec.Command("systemctl", "restart", "networking")

	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}

}

func EnableInterface(i string) {
	cmd := exec.Command("ifup", i)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}

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

		if after, ok := strings.CutPrefix(line, "# auto"); ok {
			line = after
		}

		if after, ok := strings.CutPrefix(line, "# allow-auto"); ok {
			line = after
		}
		if after, ok := strings.CutPrefix(line, "# allow-hotplug"); ok {
			line = after
		}
		if after, ok := strings.CutPrefix(line, "# allow-class"); ok {
			line = after
		}

		lines = append(lines, line)

	}

	err = os.WriteFile("/etc/network/interfaces", []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Fatal("failed to write file:", err)
	}

	cmd = exec.Command("systemctl", "restart", "networking")

	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}
}

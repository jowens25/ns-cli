package api

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"os/exec"
	"strings"
)

var directives = []string{"auto", "allow-auto", "allow-hotplug", "allow-class"}

func commentDirectiveLines(l string, i string) string {
	for _, directive := range directives {
		if strings.Contains(l, directive+" "+i) && !strings.HasPrefix(l, "#") {
			return "#" + l
		}
	}
	return l
}

func uncommentDirectiveLines(l string, i string) string {
	for _, directive := range directives {
		if strings.Contains(l, directive+" "+i) && strings.HasPrefix(l, "#") {
			return l[1:]
		}
	}
	return l
}

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

		line = commentDirectiveLines(line, i)

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
	fmt.Println(i+": ", GetInterfaceStatus(i))

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

		line = uncommentDirectiveLines(line, i)

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

	fmt.Println(i+": ", GetInterfaceStatus(i))

}

func GetInterfaceStatus(i string) string {

	file, err := os.Open("/etc/network/interfaces")

	if err != nil {
		log.Println(err)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	// read all the lines, find placements
	for scanner.Scan() {
		line := scanner.Text()

		for _, directive := range directives {
			if strings.Contains(line, directive+" "+i) && !strings.HasPrefix(line, "#") {
				return "active"
			}
		}
	}

	return "inactive"
}

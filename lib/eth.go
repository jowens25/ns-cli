package lib

import (
	"bufio"
	"bytes"
	"fmt"
	"log"
	"os"
	"os/exec"
	"strings"
	"time"
)

var directives = []string{"auto", "allow-auto", "allow-hotplug", "allow-class"}

func GetPhysicalEthStatus(ethPort string) string {
	cmd := exec.Command("ethtool", ethPort)
	out, err := cmd.CombinedOutput()

	if strings.Contains(string(out), "Link detected: yes") {
		status := ethPort
		scanner := bufio.NewScanner(bytes.NewReader(out))
		for scanner.Scan() {
			line := scanner.Text()
			if strings.Contains(line, "Speed: ") {
				status = status + line[:7]
			}
			if strings.Contains(line, "Duplex: ") {
				status = status + line[:8]

			}

		}
		return status

	} else if strings.Contains(string(out), "Link detected: no") {
		return ethPort + " (Unplugged)"
	} else {
		return err.Error()
	}
}

func waitingDots() {
	fmt.Print("Please wait")

	for range 5 {
		fmt.Print(".")
		time.Sleep(1 * time.Second)
	}
	fmt.Println()
}

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

	cmd = exec.Command("systemctl", "stop", "networking")

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

	waitingDots()

	cmd = exec.Command("systemctl", "start", "networking")

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

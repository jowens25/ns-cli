package lib

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"slices"
	"strings"
)

// reset the network restriction, update webserver config, and xinetd.d configs
func Unrestrict() {
	InitFtpConfig()
	InitSshConfig()
	InitTelnetConfig()
	InitNginxConfig()
	RestartXinetd()
	RestartNginx()
}

func AddAccess(ipAddress string) {
	ftpFile := "/etc/xinetd.d/ftp"
	telnetFile := "/etc/xinetd.d/telnet"
	sshFile := "/etc/xinetd.d/ssh"

	configs := []string{ftpFile, telnetFile, sshFile}

	for _, config := range configs {

		file, err := os.Open(config)
		if err != nil {
			log.Fatal("failed to open config file", file.Name())
		}
		defer file.Close()

		var lines []string
		op := "="

		scanner := bufio.NewScanner(file)
		for scanner.Scan() {
			line := scanner.Text()

			if strings.Contains(strings.TrimSpace(line), "only_from") {
				op = "+="
			}

			if strings.Contains(strings.TrimSpace(line), "}") {
				//lines = append(lines)

				lines = slices.Insert(lines, len(lines), "    only_from"+" "+op+" "+ipAddress)

			}

			lines = append(lines, line)
		}
		fmt.Println(lines)

		err = os.WriteFile(config, []byte(strings.Join(lines, "\n")+"\n"), 0644)
		if err != nil {
			log.Fatal("failed to telnet file:", err)
		}

		RestartXinetd()

	}

}

func RemoveAccess(ipAddress string) {
	ftpFile := "/etc/xinetd.d/ftp"
	telnetFile := "/etc/xinetd.d/telnet"
	sshFile := "/etc/xinetd.d/ssh"

	configs := []string{ftpFile, telnetFile, sshFile}

	for _, config := range configs {

		content, err := os.ReadFile(config)
		if err != nil {
			log.Printf("failed to read config file %s: %v", config, err)
			continue
		}

		lines := strings.Split(string(content), "\n")
		var filteredLines []string

		for _, line := range lines {

			if strings.Contains(strings.TrimSpace(line), ipAddress) {
				fmt.Println(line)
				continue
			}

			filteredLines = append(filteredLines, line)
		}

		// Write back to file
		newContent := strings.Join(filteredLines, "\n")
		err = os.WriteFile(config, []byte(newContent), 0644)
		if err != nil {
			log.Printf("failed to write config file %s: %v", config, err)
			continue
		}
	}

	RestartXinetd()

}

func AddNginxAccess(ipAddress string) {
	nginxFile := "/etc/nginx/nginx.conf"
	//nginxFile = "nginx.conf"

	content, err := os.ReadFile(nginxFile)
	if err != nil {
		log.Printf("failed to read nginxFile file %s: %v", nginxFile, err)

	}

	lines := strings.Split(string(content), "\n")
	var newLines []string

	for _, line := range lines {

		if strings.Contains(strings.TrimSpace(line), "allow all") {
			continue
		}

		newLines = append(newLines, line)

		if strings.Contains(strings.TrimSpace(line), "# Access Control") {

			newLines = append(newLines, "\t\t\tallow "+ipAddress+";")

			if !strings.Contains(strings.TrimSpace(line), "deny all;") {
				newLines = append(newLines, "\t\t\tdeny all; ")

			}
		}

	}

	// Write back to file
	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(nginxFile, []byte(newContent), 0644)
	if err != nil {
		log.Printf("failed to write config file %s: %v", nginxFile, err)

	}
	RestartNginx()
}

func RemoveNginxAccess(ipAddress string) {
	nginxFile := "/etc/nginx/nginx.conf"
	//nginxFile = "nginx.conf"

	content, err := os.ReadFile(nginxFile)
	if err != nil {
		log.Printf("failed to read config file %s: %v", nginxFile, err)

	}

	lines := strings.Split(string(content), "\n")
	var filteredLines []string

	//var allowDirectiveCount int

	lineNum := 0

	for i, line := range lines {

		if strings.Contains(strings.TrimSpace(line), "# Access Control") {
			lineNum = i
		}

		if strings.Contains(strings.TrimSpace(line), ipAddress) {
			continue
		}

		filteredLines = append(filteredLines, line)
	}

	if NumAllowDirectives(filteredLines) == 0 && !HasAllowAllDirective(filteredLines) {
		filteredLines = slices.Insert(filteredLines, lineNum+1, "\t\t\tallow all;")
		filteredLines = RemoveDenyAll(filteredLines)
	}

	// Write back to file
	newContent := strings.Join(filteredLines, "\n")
	err = os.WriteFile(nginxFile, []byte(newContent), 0644)
	if err != nil {
		log.Printf("failed to write config file %s: %v", nginxFile, err)

	}
	RestartNginx()
}

func NumAllowDirectives(lines []string) int {

	num := 0

	for _, line := range lines {

		if strings.Contains(strings.TrimSpace(line), "allow") {

			line = strings.TrimSuffix(line, ";")

			fields := strings.Fields(line)

			if net.ParseIP(fields[1]) != nil {
				num += 1
				continue
			}

			ip, _, err := net.ParseCIDR(fields[1])
			if err == nil && ip != nil {
				num += 1
				continue
			}

		}

	}

	return num

}

func HasAllowAllDirective(lines []string) bool {

	for _, line := range lines {

		if strings.Contains(strings.TrimSpace(line), "allow all") {

			return true

		}

	}

	return false
}

func RemoveDenyAll(lines []string) []string {
	var newLines []string
	for _, line := range lines {

		if !strings.Contains(strings.TrimSpace(line), "deny all") {

			newLines = append(newLines, line)

		}

	}

	return newLines
}

// remove dir
// if zero, add allow all

// add dir, remove allow all

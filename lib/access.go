package lib

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"net/http"
	"os"
	"slices"
	"strings"

	"github.com/gin-gonic/gin"
)

// ======================================= Handlers =======================================

func readAllowedNodes(c *gin.Context) {

	var allowedNodes []AllowedNode
	nodes := ReadAccessFromFiles()

	for _, node := range nodes {

		var newNode AllowedNode
		newNode.Address = node

		allowedNodes = append(allowedNodes, newNode)
	}

	c.JSON(http.StatusOK, gin.H{
		"allowed_nodes": allowedNodes,
	})
}

func writeAllowedNodes(c *gin.Context) {
	var node AllowedNode

	if err := c.ShouldBindJSON(&node); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	AddAccessToFiles(node.Address)

	c.JSON(http.StatusOK, gin.H{
		"message":      "Node Added",
		"allowed_node": node,
	})
}

func deleteAllowedNode(c *gin.Context) {

	var nodeToDelete AllowedNode

	if err := c.ShouldBindJSON(&nodeToDelete); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	RemoveAccessFromFiles(nodeToDelete.Address)

	c.JSON(http.StatusOK, gin.H{
		"message": "Node removed successfully",
	})
}

func resetNetworkAccess(c *gin.Context) {

	Unrestrict()

	c.JSON(http.StatusOK, gin.H{
		"message": "Network Access Reset",
	})
}

// ========================================================================================
// ======================================= Functions ======================================
func Unrestrict() {
	InitFtpConfig()
	InitSshConfig()
	InitTelnetConfig()
	InitNginxConfig()
	RestartXinetd()
	RestartNginx()
	fmt.Println("Network access reset")
}

func AddAccessToFiles(addr string) {

	_, _, err := net.ParseCIDR(addr)
	if err != nil {
		fmt.Println("invalid ip")
		return
	}

	addAccessToNginxFile(addr)
	addAccessToXinetdFile(addr)
}

func RemoveAccessFromFiles(addr string) {
	_, _, err := net.ParseCIDR(addr)
	if err != nil {
		fmt.Println("invalid ip")
		return
	}
	removeAccessFromNginxFile(addr)
	removeAccessFromXinetdFile(addr)

}

func ReadAccessFromFiles() []string {

	var allowed_nodes []string

	//sshFile := "/etc/xinetd.d/ssh"
	sshFile := AppConfig.Xinetd.SshPath

	content, err := os.ReadFile(sshFile)
	if err != nil {
		log.Printf("failed to read config file %s: %v", sshFile, err)
	}

	for line := range strings.SplitSeq(string(content), "\n") {

		if strings.Contains(strings.TrimSpace(line), "only_from") {

			fields := strings.Fields(line)

			if net.ParseIP(fields[2]) != nil {
				allowed_nodes = append(allowed_nodes, fields[2])
				continue
			}

			ip, _, err := net.ParseCIDR(fields[2])
			if err == nil && ip != nil {

				allowed_nodes = append(allowed_nodes, fields[2])
				continue
			}

		}

	}

	return allowed_nodes

}

func addAccessToXinetdFile(ipAddress string) {
	ftpFile := AppConfig.Xinetd.FtpPath
	telnetFile := AppConfig.Xinetd.TelnetPath
	sshFile := AppConfig.Xinetd.SshPath

	configs := []string{ftpFile, telnetFile, sshFile}

	for _, config := range configs {

		file, err := os.Open(config)
		if err != nil {
			log.Println("failed to open config file", configs)
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
		//fmt.Println(lines)

		err = os.WriteFile(config, []byte(strings.Join(lines, "\n")+"\n"), 0644)
		if err != nil {
			log.Println("failed to telnet file:", err)
		}

		RestartXinetd()

	}

}

func removeAccessFromXinetdFile(ipAddress string) {

	ftpFile := AppConfig.Xinetd.FtpPath
	telnetFile := AppConfig.Xinetd.TelnetPath
	sshFile := AppConfig.Xinetd.SshPath

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
				//fmt.Println(line)
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

func addAccessToNginxFile(ipAddress string) {
	//nginxFile := "/etc/nginx/nginx.conf"
	nginxFile := AppConfig.Nginx.Config

	content, err := os.ReadFile(nginxFile)
	if err != nil {
		log.Printf("failed to read nginx config file %s: %v", nginxFile, err)

	}

	lines := strings.Split(string(content), "\n")

	var allowLineNum int = -1
	var denyLineNum int = -1
	var directiveLineNum int = -1
	var accessLineNum int = -1

	for i, line := range lines {

		if strings.Contains(strings.TrimSpace(line), "# Access Control") {
			accessLineNum = i
		}

		if strings.Contains(strings.TrimSpace(line), "allow all") {
			allowLineNum = i
		}

		if strings.Contains(strings.TrimSpace(line), "deny all") {
			denyLineNum = i
		}

		if strings.Contains(strings.TrimSpace(line), ipAddress) {
			directiveLineNum = i
		}
	}

	if allowLineNum > 0 {
		lines = Pop(lines, allowLineNum)
		lines = slices.Insert(lines, accessLineNum+1, "            allow "+ipAddress+";")
		lines = slices.Insert(lines, accessLineNum+2, "            deny all;")
	}

	if denyLineNum > 0 {
		lines = slices.Insert(lines, accessLineNum+1, "            allow "+ipAddress+";")
	}

	if directiveLineNum > 0 {
		return
	}

	newContent := strings.Join(lines, "\n")
	err = os.WriteFile(nginxFile, []byte(newContent), 0644)
	if err != nil {
		log.Printf("failed to write config file %s: %v", nginxFile, err)

	}
	RestartNginx()
}

// remove element from the slice
func Pop(lines []string, index int) []string {
	return slices.Delete(lines, index, index+1)
}

func removeAccessFromNginxFile(ipAddress string) {
	//nginxFile := "/etc/nginx/nginx.conf"
	nginxFile := AppConfig.Nginx.Config
	content, err := os.ReadFile(nginxFile)
	if err != nil {
		log.Printf("failed to read nginxFile file %s: %v", nginxFile, err)

	}

	lines := strings.Split(string(content), "\n")

	//var allowLineNum int = -1
	var denyLineNum int = -1
	var directiveLineNum int = -1
	var accessLineNum int = -1
	var directiveCount int = 0

	for i, line := range lines {

		if strings.Contains(strings.TrimSpace(line), "# Access Control") {
			accessLineNum = i
		}

		if strings.Contains(strings.TrimSpace(line), "allow all") {
			//allowLineNum = i
		}

		if strings.Contains(strings.TrimSpace(line), "deny all") {
			denyLineNum = i
		}

		if strings.Contains(strings.TrimSpace(line), ipAddress) {
			directiveLineNum = i
		}

		if strings.Contains(strings.TrimSpace(line), "allow") {
			directiveCount++
		}

	}

	if directiveLineNum < 0 {
		return
	}

	// last directive, remove and add allow
	if directiveCount == 1 {

		lines = Pop(lines, denyLineNum)
		lines = Pop(lines, directiveLineNum)
		lines = slices.Insert(lines, accessLineNum+1, "            allow all;")

	} else {
		lines = Pop(lines, directiveLineNum)

	}

	newContent := strings.Join(lines, "\n")
	err = os.WriteFile(nginxFile, []byte(newContent), 0644)
	if err != nil {
		log.Printf("failed to write config file %s: %v", nginxFile, err)

	}
	RestartNginx()
}

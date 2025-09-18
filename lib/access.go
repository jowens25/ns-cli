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
	"gorm.io/gorm"
)

// ======================================= Handlers =======================================
var allowed_nodes []AllowedNode

func readAllowedNodes(c *gin.Context) {

	readXinetdAllowedNodes()

	var currentNodes []AllowedNode
	result := db.Find(&currentNodes)

	if result.Error != nil {
		log.Println(result.Error)
	}

	c.JSON(http.StatusOK, gin.H{
		"allowed_nodes": currentNodes,
	})
}

func writeAllowedNodes(c *gin.Context) {
	var node AllowedNode

	if err := c.ShouldBindJSON(&node); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	result := db.Where("address = ?", node.Address).FirstOrCreate(&node)

	if result.Error != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
		return
	}

	AddAccessToFiles(node.Address)

	c.JSON(http.StatusOK, gin.H{
		"message":      "Node Added",
		"allowed_node": node,
	})
}

func deleteAllowedNode(c *gin.Context) {
	id := c.Param("id")

	var nodeToDelete AllowedNode

	if err := db.First(&nodeToDelete, id).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	RemoveAccessFromFiles(nodeToDelete.Address)

	if err := db.Delete(&nodeToDelete).Error; err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"message": "Node removed successfully",
	})
}

func unrestrictNetworkAccess(c *gin.Context) {

	Unrestrict()

	c.JSON(http.StatusOK, gin.H{
		"message": "Network Access Reset",
	})
}

// ========================================================================================

// ======================================= Functions =======================================

// reset the network restriction, update webserver config, and xinetd.d configs
func Unrestrict() {
	//db.Unscoped().Where("1 = 1").Delete(&AllowedNode{}) // hard delete
	InitFtpConfig()
	InitSshConfig()
	InitTelnetConfig()
	InitNginxConfig()
	RestartXinetd()
	RestartNginx()

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

func readXinetdAllowedNodes() {

	allowed_nodes = nil

	sshFile := "/etc/xinetd.d/ssh"

	content, err := os.ReadFile(sshFile)
	if err != nil {
		log.Printf("failed to read config file %s: %v", sshFile, err)
	}

	for line := range strings.SplitSeq(string(content), "\n") {

		if strings.Contains(strings.TrimSpace(line), "only_from") {

			fields := strings.Fields(line)

			if net.ParseIP(fields[2]) != nil {
				var node AllowedNode
				node.Address = fields[2]
				allowed_nodes = append(allowed_nodes, node)
				continue
			}

			ip, _, err := net.ParseCIDR(fields[2])
			if err == nil && ip != nil {
				var node AllowedNode
				node.Address = fields[2]
				allowed_nodes = append(allowed_nodes, node)
				continue
			}

		}

	}

	// update the datebase to reflect the files
	var new_allowed_nodes []string

	for _, node := range allowed_nodes {

		// look up the user by user name
		result := db.Where("address = ?", node.Address).First(&AllowedNode{})
		if result.Error == gorm.ErrRecordNotFound {
			// Create new user
			db.Create(&node)

		} else {
			// Update existing user
			db.Where("address = ?", node.Address).Updates(&node)
		}

		new_allowed_nodes = append(new_allowed_nodes, node.Address)
	}

	db.Where("address NOT IN ?", new_allowed_nodes).Delete(&AllowedNode{})
}

func addAccessToXinetdFile(ipAddress string) {
	ftpFile := "/etc/xinetd.d/ftp"
	telnetFile := "/etc/xinetd.d/telnet"
	sshFile := "/etc/xinetd.d/ssh"

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
		fmt.Println(lines)

		err = os.WriteFile(config, []byte(strings.Join(lines, "\n")+"\n"), 0644)
		if err != nil {
			log.Println("failed to telnet file:", err)
		}

		RestartXinetd()

	}

}

func removeAccessFromXinetdFile(ipAddress string) {
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

func addAccessToNginxFile(ipAddress string) {
	nginxFile := "/etc/nginx/nginx.conf"

	content, err := os.ReadFile(nginxFile)
	if err != nil {
		log.Printf("failed to read nginxFile file %s: %v", nginxFile, err)

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
	nginxFile := "/etc/nginx/nginx.conf"
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

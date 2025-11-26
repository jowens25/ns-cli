package lib

import (
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
		log.Println(string(out), err)
	}

	cmd = exec.Command("cp", AppConfig.App.DefaultConfigs+"nginx.conf", AppConfig.Nginx.Config)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}
	//fmt.Println(string(out), err)

	cmd = exec.Command("systemctl", "start", "nginx")
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err)
	}
	//fmt.Println(string(out), err)

}

/*
func DisableHttp() {

	conf := AppConfig.Nginx.Config

	lines := OpenConfigFile(conf)

	for i, line := range lines {

	}

	err = os.WriteFile(conf, []byte(strings.Join(lines, "\n")+"\n"), 0644)

	RestartNginx()

	fmt.Println(GetHttpStatus())

}

func EnableHttp() {
	file, err := os.Open(AppConfig.Xinetd.FtpPath)
	if err != nil {
		log.Println("failed to open ftp file", AppConfig.Xinetd.FtpPath)
	}
	defer file.Close()

	var lines []string

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		if strings.Contains(strings.TrimSpace(line), "disable = yes") {
			line = "    disable = no"
		}
		lines = append(lines, line)
	}

	err = os.WriteFile(AppConfig.Xinetd.FtpPath, []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Println("failed to ftp file:", err)
	}

	RestartXinetd()

	fmt.Println(GetFtpStatus())

}

func GetHttpStatus() string {
	file, err := os.Open(AppConfig.Xinetd.FtpPath)
	if err != nil {
		log.Println("failed to open ftp file", AppConfig.Xinetd.FtpPath)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()

		if strings.Contains(line, "disable = yes") {
			return "inactive"
		} else if strings.Contains(line, "disable = no") {
			return "active"
		}
	}
	return "failed to get ftp status"
}

func readHttpStatus(c *gin.Context) {

	var myhttp Http

	myhttp.Status = GetHttpStatus()

	c.JSON(http.StatusOK, gin.H{
		"info": myhttp,
	})

}

func writeHttpStatus(c *gin.Context) {
	var myhttp Http
	if err := c.ShouldBindJSON(&myhttp); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	if myhttp.Action == "start" {
		EnableFtp()
	}

	if myhttp.Action == "stop" {
		DisableFtp()
	}

	myhttp.Status = GetFtpStatus()

	c.JSON(http.StatusOK, gin.H{
		"info": myhttp,
	})
}
*/

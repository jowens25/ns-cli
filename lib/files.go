package lib

import (
	"log"
	"os"
	"strings"
)

func OpenConfigFile(f string) []string {
	log.Println("open config", f)
	content, err := os.ReadFile(f)

	if err != nil {
		log.Printf("failed to config file %s: %v", f, err)
	}

	return strings.Split(string(content), "\n")
}

func SaveConfigFile(conf string, lines []string) bool {

	err := os.WriteFile(conf, []byte(strings.Join(lines, "\n")+"\n"), 0644)
	if err != nil {
		log.Printf("save config %s failed %s\r\n", conf, err)
		return false
	} else {
		log.Println("saved config")
		return true
	}
}

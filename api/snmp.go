package api

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"os/exec"
	"strings"

	"github.com/gin-gonic/gin"
	"gorm.io/gorm"
)

const (
	SNMP_CONFIG_PATH = "SNMP_CONFIG_PATH"
)

// readSnmpUsers reads the users in the snmpd.conf file
// Returns lists "v1v2c_users" "v3_users" and count "total_users" json
func readSnmpUsersFromFile() {
	v1v2c_users = nil
	v3_users = nil

	var groups []map[string]string
	var communities []map[string]string
	var createUsers []map[string]string
	file, err := os.Open(os.Getenv(SNMP_CONFIG_PATH))
	if err != nil {
		log.Fatal("failed to open config file", file.Name())
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	// Iterate and print each line
	for scanner.Scan() {
		line := scanner.Text()

		fields := strings.Fields(line)
		group := make(map[string]string)
		community := make(map[string]string)
		createUser := make(map[string]string)

		if len(fields) > 0 {
			// group - group name - sec.model - sec.name
			if strings.Contains(fields[0], "group") {
				if len(fields) == 4 {
					group["group_name"] = strings.TrimSpace(fields[1])
					group["version"] = strings.TrimSpace(fields[2])
					group["sec_name"] = strings.TrimSpace(fields[3])
					groups = append(groups, group)
				} else {
					log.Println("incomplete snmp entry")
					log.Println(fields)
				}
			}

			if strings.Contains(fields[0], "com2sec") {
				if len(fields) == 4 {
					community["sec_name"] = strings.TrimSpace(fields[1])
					community["source"] = strings.TrimSpace(fields[2])
					community["community"] = strings.TrimSpace(fields[3])
					communities = append(communities, community)
				} else {
					log.Println("incomplete snmp entry")
					log.Println(fields)

				}
			}

			if strings.Contains(fields[0], "createUser") {
				if len(fields) == 6 {
					createUser["user_name"] = strings.TrimSpace(fields[1])
					createUser["auth_type"] = strings.TrimSpace(fields[2])
					createUser["auth_passphrase"] = strings.TrimSpace(fields[3])
					createUser["priv_type"] = strings.TrimSpace(fields[4])
					createUser["priv_passphase"] = strings.TrimSpace(fields[5])

					createUsers = append(createUsers, createUser)
				} else {
					log.Println("incomplete snmp entry")
					log.Println(fields)

				}
			}
		}
	}

	//fmt.Println(groups)
	//fmt.Println(communities)
	//fmt.Println(createUsers)
	log.Println("FOR LOOP OVER GROUPS")
	for _, group := range groups {
		for _, community := range communities {
			if community["sec_name"] == group["sec_name"] {
				var user SnmpV1V2cUser
				user.Version = group["version"]
				user.GroupName = group["group_name"]
				user.Community = community["community"]
				user.Source = community["source"]
				user.SecName = community["sec_name"]
				//result := db.Where("community = ?", user.Community).Updates(&user)
				//if result.Error != nil {
				//					log.Println(result.Error)
				//}
				v1v2c_users = append(v1v2c_users, user)
			}
		}
		for _, createUser := range createUsers {
			if group["sec_name"] == createUser["user_name"] {
				var user SnmpV3User

				user.UserName = createUser["user_name"]
				user.AuthType = createUser["auth_type"]
				user.AuthPassphrase = createUser["auth_passphrase"]
				user.PrivType = createUser["priv_type"]
				user.PrivPassphrase = createUser["priv_passphase"]
				user.GroupName = group["group_name"]
				user.Version = group["version"]

				//result := db.Where("user_name = ?", user.UserName).Updates(&user)
				//if result.Error != nil {
				//	log.Println(result.Error)
				//}
				v3_users = append(v3_users, user)
			}
		}
	}

	var new_v3_usernames []string

	for _, user := range v3_users {

		// look up the user by user name
		result := db.Where("user_name = ?", user.UserName).First(&SnmpV3User{})
		if result.Error == gorm.ErrRecordNotFound {
			// Create new user
			db.Create(&user)

		} else {
			// Update existing user
			db.Where("user_name = ?", user.UserName).Updates(&user)
		}

		new_v3_usernames = append(new_v3_usernames, user.UserName)
	}

	db.Where("user_name NOT IN ?", new_v3_usernames).Delete(&SnmpV3User{})

	var new_v1v2c_communities []string

	for _, user := range v1v2c_users {

		// look up the user by user name
		result := db.Where("community = ?", user.Community).First(&SnmpV1V2cUser{})
		if result.Error == gorm.ErrRecordNotFound {
			// Create new user
			db.Create(&user)

		} else {
			// Update existing user
			db.Where("community = ?", user.Community).Updates(&user)
		}

		new_v1v2c_communities = append(new_v1v2c_communities, user.Community)
	}

	db.Where("community NOT IN ?", new_v1v2c_communities).Delete(&SnmpV1V2cUser{})

}

func resetSnmpConfig(c *gin.Context) {

	StopSnmpd()
	db.Unscoped().Where("1 = 1").Delete(&SnmpV1V2cUser{}) // hard delete
	db.Unscoped().Where("1 = 1").Delete(&SnmpV3User{})    // hard delete
	//db.Unscoped().Where("1 = 1").Delete(&SnmpTrap{})      // hard delete

	CopySnmpdConfig()

	RestartSnmpd()

}

func GetSnmpdStatus() string {
	cmd := exec.Command("systemctl", "is-active", "snmpd")
	out, _ := cmd.CombinedOutput()
	return strings.TrimSpace(string(out))
}

func StopSnmpd() {

	DisablePort("161")
	DisablePort("162")

	cmd := exec.Command("systemctl", "stop", "snmpd")
	out, _ := cmd.CombinedOutput()
	fmt.Println(out)
	log.Println("SNMPD: ", GetSnmpdStatus())
}

func StartSnmpd() {

	EnablePort("161")
	EnablePort("162")

	cmd := exec.Command("systemctl", "start", "snmpd")
	out, _ := cmd.CombinedOutput()
	fmt.Println(out)
	log.Println("SNMPD: ", GetSnmpdStatus())
}

func RestartSnmpd() {
	cmd := exec.Command("systemctl", "restart", "snmpd")
	out, _ := cmd.CombinedOutput()
	fmt.Println(out)
	log.Println("SNMPD: ", GetSnmpdStatus())
}

func CopySnmpdConfig() {
	dest := os.Getenv("SNMP_CONFIG_PATH")
	log.Println("should be here")
	log.Println(dest)

	cmd := exec.Command("cp", "snmpd.conf", dest)
	out, _ := cmd.CombinedOutput()
	log.Println(strings.TrimSpace(string(out)))
}

//db.Model(&SnmpV1V2cUser{}).Count(&count)
//
//snmpV1V2cUser.ID = count + 1
//
//result := db.Create(&snmpV1V2cUser)
//if result.Error != nil {
//	c.JSON(http.StatusInternalServerError, gin.H{"error": result.Error.Error()})
//	return
//}

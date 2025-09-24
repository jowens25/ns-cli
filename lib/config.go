package lib

import (
	"fmt"
	"log"
	"os/exec"
	"sync"

	"github.com/spf13/viper"
)

type Config struct {
	App      ApplicationConfig `mapstructure:"app"`
	Nginx    NginxConfig       `mapstructure:"nginx"`    // nginx
	Xinetd   XinetdConfig      `mapstructure:"xinetd"`   // xinetd
	Security SecurityConfig    `mapstructure:"security"` // pam
	Serial   SerialConfig      `mapstructure:"serial"`   // serial
	User     UserConfig        `mapstructure:"user"`     // users
	Snmp     SnmpConfig        `mapstructure:"snmp"`     // snmp
	Cors     CorsConfig        `mapstructure:"cors"`
	Api      ApiConfig         `mapstructure:"api"`
	Network  NetworkConfig     `mapstructure:"network"`
}

type ApplicationConfig struct {
	Database       string `mapstructure:"database"`        // /etc/ns/app.db
	Config         string `mapstructure:"config"`          // /etc/ns/config.toml
	Log            string `mapstructure:"log"`             // /etc/ns/app.log
	DefaultConfigs string `mapstructure:"default_configs"` // /usr/share/ns/configs/
}

type ApiConfig struct {
	Port string `mapstructure:"port"`
	Host string `mapstructure:"host"`
}

type NetworkConfig struct {
	Ip        string `mapstructure:"ip"`
	Dns1      string `mapstructure:"dns1"`
	Dns2      string `mapstructure:"dns2"`
	Interface string `mapstructure:"interface"`
}

type NginxConfig struct {
	Config string `mapstructure:"config"` // /etc/nginx/nginx.conf
	Key    string `mapstructure:"key"`    // /etc/nginx/ssl/selfsigned.key
	Cert   string `mapstructure:"cert"`   // /etc/nginx/ssl/selfsigned.crt
}

type XinetdConfig struct {
	FtpPath    string `mapstructure:"ftp"`    // /etc/xinetd.d/ftp
	TelnetPath string `mapstructure:"telnet"` // /etc/xinetd.d/telnet
	SshPath    string `mapstructure:"ssh"`    // /etc/xinetd.d/ssh

}

type SecurityConfig struct {
	Pwquality string `mapstructure:"pwquality"` // /etc/security/pwquality.conf
	Login     string `mapstructure:"login"`     // /etc/login.defs
}

type PasswordConfig struct {
}

type SerialConfig struct {
	Port     string `mapstructure:"port"`     // /dev/ttymxc2
	Baudrate int    `mapstructure:"baudrate"` // 38400
}

type UserConfig struct {
	AdminGroup      string `mapstructure:"group-admin"` //"novusadmin"
	UserGroup       string `mapstructure:"group-user"`  //"novususer"
	GroupPath       string `mapstructure:"group-path"`  // /etc/group
	DefaultPassword string `mapstructure:"default-password"`
	DefaultUsername string `mapstructure:"default-username"`
}

type SnmpConfig struct {
	Path string `mapstructure:"path"` // /etc/snmp/snmpd.conf
}

type CorsConfig struct {
	Host1 string `mapstructure:"host1"`
	Host2 string `mapstructure:"host2"`
}

func InitConfig() *Config {

	viper.SetConfigName("config")
	viper.SetConfigType("toml")
	viper.AddConfigPath("/etc/ns") // A system-wide path

	viper.SetDefault("app.database", "/etc/ns/app.db")
	viper.SetDefault("app.config", "/etc/ns/config.toml")
	viper.SetDefault("app.log", "/tmp/ns.log")
	viper.SetDefault("api.port", "5000")
	viper.SetDefault("api.host", "localhost") // production
	viper.SetDefault("app.default_configs", "/usr/share/ns/configs/")
	//viper.SetDefault("api.host", "0.0.0.0")   // development

	viper.SetDefault("nginx.config", "/etc/nginx/nginx.conf")
	//viper.SetDefault("nginx.defconfig", "/etc/nginx/def_nginx.conf")
	viper.SetDefault("nginx.key", "/etc/nginx/ssl/nginx.key")
	viper.SetDefault("nginx.cert", "/etc/nginx/ssl/nginx.crt")

	viper.SetDefault("network.ip", "10.1.10.220")
	viper.SetDefault("network.interface", "eth0")
	viper.SetDefault("network.dns1", "8.8.8.8")
	viper.SetDefault("network.dns2", "8.8.4.4")

	viper.SetDefault("xinetd.ftp", "/etc/xinetd.d/ftp")
	viper.SetDefault("xinetd.telnet", "/etc/xinetd.d/telnet")
	viper.SetDefault("xinetd.ssh", "/etc/xinetd.d/ssh")

	viper.SetDefault("security.pwquality", "/etc/security/pwquality.conf")
	viper.SetDefault("security.login", "/etc/login.defs")

	viper.SetDefault("serial.port", "/dev/ttymxc2")
	viper.SetDefault("serial.baudrate", 38400)

	viper.SetDefault("user.group-admin", "novusadmin")
	viper.SetDefault("user.group-user", "novususer")
	viper.SetDefault("user.group-path", "/etc/group")
	viper.SetDefault("user.default-username", "novus")
	viper.SetDefault("user.default-password", "novus")

	viper.SetDefault("snmp.path", "/etc/snmp/snmpd.conf")

	viper.SetDefault("cors.host1", "https://localhost")
	viper.SetDefault("cors.host2", "http://localhost")

	// Read config file
	if err := viper.ReadInConfig(); err != nil {
		if _, ok := err.(viper.ConfigFileNotFoundError); ok {
			fmt.Println("Config file not found, using defaults and environment variables")
			viper.SafeWriteConfig()
		} else {
			log.Printf("Error reading config file: %v", err)
		}
	}
	//else {
	//	fmt.Printf("Using config file: %s\n", viper.ConfigFileUsed())
	//}

	var config Config
	if err := viper.Unmarshal(&config); err != nil {
		log.Printf("Unable to decode config: %v", err)
	}

	return &config

}

// Global config variable
var AppConfig *Config
var once sync.Once

// GetConfig returns the global config, initializing it if necessary
func GetConfig() *Config {
	once.Do(func() {
		AppConfig = InitConfig()
	})
	return AppConfig
}

func CopyConfigs() {

	cmd := exec.Command("cp", AppConfig.App.DefaultConfigs+"ftp", AppConfig.Xinetd.FtpPath)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err.Error())
		return
	}

	cmd = exec.Command("cp", AppConfig.App.DefaultConfigs+"ssh", AppConfig.Xinetd.SshPath)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err.Error())
		return
	}

	cmd = exec.Command("cp", AppConfig.App.DefaultConfigs+"telnet", AppConfig.Xinetd.TelnetPath)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err.Error())
		return
	}

	cmd = exec.Command("cp", AppConfig.App.DefaultConfigs+"nginx.conf", AppConfig.Nginx.Config)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err.Error())
		return
	}

	cmd = exec.Command("cp", AppConfig.App.DefaultConfigs+"snmpd.conf", AppConfig.Snmp.Path)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err.Error())
		return
	}

	cmd = exec.Command("cp", AppConfig.App.DefaultConfigs+"login.defs", AppConfig.Security.Login)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err.Error())
		return
	}

	cmd = exec.Command("cp", AppConfig.App.DefaultConfigs+"pwquality.conf", AppConfig.Security.Pwquality)
	out, err = cmd.CombinedOutput()
	if err != nil {
		log.Println(string(out), err.Error())
		return
	}

	fmt.Println("configs copied")

	GetConfig()
}

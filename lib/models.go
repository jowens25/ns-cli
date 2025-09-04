package lib

import (
	"github.com/golang-jwt/jwt/v5"
	"gorm.io/gorm"
)

type User struct {
	gorm.Model
	Role     string `json:"role" gorm:"not null"`
	Username string `json:"username" gorm:"uniqueIndex;not null"`
	Email    string `json:"email" gorm:"uniqueIndex;not null"`
	Password string `json:"password" gorm:"not null"` // "-" excludes from JSON

}

type LoginRequest struct {
	Username string `json:"username" binding:"required"`
	Password string `json:"password" binding:"required"`
}

type LoginResponse struct {
	Token string `json:"token"`
	User  User   `json:"user"`
}

type Snmp struct {
	Status         string `json:"status"`
	Action         string `json:"action"`
	SysObjId       string `json:"sys_obj_id"`
	SysContact     string `json:"sys_contact"`
	SysLocation    string `json:"sys_location"`
	SysDescription string `json:"sys_description"`
}

type Telnet struct {
	Status string `json:"status"`
	Action string `json:"action"`
}

type Ssh struct {
	Status string `json:"status"`
	Action string `json:"action"`
}

type Ftp struct {
	Status string `json:"status"`
	Action string `json:"action"`
}
type Http struct {
	Status string `json:"status"`
	Action string `json:"action"`
}

type Ntp struct {
	gorm.Model
	Version          string `json:"version"`
	Instance         string `json:"instance"`
	Mac              string `json:"mac"`
	VlanAddress      string `json:"vlan_address"`
	VlanStatus       string `json:"vlan_status"`
	IpMode           string `json:"ip_mode"`
	IpAddress        string `json:"ip_address"`
	UnicastMode      string `json:"unicast_mode"`
	MulticastMode    string `json:"multicast_mode"`
	BroadcastMode    string `json:"broadcast_mode"`
	Status           string `json:"status"`
	Stratum          string `json:"stratum"`
	PollInterval     string `json:"poll_interval"`
	Precision        string `json:"precision"`
	ReferenceId      string `json:"reference_id"`
	Leap59           string `json:"leap59"`
	Leap59Inprogress string `json:"leap59_inprogress"`
	Leap61           string `json:"leap61"`
	Leap61Inprogress string `json:"leap61_inprogress"`
	Utc_smearing     string `json:"utc_smearing"`
	UtcOffsetStatus  string `json:"utc_offset_status"`
	UtcOffsetValue   string `json:"utc_offset_value"`
	Requests         string `json:"requests"`
	Responses        string `json:"responses"`
	RequestsDropped  string `json:"requests_dropped"`
	Broadcasts       string `json:"broadcasts"`
	ClearCounters    string `json:"clear_counters"`
}

type SnmpV1V2cUser struct {
	gorm.Model
	ComNumber string `json:"com_number"`
	Version   string `json:"version"`
	GroupName string `json:"group_name"`
	Community string `json:"community"`
	Source    string `json:"source"`
	SecName   string `json:"sec_name"`
}

type SnmpV3User struct {
	gorm.Model
	Version        string `json:"version"`
	UserName       string `json:"user_name"`
	AuthType       string `json:"auth_type"`
	AuthPassphrase string `json:"auth_passphrase"`
	PrivType       string `json:"priv_type"`
	PrivPassphrase string `json:"priv_passphrase"`
	GroupName      string `json:"group_name"`
}

type SnmpTrap struct {
	gorm.Model
	Version        string `json:"version"`
	Community      string `json:"user"`
	DestinationIp  string `json:"destination_ip"`
	Port           string `json:"port"`
	EngineId       string `json:"engine_id"`
	AuthType       string `json:"auth_type"`
	AuthPassphrase string `json:"auth_passphrase"`
	PrivType       string `json:"priv_type"`
	PrivPassphrase string `json:"priv_passphrase"`
}

type AllowedNode struct {
	gorm.Model
	Address string `json:"address"`
}

// JWT Claims
type Claims struct {
	UserID   uint   `json:"user_id"`
	UserRole string `json:"role"`
	Username string `json:"username"`
	jwt.RegisteredClaims
}

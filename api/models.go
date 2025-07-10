package api

import (
	"time"

	"github.com/golang-jwt/jwt/v5"
)

type User struct {
	ID        uint      `json:"id" gorm:"primaryKey"`
	Role      string    `json:"role" gorm:"not null"`
	Username  string    `json:"username" gorm:"uniqueIndex;not null"`
	Email     string    `json:"email" gorm:"uniqueIndex;not null"`
	Password  string    `json:"password" gorm:"not null"` // "-" excludes from JSON
	CreatedAt time.Time `json:"created_at"`
	UpdatedAt time.Time `json:"updated_at"`
	UserType  string    `json:""`
	//Posts     []Post    `json:"posts,omitempty" gorm:"foreignKey:AuthorID"`
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
	Status string `json:"status"`
	Action string `json:"action"`

	SysObjId       string `json:"sys_obj_id"`
	SysContact     string `json:"sys_contact"`
	SysLocation    string `json:"sys_location"`
	SysDescription string `json:"sys_description"`
}

type SnmpV1V2cUser struct {
	ID         int64  `json:"id" gorm:"primaryKey"`
	Version    string `json:"version"`
	GroupName  string `json:"group_name"`
	Community  string `json:"community"`
	IpVersion  string `json:"ip_version"`
	Ip4Address string `json:"ip4_address"`
	Ip6Address string `json:"ip6_address"`
}

type SnmpV3User struct {
	ID             int64  `json:"id" gorm:"primaryKey"`
	UserName       string `json:"user_name"`
	AuthType       string `json:"auth_type"`
	AuthPassphrase string `json:"auth_passphrase"`
	PrivType       string `json:"priv_type"`
	PrivPassphrase string `json:"priv_passphrase"`
	GroupName      string `json:"group_name"`
}

type SnmpTrap struct {
	ID                   int64  `json:"id" gorm:"primaryKey"`
	Version              string `json:"version"`
	User                 string `json:"user"`
	DestinationIpVersion string `json:"destination_ip_version"`
	DestinationIp        string `json:"destination_ip"`
	Port                 string `json:"port"`
	EngineId             string `json:"engine_id"`
	AuthType             string `json:"auth_type"`
	AuthPassphrase       string `json:"auth_passphrase"`
	PrivType             string `json:"priv_type"`
	PrivPassphrase       string `json:"priv_passphrase"`
}

// JWT Claims
type Claims struct {
	UserID   uint   `json:"user_id"`
	UserRole string `json:"role"`
	Username string `json:"username"`
	jwt.RegisteredClaims
}

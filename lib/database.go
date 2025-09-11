package lib

import (
	"fmt"
	"log"

	"gorm.io/driver/sqlite"
	"gorm.io/gorm"
	"gorm.io/gorm/logger"
)

var db *gorm.DB

func initDataBase() {

	var err error

	db, err = gorm.Open(sqlite.Open(AppConfig.App.Database), &gorm.Config{
		Logger: logger.Default.LogMode(logger.Info),
	})
	if err != nil {
		log.Fatal("Failed to connect to database: ", err)
	}

	err = db.AutoMigrate(&User{}, &SnmpV1V2cUser{}, &SnmpV3User{}, &Ntp{}, &AllowedNode{})

	if err != nil {
		log.Fatal("Failed to migrate database: ", err)
	}

	createDefaultUser()

	//loadSystem()

	log.Println("Database initialized successfully")

}

//func loadSystem() {
//
//}

func createDefaultUser() {

	var userCount int64
	db.Model(&User{}).Count(&userCount)

	if userCount == 0 {

		var user User
		user.Username = "novus"
		user.Password = "Novus123!"
		user.Role = "admin"

		result := db.Where("username = ?", user.Username).FirstOrCreate(&user)
		if result.Error != nil {
			fmt.Println(result.Error.Error())
			return
		}

		addAdminGroup()
		addUserGroup()

		warning, err := addUserToSystem(user)
		if err != nil {
			fmt.Println(err.Error())
		}
		fmt.Println(warning)

		// /readUsersFromSystem()

		//user := User{
		//
		//	Username: "admin",
		//	Role:     "admin",
		//	Email:    "",
		//	Password: "admin",
		//}
		//
		//db.Create(&user)
		//
		//snmpV1V2User := SnmpV1V2cUser{
		//	Version:   "v2c",
		//	GroupName: "read_write",
		//	Community: "myCommunity",
		//	//	IpVersion:  "ipv4",
		//	//	Ip4Address: "10.1.10.220",
		//}
		//
		//db.Create(&snmpV1V2User)

		//access := Access{
		//
		//	Node: "10.1.10.1",
		//}
		//db.Create(&access)
		//
		//access = Access{
		//
		//	Node: "10.1.10.2",
		//}
		//db.Create(&access)
		//access = Access{
		//
		//	Node: "10.1.10.3",
		//}
		//
		//db.Create(&access)

	}

}

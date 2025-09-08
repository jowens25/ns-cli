package lib

import (
	"log"

	"gorm.io/driver/sqlite"
	"gorm.io/gorm"
	"gorm.io/gorm/logger"
)

var db *gorm.DB

func initDataBase() {

	var err error

	db, err = gorm.Open(sqlite.Open(DB_PATH), &gorm.Config{
		Logger: logger.Default.LogMode(logger.Info),
	})
	if err != nil {
		log.Fatal("Failed to connect to database: ", err)
	}

	err = db.AutoMigrate(&User{}, &SnmpV1V2cUser{}, &SnmpV3User{}, &Ntp{}, &AllowedNode{})

	if err != nil {
		log.Fatal("Failed to migrate database: ", err)
	}

	//createDefaultUser()

	//loadSystem()

	log.Println("Database initialized successfully")

}

//func loadSystem() {
//
//}

//func createDefaultUser() {
//
//	var userCount int64
//	db.Model(&User{}).Count(&userCount)
//
//	if userCount == 0 {
//		adminPassword, _ := bcrypt.GenerateFromPassword([]byte("admin"), bcrypt.DefaultCost)
//
//		user := User{
//
//			Username: "admin",
//			Role:     "admin",
//			Email:    "admin@novuspower.com",
//			Password: string(adminPassword),
//		}
//
//		db.Create(&user)
//
//		user = User{
//
//			Username: "viewer",
//			Role:     "viewer",
//			Email:    "viewer@novuspower.com",
//			Password: string(adminPassword),
//		}
//
//		db.Create(&user)
//
//		user = User{
//
//			Username: "factory",
//			Role:     "admin",
//			Email:    "factory@novuspower.com",
//			Password: string(adminPassword),
//		}
//
//		db.Create(&user)
//
//		snmpV1V2User := SnmpV1V2cUser{
//			Version:   "v2c",
//			GroupName: "read_write",
//			Community: "myCommunity",
//			//	IpVersion:  "ipv4",
//			//	Ip4Address: "10.1.10.220",
//		}
//
//		db.Create(&snmpV1V2User)
//
//		//access := Access{
//		//
//		//	Node: "10.1.10.1",
//		//}
//		//db.Create(&access)
//		//
//		//access = Access{
//		//
//		//	Node: "10.1.10.2",
//		//}
//		//db.Create(&access)
//		//access = Access{
//		//
//		//	Node: "10.1.10.3",
//		//}
//		//
//		//db.Create(&access)
//
//	}
//
//}

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

	db, err = gorm.Open(sqlite.Open(AppConfig.App.Database), &gorm.Config{
		Logger: logger.Default.LogMode(logger.Info),
	})
	if err != nil {
		log.Println("Failed to connect to database: ", err)
	}

	err = db.AutoMigrate(&SnmpV2User{}, &SnmpV3User{}, &Ntp{}, &AllowedNode{})

	if err != nil {
		log.Println("Failed to migrate database: ", err)
	}

	createDefaultUser()

	//loadSystem()

	log.Println("Database initialized successfully")

}

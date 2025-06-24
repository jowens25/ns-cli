package api

import (
	"log"

	"golang.org/x/crypto/bcrypt"
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

	err = db.AutoMigrate(&User{})

	if err != nil {
		log.Fatal("Failed to migrate database: ", err)
	}

	createDefaultUser()

	log.Println("Database initialized successfully")

}

func createDefaultUser() {

	var userCount int64
	db.Model(&User{}).Count(&userCount)

	if userCount == 0 {
		adminPassword, _ := bcrypt.GenerateFromPassword([]byte("admin"), bcrypt.DefaultCost)

		user := User{

			Username: "admin",
			Email:    "jowens@novuspower.com",
			Password: string(adminPassword),
		}

		db.Create(&user)

		user = User{

			Username: "bill",
			Email:    "bill@novuspower.com",
			Password: string(adminPassword),
		}

		db.Create(&user)

	}

}

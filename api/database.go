package api

import (
	"log"

	"golang.org/x/crypto/bcrypt"
	"gorm.io/driver/sqlite"
	"gorm.io/gorm"
	"gorm.io/gorm/logger"
)

var Db *gorm.DB

func initDataBase() {

	var err error

	Db, err = gorm.Open(sqlite.Open(DB_PATH), &gorm.Config{
		Logger: logger.Default.LogMode(logger.Info),
	})
	if err != nil {
		log.Fatal("Failed to connect to database: ", err)
	}

	err = Db.AutoMigrate(&User{})

	if err != nil {
		log.Fatal("Failed to migrate database: ", err)
	}

	createDefaultUser()

	log.Println("Database initialized successfully")

}

func createDefaultUser() {

	var userCount int64
	Db.Model(&User{}).Count(&userCount)

	if userCount == 0 {
		adminPassword, _ := bcrypt.GenerateFromPassword([]byte("admin"), bcrypt.DefaultCost)

		user := User{

			Username: "admin",
			Role:     "admin",
			Email:    "admin@novuspower.com",
			Password: string(adminPassword),
		}

		Db.Create(&user)

		user = User{

			Username: "viewer",
			Role:     "viewer",
			Email:    "viewer@novuspower.com",
			Password: string(adminPassword),
		}

		Db.Create(&user)

		user = User{

			Username: "factory",
			Role:     "admin",
			Email:    "factory@novuspower.com",
			Password: string(adminPassword),
		}

		Db.Create(&user)

	}

}

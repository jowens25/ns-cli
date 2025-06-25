package api

import "time"

type User struct {
	ID        uint      `json:"id" gorm:"primaryKey"`
	Role      string    `json:"role" gorm:"not null"`
	Username  string    `json:"username" gorm:"uniqueIndex;not null"`
	Email     string    `json:"email" gorm:"uniqueIndex;not null"`
	Password  string    `json:"-" gorm:"not null"` // "-" excludes from JSON
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

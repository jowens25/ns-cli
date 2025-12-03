package lib

import (
	"fmt"
	"log"
	"net/http"

	"github.com/gin-gonic/gin"
	"github.com/msteinert/pam"
)

//func Authorize(user string, password string) bool {
//
//	// Start PAM transaction
//	t, err := pam.StartFunc("login", user, func(s pam.Style, msg string) (string, error) {
//		switch s {
//		case pam.PromptEchoOff:
//			return password, nil
//		case pam.PromptEchoOn:
//			return user, nil
//		}
//		return "", nil
//	})
//
//	if err != nil {
//		log.Println("pam failed")
//		return false
//	}
//	defer t.CloseSession(pam.Silent)
//
//	if err = t.Authenticate(0); err != nil {
//		fmt.Println("invalid cred")
//		return false
//	}
//
//	fmt.Println("authentication succeeded")
//
//	return true
//}
//
func loginHandler(c *gin.Context) {
	var request LoginRequest

	if err := c.ShouldBindJSON(&request); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{
			"error":   "Invalid request format",
			"details": err.Error(),
		})
		fmt.Println(err.Error())
		return
	}

	fmt.Println("request password: ", request.Password)

	if !Authorize(request.Username, request.Password) {
		c.JSON(http.StatusUnauthorized, gin.H{
			"error": "Invalid username or password",
		})
		return
	}

	var requestedUser User
	requestedUser.Username = request.Username
	user := lookUpSystemUser(requestedUser)

	token, err := generateJWT(&user)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": "Failed to generate token",
		})
		return
	}

	response := LoginResponse{
		Token: token,
		User:  user,
	}

	c.JSON(http.StatusOK, response)
}

package api

import (
	"NovusTimeServer/web"
	"io/fs"
	"net/http"

	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
)

func startJsServer() {
	jsRouter := gin.Default()

	webFS, err := fs.Sub(web.Files, "files")
	if err != nil {
		panic(err)
	}
	jsRouter.StaticFS("/", http.FS(webFS))

	go jsRouter.Run("10.1.10.205" + WEB_PORT)
}

func startApiServer() {
	apiRouter := gin.Default()
	apiRouter.SetTrustedProxies(nil)
	corsConfig := cors.DefaultConfig()
	corsConfig.AllowMethods = []string{"GET", "POST", "PUT", "DELETE", "OPTIONS", "PATCH"}
	corsConfig.AllowHeaders = []string{"Authorization", "Content-Type", "X-Request-ID"}
	corsConfig.AllowCredentials = true
	corsConfig.AllowAllOrigins = true
	//corsConfig.AllowOrigins = []string{
	//	"http://" + WEB_HOST + WEB_PORT,
	//	"http://" + API_HOST + API_PORT,
	//}

	//r.Use(corsConfig)
	apiRouter.Use(cors.New(corsConfig))

	apiRouter.Use(gin.Recovery())

	// api version group
	v1 := apiRouter.Group("/api/v1")

	// public routes
	v1.POST("/auth/login", loginHandler)

	protected := v1.Group("/")
	protected.Use(authorizationMiddleware())
	{
		protected.POST("/logout", logoutHandler)

		protected.GET("/health", healthHandler)

		protected.GET("/users", readUsers)
		protected.POST("/users", writeUser)
		protected.PATCH("/users/:id", editUser)
		protected.DELETE("/users/:id", deleteUser)

		snmpGroup := protected.Group("/snmp")

		snmpGroup.GET("/v1v2c_user", readSnmpV1V2cUsers)
		snmpGroup.POST("/v1v2c_user", writeSnmpV1V2cUser)
		snmpGroup.PATCH("/v1v2c_user/:id", editSnmpV1V2cUser)
		snmpGroup.DELETE("/v1v2c_user/:id", deleteSnmpV1V2cUser)

		snmpGroup.GET("v3_user", readSnmpV3Users)
		snmpGroup.POST("v3_user", writeSnmpV3User)
		snmpGroup.PATCH("v3_user/:id", editSnmpV3User)
		snmpGroup.DELETE("v3_user/:id", deleteSnmpV3User)

		snmpGroup.GET("/info", readSnmpInfo)
		snmpGroup.PATCH("/info", writeSnmpInfo)
		snmpGroup.GET("/reset_config", resetSnmpConfig)

		protected.GET("/ntp/:prop", readNtpProperty)
		protected.POST("/ntp/:prop", writeNtpProperty)

		networkGroup := protected.Group("/network")
		networkGroup.GET("/ssh", readSshStatus)
		networkGroup.PATCH("/ssh", writeSshStatus)
		networkGroup.GET("/http", readHttpStatus)
		networkGroup.PATCH("/http", writeHttpStatus)
		networkGroup.GET("/telnet", readTelnetStatus)
		networkGroup.PATCH("/telnet", writeTelnetStatus)

		//networkGroup.GET("/reset_network", resetNetworkConfig)

	}

	// 404 handler
	apiRouter.NoRoute(func(c *gin.Context) {
		c.JSON(http.StatusNotFound, gin.H{
			"error":   "Endpoint not found",
			"path":    c.Request.URL.Path,
			"method":  c.Request.Method,
			"message": "The requested resource could not be found",
		})
	})

	apiRouter.Run("10.1.10.205" + API_PORT)
}

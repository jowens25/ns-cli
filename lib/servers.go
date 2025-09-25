package lib

import (
	"net/http"

	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
)

func startApiServer() {
	apiRouter := gin.Default()
	//apiRouter.SetTrustedProxies([]string{AppConfig.Api.Host}) // localhost

	corsConfig := cors.DefaultConfig()

	corsConfig.AllowMethods = []string{"GET", "POST", "PUT", "DELETE", "OPTIONS", "PATCH"}
	corsConfig.AllowHeaders = []string{"Authorization", "Content-Type", "X-Request-ID"}
	corsConfig.AllowCredentials = true
	// offical
	//corsConfig.AllowOrigins = []string{
	//
	//	AppConfig.Cors.Host1,
	//	AppConfig.Cors.Host2, // production
	//
	//	//"https://localhost",
	//	//"http://localhost", // production
	//}

	corsConfig.AllowAllOrigins = true // development

	apiRouter.Use(cors.New(corsConfig))

	apiRouter.Use(gin.Recovery())

	// api version group
	v1 := apiRouter.Group("/api/v1")

	// public routes
	v1.POST("/login", loginHandler)
	v1.GET("/health", healthHandler)

	protected := v1.Group("/")
	{
		protected.POST("/logout", logoutHandler)

		protected.GET("/users", readSystemUsers)
		protected.POST("/users", writeSystemUser)
		//protected.PATCH("/users/:id", editSystemUser)
		protected.DELETE("/users/:id", deleteSystemUser)

		snmpGroup := protected.Group("/snmp")

		snmpGroup.GET("/v1v2c_user", readSnmpV2Users)
		snmpGroup.POST("/v1v2c_user", writeSnmpV2User)
		snmpGroup.PATCH("/v1v2c_user/:id", editSnmpV2User)
		snmpGroup.DELETE("/v1v2c_user/:id", deleteSnmpV2User)

		snmpGroup.GET("v3_user", readSnmpV3Users)
		snmpGroup.POST("v3_user", writeSnmpV3User)
		snmpGroup.PATCH("v3_user/:id", editSnmpV3User)
		snmpGroup.DELETE("v3_user/:id", deleteSnmpV3User)

		snmpGroup.GET("/info", readSnmpInfo)
		snmpGroup.PATCH("/info", writeSnmpInfo)
		snmpGroup.GET("/reset_config", resetSnmpConfig)

		protected.GET("/ntp/:prop", readNtpProperty)
		protected.POST("/ntp/:prop", writeNtpProperty)

		protected.GET("/device/:prop", readDeviceProperty)
		protected.POST("/device/:prop", writeDeviceProperty)
		protected.POST("device/serial/:cmd", writeSerialCommand)

		//protected.POST("/device/serial/:value", writeSerialCommand)

		networkGroup := protected.Group("/network")
		networkGroup.GET("/ssh", readSshStatus)
		networkGroup.PATCH("/ssh", writeSshStatus)
		networkGroup.GET("/ftp", readFtpStatus)
		networkGroup.PATCH("/ftp", writeFtpStatus)
		networkGroup.GET("/http", readHttpStatus)
		networkGroup.PATCH("/http", writeHttpStatus)
		networkGroup.GET("/telnet", readTelnetStatus)
		networkGroup.PATCH("/telnet", writeTelnetStatus)

		networkGroup.GET("/info", readNetworkInfo)
		networkGroup.POST("/info/:prop", writeNetworkInfo)
		networkGroup.POST("/reset", writeNetworkReset)

		networkGroup.GET("/access", readAllowedNodes)
		networkGroup.POST("/access", writeAllowedNodes)
		networkGroup.DELETE("/access/:id", deleteAllowedNode)
		networkGroup.POST("/access/reset", resetNetworkAccess)
		networkGroup.GET("/health", healthHandler)
		securityGroup := protected.Group("/security")

		//securityGroup.POST("/chpw", writeChangePassword)
		securityGroup.GET("/policy", readSecurityPolicy)
		securityGroup.POST("/policy", writeSecurityPolicy)
		//securityGroup.GET("/policy")

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

	//apiRouter.Run("0.0.0.0" + API_PORT) // development use localhost for nginx prod
	apiRouter.Run(AppConfig.Api.Host + ":" + AppConfig.Api.Port) // offical
}

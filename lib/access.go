package lib

// reset the network restriction, update webserver config, and xinetd.d configs
func Unrestrict() {
	InitFtpConfig()
	InitSshConfig()
	InitTelnetConfig()

	RestartXinetd()
}

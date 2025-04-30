package axi

/*

#include "axi.h"
#include "config.h"
#include "ntpServer.h"
*/
import "C"

func RunConnect() {

	C.connect()

	C.readConfig()

	println(C.readStatus())
}

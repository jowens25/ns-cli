package axi

/*

#include "serialInterface.h"
#include "axi.h"
*/
import "C"

func RunConnect() {

	C.connect()
}

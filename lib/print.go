package lib

import (
	"fmt"
	"log"
	"path/filepath"
	"runtime"
)

func Print(format string, a any) {

	_, file, line, ok := runtime.Caller(1) // Caller(1) gets the caller of MyInfo
	if !ok {
		file = "???"
		line = 0
	}
	// Use filepath.Base to get just the filename, not the full path
	log.Printf("%s:%d: "+format, filepath.Base(file), line, a)

	fmt.Printf(format+"\r\n", a)
}

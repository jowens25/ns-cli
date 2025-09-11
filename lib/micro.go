package lib

import "C"
import (
	"fmt"
	"log"
	"os"
	"strings"

	"go.bug.st/serial"
)

const MCU_ENV_VAR string = "MCU_SERIAL_PORT"

var mcu_port string
var exists bool

func init() {

	mcu_port, exists = os.LookupEnv(MCU_ENV_VAR)
	if !exists {
		os.Setenv(MCU_ENV_VAR, "/dev/ttymxc2")
		fmt.Println("set default serial port: /dev/ttymxc2")
	}
}

// command is the actual string so ex $BAUDNV
func ReadWriteMicro(command string) string {

	command = command + "\r\n"

	mode := &serial.Mode{
		BaudRate: 38400, // Adjust to match your device
		DataBits: 8,
		Parity:   serial.NoParity,
		StopBits: serial.OneStopBit,
	}

	read_data := make([]byte, 1024)

	port, err := serial.Open(mcu_port, mode)
	if err != nil {
		log.Fatal(err)
	}
	defer port.Close()

	port.ResetInputBuffer()
	port.ResetOutputBuffer()

	_, err = port.Write([]byte(command))

	fmt.Println(command)

	if err != nil {
		log.Fatal(err)
	}

	_, err = port.Read(read_data)

	if err != nil {
		log.Fatal(err)
	}

	lines := strings.Split(string(read_data), "\n")

	return lines[0]

}

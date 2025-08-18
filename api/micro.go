package api

import (
	"fmt"
	"log"
	"os"
	"syscall"
	"time"
)

func FlushSerialBuffer(f *os.File) error {
	fd := int(f.Fd())
	_, _, errno := syscall.Syscall(syscall.SYS_IOCTL, uintptr(fd),
		uintptr(0x540B), uintptr(2)) // TCFLSH ioctl
	if errno != 0 {
		return errno
	}
	return nil
}

func MakeCommand(cmd string, param ...string) []byte {

	out := []byte("$" + cmd)

	if len(param) > 0 {
		out = append(out, '=')
	}

	checksum := CalculateChecksum(out)
	out = append(out, '*')
	out = append(out, checksum...)
	out = append(out, '\r')
	out = append(out, '\n')

	return out

}

func MicroWrite(command string, responseMarker string, parameter ...string) string {

	mcu_port := "/dev/ttymxc2"

	cmd := MakeCommand(command, parameter...)

	read_data := make([]byte, 512)

	f, err := os.OpenFile(mcu_port, os.O_RDWR, 0644)

	FlushSerialBuffer(f)

	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()

	n, err := f.Write(cmd)

	time.Sleep(time.Millisecond * 50)

	if err != nil {
		log.Fatal(err)
	}

	if n > 0 {
		fmt.Println("wrote: ", n, " bytes")
	}

	n, err = f.Read(read_data)

	if err != nil {
		log.Fatal(err)
	}

	if n > 0 {
		fmt.Println("read: ", n, " bytes")
	}

	return string(read_data)
}

package lib

import "C"
import (
	"bytes"
	"fmt"

	"go.bug.st/serial"
)

func ReadWriteMicro(command string) (string, error) {
	mode := &serial.Mode{
		BaudRate: 38400,
		DataBits: 8,
		Parity:   serial.NoParity,
		StopBits: serial.OneStopBit,
	}
	mcu_port := "/dev/ttymxc2"

	port, err := serial.Open(mcu_port, mode)
	if err != nil {
		return "", fmt.Errorf("open: %w", err)
	}
	defer port.Close()

	// Clear buffers before sending
	port.ResetInputBuffer()
	port.ResetOutputBuffer()

	_, err = port.Write([]byte(command))
	if err != nil {
		return "", fmt.Errorf("write: %w", err)
	}

	var lineBuf bytes.Buffer
	buf := make([]byte, 64)
	for {
		n, err := port.Read(buf)
		if err != nil {
			return "", fmt.Errorf("read: %w", err)
		}
		if n == 0 {
			continue // or optionally break or timeout
		}
		lineBuf.Write(buf[:n])
		if bytes.Contains(buf[:n], []byte{'\n'}) {
			break
		}
	}
	return lineBuf.String(), nil
}

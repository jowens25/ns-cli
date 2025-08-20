package lib

import (
	"encoding/hex"
	"strings"
)

func CalculateChecksum(writeData []byte) string {
	var checksum byte = 0
	for i := 1; i < len(writeData); i++ {
		if writeData[i] == byte('$') {
			continue // skip the $ for this checksum calc
		}
		if writeData[i] == byte('*') {
			break
		}
		checksum ^= writeData[i]
	}
	return strings.ToUpper(hex.EncodeToString([]byte{checksum}))
}

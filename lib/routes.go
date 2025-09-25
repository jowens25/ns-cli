package lib

import (
	"fmt"
	"log"
	"os/exec"
	"strings"
)

func AddIpv4Route(i string, address string, gateway string) {

	if !HasInterface(i) {
		return
	}
	connection, _ := GetConnectionNameFromDevice(i)

	route := address + " " + gateway

	cmd := exec.Command("nmcli", "connection", "modify", connection, "+ipv4.routes", route)
	out, err := cmd.CombinedOutput()
	if err != nil {
		fmt.Println(string(out))
	}

}

func RemoveIpv4Route(i string, address string, gateway string) {
	if !HasInterface(i) {
		return
	}
	connection, _ := GetConnectionNameFromDevice(i)

	route := address + " " + gateway

	cmd := exec.Command("nmcli", "connection", "modify", connection, "-ipv4.routes", route)
	//fmt.Println("Running command:", strings.Join(cmd.Args, " "))
	out, err := cmd.CombinedOutput()
	if err != nil {
		fmt.Println(string(out))
	}

}

func ShowIpv4Routes(i string) string {
	if !HasInterface(i) {
		return "no such interface"
	}
	connection, _ := GetConnectionNameFromDevice(i)
	cmd := exec.Command("nmcli", "-f", "ipv4.routes", "con", "show", connection)
	out, err := cmd.CombinedOutput()
	if err != nil {
		log.Println(err.Error())
	}

	for line := range strings.SplitSeq(string(out), "\n") {
		fields := strings.Split(line, ":")

		if len(fields) == 2 {

			routes := strings.TrimSpace(fields[1])

			if routes == "--" {
				return routes
			}

			if strings.Contains(routes, "ip") {
				myRoutes := "subnet -> gateway\n"

				for route := range strings.SplitSeq(routes, ";") {
					route = strings.TrimSpace(route)

					route = strings.ReplaceAll(route, "{ ip = ", "")
					route = strings.ReplaceAll(route, ", nh = ", " -> ")
					route = strings.ReplaceAll(route, " }", "\n")

					myRoutes += route

				}
				return myRoutes

			}

		}
	}
	return "error"
}

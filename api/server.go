package api

import (
	"context"
	"fmt"
	"net/http"
	"os"
	"os/signal"
	"strconv"
	"syscall"
	"time"
)

var pidFile string = "/tmp/nts.pid"

func RunServers() {
	pid := os.Getpid()
	os.WriteFile(pidFile, []byte(strconv.Itoa(pid)), 0644)

	// Create separate ServeMux and http.Server for each server
	jsMux := http.NewServeMux()
	jsMux.Handle("/", http.FileServer(http.Dir("./pkg/ntscli/web")))
	jsServer := &http.Server{Addr: ":8000", Handler: jsMux}

	apiMux := http.NewServeMux()
	// NTP Server routes
	apiMux.HandleFunc("/api/v1/ntp-server/version", NtpVersionHandler)
	apiMux.HandleFunc("/api/v1/ntp-server/instance", NtpInstanceHandler)
	apiMux.HandleFunc("/api/v1/ntp-server/mac", NtpMacAddressHandler)
	apiMux.HandleFunc("/api/v1/ntp-server/vlan/addr", NtpVlanAddressHandler)
	//apiMux.HandleFunc("/api/v1/ntp-server/vlan/status", NtpVlanStatusHandler)

	// PTP OC routes
	//apiMux.HandleFunc("/ptp-oc/version", PtpVersionHandler)
	//apiMux.HandleFunc("/ptp-oc/instance", PtpInstanceHandler)
	//apiMux.HandleFunc("/ptp-oc/mac", PtpMacHandler)
	//apiMux.HandleFunc("/ptp-oc/vlan", PtpVlanHandler)

	//apiMux.HandleFunc("/posts", handler)
	apiServer := &http.Server{Addr: ":8080", Handler: corsMiddleware(apiMux)}

	go func() {
		fmt.Println("Serving static files on :8000")
		if err := jsServer.ListenAndServe(); err != nil && err != http.ErrServerClosed {
			fmt.Println("8000 error:", err)
		}
	}()

	go func() {
		fmt.Println("Serving API on :8080")
		if err := apiServer.ListenAndServe(); err != nil && err != http.ErrServerClosed {
			fmt.Println("8080 error:", err)
		}
	}()

	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)
	<-stop
	ctx, cancel := context.WithTimeout(context.Background(), 5*time.Second)
	defer cancel()
	jsServer.Shutdown(ctx)
	apiServer.Shutdown(ctx)
	fmt.Println("Servers stopped gracefully.")
	// Call Shutdown on your servers
	// e.g., apiServer.Shutdown(ctx), staticServer.Shutdown(ctx)
	fmt.Println("Servers stopped gracefully.")
}

func StopServers() {
	data, err := os.ReadFile(pidFile)
	if err != nil {
		fmt.Println("Could not read PID file:", err)
		return
	}
	pid, err := strconv.Atoi(string(data))
	if err != nil {
		fmt.Println("Invalid PID:", err)
		return
	}
	proc, err := os.FindProcess(pid)
	if err != nil {
		fmt.Println("Could not find process:", err)
		return
	}
	// Send SIGTERM (or SIGINT)
	proc.Signal(syscall.SIGTERM)
	fmt.Println("Sent stop signal to server process")
}

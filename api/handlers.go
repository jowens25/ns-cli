package api

import (
	"encoding/json"
	"net/http"

	"github.com/jowens25/axi"
)

func NtpVersionHandler(w http.ResponseWriter, r *http.Request) {
	{ // add this func to the func queue

		if r.Method != http.MethodGet {
			http.Error(w, "Method Not Allowed", http.StatusMethodNotAllowed)
			return
		}
		respondJSON(w, map[string]string{"version": axi.ReadNtpServerVersion()})
	}

}

func NtpInstanceHandler(w http.ResponseWriter, r *http.Request) {

	{

		if r.Method != http.MethodGet {
			http.Error(w, "Method Not Allowed", http.StatusMethodNotAllowed)
			return
		}
		respondJSON(w, map[string]string{"instance": axi.ReadNtpServerInstance()})
	}

}

func NtpMacAddressHandler(w http.ResponseWriter, r *http.Request) {

	{

		switch r.Method {
		case http.MethodGet:
			respondJSON(w, map[string]string{"mac": axi.ReadNtpServerMacAddress()})
		case http.MethodPost:
			// Parse POST data (could be JSON or form-data)
			type MacData struct {
				Mac string `json:"mac"`
			}
			var data MacData
			if err := json.NewDecoder(r.Body).Decode(&data); err != nil {
				http.Error(w, "Invalid JSON", http.StatusBadRequest)
				return
			}
			axi.WriteNtpServerMacAddress(data.Mac)
			// Save mac somewhere...
			respondJSON(w, map[string]string{"status": "MAC updated", "mac": data.Mac})
		default:
			http.Error(w, "Method Not Allowed", http.StatusMethodNotAllowed)
		}
	}

}

func NtpVlanAddressHandler(w http.ResponseWriter, r *http.Request) {

	println("ntp vlan addr handler....")
	switch r.Method {
	case http.MethodGet:
		respondJSON(w, map[string]string{"vlan": axi.ReadNtpServerVlanAddress()})
	case http.MethodPost:
		type VlanData struct {
			Vlan string `json:"vlan"`
		}
		var data VlanData
		if err := json.NewDecoder(r.Body).Decode(&data); err != nil {
			http.Error(w, "Invalid JSON", http.StatusBadRequest)
			return
		}
		// Save vlan...
		axi.WriteNtpServerVlanAddress(data.Vlan)

		respondJSON(w, map[string]string{"status": "VLAN updated", "vlan": data.Vlan})
	default:
		http.Error(w, "Method Not Allowed", http.StatusMethodNotAllowed)
	}

}

func respondJSON(w http.ResponseWriter, payload interface{}) {
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(payload)
}

func corsMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		origin := r.Header.Get("Origin")

		// Allowed origins list (adjust as needed)
		allowedOrigins := map[string]bool{
			"http://localhost:8000":   true,
			"http://10.1.10.205:8000": true,
			"http://10.1.10.93:29020": true,
			"http://10.1.10.93:8000":  true,
			"http://localhost:32930":  true,
			"http://localhost:55975":  true,
		}

		if allowedOrigins[origin] {
			w.Header().Set("Access-Control-Allow-Origin", origin)
		}

		w.Header().Set("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
		w.Header().Set("Access-Control-Allow-Headers", "Content-Type")

		// Handle OPTIONS preflight requests
		if r.Method == http.MethodOptions {
			w.WriteHeader(http.StatusOK)
			return
		}

		// Call the next handler
		next.ServeHTTP(w, r)
	})
}

package api

import (
	"encoding/json"
	"net/http"
	"strings"
)

// accepts axiRead and axiWrite functions, a key, and the requests
func Handler(
	axiReadFunction func(string) string,
	axiWriteFunction func(string, string),
	key string,
	readOnly bool) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		Handle(w, r, axiReadFunction, axiWriteFunction, key, readOnly)
	}
}

func Handle(
	w http.ResponseWriter,
	r *http.Request,
	axiReadFunction func(string) string,
	axiWriteFunction func(string, string),
	key string,
	readOnly bool) {

	switch r.Method {
	case http.MethodGet:
		value := axiReadFunction(key) //axi.readNtpProperty()
		w.Header().Set("Content-Type", "application/json")
		json.NewEncoder(w).Encode(map[string]string{key: value})

	case http.MethodPost:
		if readOnly {
			http.Error(w, "Property is read-only", http.StatusMethodNotAllowed)
			return
		}

		var postData map[string]string
		err := json.NewDecoder(r.Body).Decode(&postData)
		if err != nil {
			http.Error(w, "Invalid JSON", http.StatusBadRequest)
			return
		}
		newValue, ok := postData[key]
		if !ok {
			http.Error(w, "Missing key in JSON", http.StatusBadRequest)
			return
		}
		axiWriteFunction(key, newValue)
		w.Header().Set("Content-Type", "application/json")
		json.NewEncoder(w).Encode(map[string]string{key: newValue})

	default:
		http.Error(w, "Method Not Allowed", http.StatusMethodNotAllowed)
	}
}

func corsMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		origin := r.Header.Get("Origin")

		// Allowed origins list (adjust as needed)
		allowedOrigins := map[string]bool{
			"http://localhost:8000": true, // will point to go js server in production
			//"http://10.1.10.205:8000": true,
			//"http://10.1.10.93:29020": true,
			//"http://10.1.10.93:8000":  true,
			//"http://localhost:32930":  true,
			//"http://localhost:51241":  true,
		}

		if strings.HasPrefix(origin, "http://localhost:") || strings.HasPrefix(origin, "http://10.1.10.249:") {
			w.Header().Set("Access-Control-Allow-Origin", origin)
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

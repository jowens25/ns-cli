package api

//
//type PostData struct {
//	Module string `json:"module"`
//	Cmd    string `json:"command"`
//	Data   string `json:"data"`
//}
//
//func handler(w http.ResponseWriter, r *http.Request) {
//
//	origin := r.Header.Get("Origin")
//
//	// List of allowed origins (adjust as needed)
//	allowedOrigins := map[string]bool{
//		"http://localhost:8000":   true,
//		"http://10.1.10.205:8000": true,
//		"http://10.1.10.93:29020": true,
//		"http://10.1.10.93:8000":  true,
//		"http://localhost:32930":  true,
//	}
//
//	if allowedOrigins[origin] {
//		w.Header().Set("Access-Control-Allow-Origin", origin)
//	} else {
//		// Optionally reject or set no CORS header
//		http.Error(w, "Origin not allowed", http.StatusForbidden)
//		return
//	}
//	w.Header().Set("Access-Control-Allow-Methods", "POST, OPTIONS")
//	w.Header().Set("Access-Control-Allow-Headers", "Content-Type")
//
//	// Handle preflight OPTIONS request
//	if r.Method == http.MethodOptions {
//		w.WriteHeader(http.StatusOK)
//		return
//	}
//
//	if r.Method != http.MethodPost {
//		http.Error(w, "Only POST allowed", http.StatusMethodNotAllowed)
//		return
//	}
//
//	log.Println("posts handler called")
//	// Parse form data (for application/x-www-form-urlencoded)
//	if err := r.ParseForm(); err != nil {
//		http.Error(w, "Bad request", http.StatusBadRequest)
//		return
//	}
//
//	data := PostData{
//		Module: r.FormValue("module"),
//		Cmd:    r.FormValue("command"),
//		Data:   r.FormValue("data"),
//	}
//
//	//serverResponse := handleResponse(data.Module, data.Cmd, data.Data)
//
//	// Print received data
//	fmt.Printf("Received: %+v\n", data)
//
//	// Respond with JSON
//	w.Header().Set("Content-Type", "application/json")
//
//	w.WriteHeader(http.StatusOK)
//	json.NewEncoder(w).Encode(map[string]string{
//		"status": "success",
//		//"msg":    serverResponse,
//	})
//}

//func handleResponse(module string, command string, data string) string {

//axi.ReadConfig()
//
//if DeviceHasNtpServer() == 0 {
//	switch module {
//	case "ntp":
//		log.Println(data)
//		writeNtpServerStatus(data)
//		newStatus := formatNtpServerSTATUS()
//
//		log.Println(newStatus)
//		return newStatus
//	case "yourmom":
//		log.Println("your mom case")
//	default:
//		log.Println("def case")
//	}
//}
//return "your mom"
//}

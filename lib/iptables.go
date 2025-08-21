package lib

import (
	"fmt"
	"log"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
)

// Check if running as root
func checkRoot() error {
	if os.Geteuid() != 0 {
		return fmt.Errorf("this operation requires root privileges (run with sudo)")
	}
	return nil
}

// Check if command exists
func commandExists(command string) bool {
	_, err := exec.LookPath(command)
	return err == nil
}

// Enable iptables service
func enableIptablesService() error {
	// Try different service names
	services := []string{"iptables-persistent", "iptables", "netfilter-persistent"}

	for _, service := range services {
		// Check if service exists
		cmd := exec.Command("systemctl", "list-unit-files")
		out, err := cmd.CombinedOutput()
		if err == nil && strings.Contains(string(out), service) {
			// Enable the service
			cmd = exec.Command("systemctl", "enable", service)
			if err := cmd.Run(); err == nil {
				fmt.Printf("%s service enabled.\n", service)
				return nil
			}
		}
	}

	fmt.Println("Warning: No iptables service found to enable.")
	return nil
}

// IptablesInstall - Make IPv4 packet filter rules persistent
func IptablesInstall() error {
	if err := checkRoot(); err != nil {
		return err
	}

	if !commandExists("iptables-save") {
		return fmt.Errorf("iptables-save command not found")
	}

	ipv4Path := "/etc/iptables/rules.v4"

	// Create directory if it doesn't exist
	dir := filepath.Dir(ipv4Path)
	if err := os.MkdirAll(dir, 0755); err != nil {
		return fmt.Errorf("failed to create directory %s: %v", dir, err)
	}

	fmt.Printf("Saving IPv4 rules to %s...\n", ipv4Path)

	// Save iptables rules
	cmd := exec.Command("iptables-save")
	out, err := cmd.Output()
	if err != nil {
		return fmt.Errorf("failed to run iptables-save: %v", err)
	}

	// Write to file
	if err := os.WriteFile(ipv4Path, out, 0644); err != nil {
		return fmt.Errorf("failed to write rules file: %v", err)
	}

	fmt.Println("IPv4 rules saved successfully.")

	enableIptablesService()

	return nil
}

// Ip6tablesInstall - Make IPv6 packet filter rules persistent
func Ip6tablesInstall() error {
	if err := checkRoot(); err != nil {
		return err
	}

	if !commandExists("ip6tables-save") {
		return fmt.Errorf("ip6tables-save command not found")
	}

	ipv6Path := "/etc/iptables/rules.v6"

	// Create directory if it doesn't exist
	dir := filepath.Dir(ipv6Path)
	if err := os.MkdirAll(dir, 0755); err != nil {
		return fmt.Errorf("failed to create directory %s: %v", dir, err)
	}

	fmt.Printf("Saving IPv6 rules to %s...\n", ipv6Path)

	// Save ip6tables rules
	cmd := exec.Command("ip6tables-save")
	out, err := cmd.Output()
	if err != nil {
		return fmt.Errorf("failed to run ip6tables-save: %v", err)
	}

	// Write to file
	if err := os.WriteFile(ipv6Path, out, 0644); err != nil {
		return fmt.Errorf("failed to write rules file: %v", err)
	}

	fmt.Println("IPv6 rules saved successfully.")

	enableIptablesService()

	return nil
}

// IptablesRecover - Restore default IPv4 packet filter rules
func IptablesRecover() error {
	if err := checkRoot(); err != nil {
		return err
	}

	if !commandExists("iptables") {
		return fmt.Errorf("iptables command not found")
	}

	fmt.Println("Restoring default IPv4 iptables rules...")

	// Clear all rules and chains
	commands := [][]string{
		{"iptables", "-F"},                    // Flush all rules
		{"iptables", "-X"},                    // Delete user-defined chains
		{"iptables", "-t", "nat", "-F"},       // Flush NAT table
		{"iptables", "-t", "nat", "-X"},       // Delete NAT user-defined chains
		{"iptables", "-t", "mangle", "-F"},    // Flush mangle table
		{"iptables", "-t", "mangle", "-X"},    // Delete mangle user-defined chains
		{"iptables", "-P", "INPUT", "ACCEPT"}, // Set default policy
		{"iptables", "-P", "FORWARD", "ACCEPT"},
		{"iptables", "-P", "OUTPUT", "ACCEPT"},
		{"iptables", "-A", "INPUT", "-i", "lo", "-j", "ACCEPT"}, // Allow loopback
		{"iptables", "-A", "OUTPUT", "-o", "lo", "-j", "ACCEPT"},
	}

	for _, cmdArgs := range commands {
		cmd := exec.Command(cmdArgs[0], cmdArgs[1:]...)
		if out, err := cmd.CombinedOutput(); err != nil {
			// Some commands might fail on certain systems, log but continue
			log.Printf("Warning: %s failed: %s", strings.Join(cmdArgs, " "), string(out))
		}
	}

	fmt.Println("IPv4 iptables rules reset to defaults.")

	// Show current rules
	fmt.Println("Current rules:")
	cmd := exec.Command("iptables", "-L", "-n")
	if out, err := cmd.Output(); err == nil {
		fmt.Print(string(out))
	}

	return nil
}

// Ip6tablesRecover - Restore default IPv6 packet filter rules
func Ip6tablesRecover() error {
	if err := checkRoot(); err != nil {
		return err
	}

	if !commandExists("ip6tables") {
		return fmt.Errorf("ip6tables command not found")
	}

	fmt.Println("Restoring default IPv6 ip6tables rules...")

	// Clear all rules and chains
	commands := [][]string{
		{"ip6tables", "-F"},                    // Flush all rules
		{"ip6tables", "-X"},                    // Delete user-defined chains
		{"ip6tables", "-t", "mangle", "-F"},    // Flush mangle table
		{"ip6tables", "-t", "mangle", "-X"},    // Delete mangle user-defined chains
		{"ip6tables", "-P", "INPUT", "ACCEPT"}, // Set default policy
		{"ip6tables", "-P", "FORWARD", "ACCEPT"},
		{"ip6tables", "-P", "OUTPUT", "ACCEPT"},
		{"ip6tables", "-A", "INPUT", "-i", "lo", "-j", "ACCEPT"}, // Allow loopback
		{"ip6tables", "-A", "OUTPUT", "-o", "lo", "-j", "ACCEPT"},
	}

	// Try NAT table (might not exist on older kernels)
	natCommands := [][]string{
		{"ip6tables", "-t", "nat", "-F"},
		{"ip6tables", "-t", "nat", "-X"},
	}

	for _, cmdArgs := range commands {
		cmd := exec.Command(cmdArgs[0], cmdArgs[1:]...)
		if out, err := cmd.CombinedOutput(); err != nil {
			log.Printf("Warning: %s failed: %s", strings.Join(cmdArgs, " "), string(out))
		}
	}

	// Try NAT commands (may fail on systems without IPv6 NAT support)
	for _, cmdArgs := range natCommands {
		cmd := exec.Command(cmdArgs[0], cmdArgs[1:]...)
		cmd.Run() // Ignore errors for NAT table
	}

	fmt.Println("IPv6 ip6tables rules reset to defaults.")

	// Show current rules
	fmt.Println("Current rules:")
	cmd := exec.Command("ip6tables", "-L", "-n")
	if out, err := cmd.Output(); err == nil {
		fmt.Print(string(out))
	}

	return nil
}

// Helper function to check if iptables rules are currently loaded
func HasIptablesRules() (bool, error) {
	cmd := exec.Command("iptables", "-L", "-n")
	out, err := cmd.Output()
	if err != nil {
		return false, err
	}

	// Check if there are any rules beyond the default empty chains
	lines := strings.Split(string(out), "\n")
	ruleCount := 0
	for _, line := range lines {
		line = strings.TrimSpace(line)
		// Skip headers and chain policy lines
		if line != "" && !strings.HasPrefix(line, "Chain") &&
			!strings.HasPrefix(line, "target") && !strings.Contains(line, "policy") {
			ruleCount++
		}
	}

	return ruleCount > 0, nil
}

// Helper function to backup current rules before recovery
func BackupCurrentRules(backupPath string) error {
	if err := checkRoot(); err != nil {
		return err
	}

	// Create backup directory
	if err := os.MkdirAll(filepath.Dir(backupPath), 0755); err != nil {
		return fmt.Errorf("failed to create backup directory: %v", err)
	}

	// Backup IPv4 rules
	cmd := exec.Command("iptables-save")
	out, err := cmd.Output()
	if err != nil {
		return fmt.Errorf("failed to backup IPv4 rules: %v", err)
	}

	ipv4BackupPath := backupPath + ".ipv4"
	if err := os.WriteFile(ipv4BackupPath, out, 0644); err != nil {
		return fmt.Errorf("failed to write IPv4 backup: %v", err)
	}

	// Backup IPv6 rules
	cmd = exec.Command("ip6tables-save")
	out, err = cmd.Output()
	if err != nil {
		return fmt.Errorf("failed to backup IPv6 rules: %v", err)
	}

	ipv6BackupPath := backupPath + ".ipv6"
	if err := os.WriteFile(ipv6BackupPath, out, 0644); err != nil {
		return fmt.Errorf("failed to write IPv6 backup: %v", err)
	}

	fmt.Printf("Rules backed up to %s.ipv4 and %s.ipv6\n", backupPath, backupPath)
	return nil
}

// Unrestrict - Clear network access restriction table
func Unrestrict() error {
	if err := checkRoot(); err != nil {
		return err
	}

	fmt.Println("Clearing network access restriction tables...")

	// Clear IPv4 restrictions
	if err := clearIPv4Restrictions(); err != nil {
		log.Printf("Warning: Failed to clear some IPv4 restrictions: %v", err)
	}

	// Clear IPv6 restrictions
	if err := clearIPv6Restrictions(); err != nil {
		log.Printf("Warning: Failed to clear some IPv6 restrictions: %v", err)
	}

	// Clear any custom restriction chains
	if err := clearCustomRestrictionChains(); err != nil {
		log.Printf("Warning: Failed to clear some custom chains: %v", err)
	}

	fmt.Println("Network access restriction tables cleared successfully.")
	fmt.Println("All network traffic is now unrestricted.")

	return nil
}

// Clear IPv4 network restrictions
func clearIPv4Restrictions() error {
	if !commandExists("iptables") {
		return fmt.Errorf("iptables command not found")
	}

	fmt.Println("Clearing IPv4 restrictions...")

	// Commands to clear restrictions while keeping basic functionality
	commands := [][]string{
		// Set permissive default policies
		{"iptables", "-P", "INPUT", "ACCEPT"},
		{"iptables", "-P", "FORWARD", "ACCEPT"},
		{"iptables", "-P", "OUTPUT", "ACCEPT"},

		// Clear all restrictive rules but keep essential ones
		{"iptables", "-F", "INPUT"},
		{"iptables", "-F", "OUTPUT"},
		{"iptables", "-F", "FORWARD"},

		// Re-add essential loopback rules
		{"iptables", "-A", "INPUT", "-i", "lo", "-j", "ACCEPT"},
		{"iptables", "-A", "OUTPUT", "-o", "lo", "-j", "ACCEPT"},

		// Allow established and related connections
		{"iptables", "-A", "INPUT", "-m", "state", "--state", "ESTABLISHED,RELATED", "-j", "ACCEPT"},
		{"iptables", "-A", "OUTPUT", "-m", "state", "--state", "ESTABLISHED,RELATED", "-j", "ACCEPT"},
	}

	for _, cmdArgs := range commands {
		cmd := exec.Command(cmdArgs[0], cmdArgs[1:]...)
		if out, err := cmd.CombinedOutput(); err != nil {
			// Log warnings but continue - some commands might fail on different systems
			log.Printf("Warning: %s failed: %s", strings.Join(cmdArgs, " "), string(out))
		}
	}

	return nil
}

// Clear IPv6 network restrictions
func clearIPv6Restrictions() error {
	if !commandExists("ip6tables") {
		return fmt.Errorf("ip6tables command not found")
	}

	fmt.Println("Clearing IPv6 restrictions...")

	// Commands to clear IPv6 restrictions
	commands := [][]string{
		// Set permissive default policies
		{"ip6tables", "-P", "INPUT", "ACCEPT"},
		{"ip6tables", "-P", "FORWARD", "ACCEPT"},
		{"ip6tables", "-P", "OUTPUT", "ACCEPT"},

		// Clear all restrictive rules
		{"ip6tables", "-F", "INPUT"},
		{"ip6tables", "-F", "OUTPUT"},
		{"ip6tables", "-F", "FORWARD"},

		// Re-add essential loopback rules
		{"ip6tables", "-A", "INPUT", "-i", "lo", "-j", "ACCEPT"},
		{"ip6tables", "-A", "OUTPUT", "-o", "lo", "-j", "ACCEPT"},

		// Allow established and related connections
		{"ip6tables", "-A", "INPUT", "-m", "state", "--state", "ESTABLISHED,RELATED", "-j", "ACCEPT"},
		{"ip6tables", "-A", "OUTPUT", "-m", "state", "--state", "ESTABLISHED,RELATED", "-j", "ACCEPT"},
	}

	for _, cmdArgs := range commands {
		cmd := exec.Command(cmdArgs[0], cmdArgs[1:]...)
		if out, err := cmd.CombinedOutput(); err != nil {
			log.Printf("Warning: %s failed: %s", strings.Join(cmdArgs, " "), string(out))
		}
	}

	return nil
}

// Clear custom restriction chains that might exist
func clearCustomRestrictionChains() error {
	fmt.Println("Clearing custom restriction chains...")

	// Common names for restriction chains
	restrictionChains := []string{
		"RESTRICT", "BLOCK", "DENY", "DROP_CHAIN", "BLACKLIST",
		"ACCESS_CONTROL", "FIREWALL", "FILTER_CHAIN", "SECURITY",
	}

	// Clear IPv4 custom chains
	for _, chain := range restrictionChains {
		// Try to flush and delete the chain
		cmd := exec.Command("iptables", "-F", chain)
		cmd.Run() // Ignore errors - chain might not exist

		cmd = exec.Command("iptables", "-X", chain)
		cmd.Run() // Ignore errors - chain might not exist
	}

	// Clear IPv6 custom chains
	for _, chain := range restrictionChains {
		cmd := exec.Command("ip6tables", "-F", chain)
		cmd.Run() // Ignore errors

		cmd = exec.Command("ip6tables", "-X", chain)
		cmd.Run() // Ignore errors
	}

	return nil
}

// Check current restriction status
func GetRestrictionStatus() (map[string]interface{}, error) {
	status := make(map[string]interface{})

	// Check IPv4 policy
	cmd := exec.Command("iptables", "-L", "-n")
	out, err := cmd.Output()
	if err == nil {
		status["ipv4_rules"] = strings.Split(string(out), "\n")

		// Count restrictive rules (DROP, REJECT)
		restrictiveCount := strings.Count(string(out), "DROP") + strings.Count(string(out), "REJECT")
		status["ipv4_restrictive_rules"] = restrictiveCount
		status["ipv4_unrestricted"] = restrictiveCount == 0
	}

	// Check IPv6 policy
	cmd = exec.Command("ip6tables", "-L", "-n")
	out, err = cmd.Output()
	if err == nil {
		status["ipv6_rules"] = strings.Split(string(out), "\n")

		restrictiveCount := strings.Count(string(out), "DROP") + strings.Count(string(out), "REJECT")
		status["ipv6_restrictive_rules"] = restrictiveCount
		status["ipv6_unrestricted"] = restrictiveCount == 0
	}

	return status, nil
}

// Apply common unrestricted configuration
func ApplyUnrestrictedConfig() error {
	if err := checkRoot(); err != nil {
		return err
	}

	fmt.Println("Applying unrestricted network configuration...")

	// IPv4 unrestricted rules
	ipv4Commands := [][]string{
		// Allow all loopback
		{"iptables", "-A", "INPUT", "-i", "lo", "-j", "ACCEPT"},
		{"iptables", "-A", "OUTPUT", "-o", "lo", "-j", "ACCEPT"},

		// Allow all established connections
		{"iptables", "-A", "INPUT", "-m", "state", "--state", "ESTABLISHED,RELATED", "-j", "ACCEPT"},
		{"iptables", "-A", "OUTPUT", "-m", "state", "--state", "ESTABLISHED,RELATED", "-j", "ACCEPT"},

		// Allow all outgoing connections
		{"iptables", "-A", "OUTPUT", "-j", "ACCEPT"},

		// Allow common incoming services (optional - can be customized)
		{"iptables", "-A", "INPUT", "-p", "tcp", "--dport", "22", "-j", "ACCEPT"},  // SSH
		{"iptables", "-A", "INPUT", "-p", "tcp", "--dport", "80", "-j", "ACCEPT"},  // HTTP
		{"iptables", "-A", "INPUT", "-p", "tcp", "--dport", "443", "-j", "ACCEPT"}, // HTTPS

		// Allow ping
		{"iptables", "-A", "INPUT", "-p", "icmp", "-j", "ACCEPT"},
		{"iptables", "-A", "OUTPUT", "-p", "icmp", "-j", "ACCEPT"},
	}

	// IPv6 unrestricted rules
	ipv6Commands := [][]string{
		{"ip6tables", "-A", "INPUT", "-i", "lo", "-j", "ACCEPT"},
		{"ip6tables", "-A", "OUTPUT", "-o", "lo", "-j", "ACCEPT"},
		{"ip6tables", "-A", "INPUT", "-m", "state", "--state", "ESTABLISHED,RELATED", "-j", "ACCEPT"},
		{"ip6tables", "-A", "OUTPUT", "-m", "state", "--state", "ESTABLISHED,RELATED", "-j", "ACCEPT"},
		{"ip6tables", "-A", "OUTPUT", "-j", "ACCEPT"},
		{"ip6tables", "-A", "INPUT", "-p", "tcp", "--dport", "22", "-j", "ACCEPT"},
		{"ip6tables", "-A", "INPUT", "-p", "tcp", "--dport", "80", "-j", "ACCEPT"},
		{"ip6tables", "-A", "INPUT", "-p", "tcp", "--dport", "443", "-j", "ACCEPT"},
		{"ip6tables", "-A", "INPUT", "-p", "ipv6-icmp", "-j", "ACCEPT"},
		{"ip6tables", "-A", "OUTPUT", "-p", "ipv6-icmp", "-j", "ACCEPT"},
	}

	// Apply IPv4 rules
	for _, cmdArgs := range ipv4Commands {
		cmd := exec.Command(cmdArgs[0], cmdArgs[1:]...)
		if out, err := cmd.CombinedOutput(); err != nil {
			log.Printf("Warning: %s failed: %s", strings.Join(cmdArgs, " "), string(out))
		}
	}

	// Apply IPv6 rules
	for _, cmdArgs := range ipv6Commands {
		cmd := exec.Command(cmdArgs[0], cmdArgs[1:]...)
		if out, err := cmd.CombinedOutput(); err != nil {
			log.Printf("Warning: %s failed: %s", strings.Join(cmdArgs, " "), string(out))
		}
	}

	fmt.Println("Unrestricted configuration applied.")
	return nil
}

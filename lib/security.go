package lib

import (
	"fmt"
	"net/http"
	"os"
	"strings"

	"github.com/gin-gonic/gin"
)

func readSecurityPolicy(c *gin.Context) {
	var policy SecurityPolicy

	// length
	minLen, err := getMinimumPasswordLength()
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": err.Error(),
		})
		return
	}
	policy.MinimumLength = minLen

	// upper
	upperRequired, err := getUppercaseRequired()
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": err.Error(),
		})
		return
	}
	policy.RequireUpper = upperRequired

	// lower
	lowerRequired, err := getLowercaseRequired()
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": err.Error(),
		})
		return
	}
	policy.RequireLower = lowerRequired

	//number
	numberRequired, err := getNumberRequired()
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": err.Error(),
		})
		return
	}
	policy.RequireNumber = numberRequired

	//special
	specialRequired, err := getSpecialRequired()
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": err.Error(),
		})
		return
	}
	policy.RequireSpecial = specialRequired

	//no user
	noUserRequired, err := getNoUserRequired()
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": err.Error(),
		})
		return
	}
	policy.RequireNoUser = noUserRequired

	// age

	minAge, err := getMinimumPasswordAge()
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": err.Error(),
		})
		return
	}
	policy.MinimumAge = minAge

	// age
	maxAge, err := getMaximumPasswordAge()
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": err.Error(),
		})
		return
	}
	policy.MaximumAge = maxAge

	// warn
	warningAge, err := getPasswordAgeWarning()
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{
			"error": err.Error(),
		})
		return
	}
	policy.ExpirationWarning = warningAge

	// return
	c.JSON(http.StatusOK, gin.H{
		"policy": policy,
	})

}

func writeSecurityPolicy(c *gin.Context) {
	var policy SecurityPolicy

	if err := c.ShouldBindJSON(&policy); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	// length
	err := setMinimumPasswordLength(policy.MinimumLength)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	// upper
	err = setUppercaseRequired(policy.RequireUpper)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	// lower
	err = setLowercaseRequired(policy.RequireLower)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	// number
	err = setNumberRequired(policy.RequireNumber)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	// special
	err = setSpecialRequired(policy.RequireSpecial)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	// no user
	err = setNoUserRequired(policy.RequireNoUser)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	// min age
	err = setMinimumPasswordAge(policy.MinimumAge)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	// max age
	err = setMaximumPasswordAge(policy.MaximumAge)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	// warning age
	err = setPasswordAgeWarning(policy.ExpirationWarning)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	// return
	c.JSON(http.StatusOK, gin.H{
		"policy": policy,
	})

}

var pwqualityConf string = "/etc/security/pwquality.conf"
var loginConf string = "/etc/login.defs"

func openConfigFile(f string) []string {
	content, err := os.ReadFile(f)

	if err != nil {
		fmt.Printf("failed to config file %s: %v", f, err)
	}

	return strings.Split(string(content), "\n")
}

func getMinimumPasswordLength() (string, error) {

	for _, line := range openConfigFile(pwqualityConf) {

		if strings.Contains(line, "minlen") {
			parts := strings.Split(line, " = ")
			if len(parts) == 2 {

				return parts[1], nil
			}
		}
	}
	return "0", fmt.Errorf("no minlen set")
}

func setMinimumPasswordLength(minLen string) error {
	var err error = nil

	var newLines []string
	for _, line := range openConfigFile(pwqualityConf) {

		if strings.Contains(line, "minlen") {
			line = "minlen = " + minLen
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(pwqualityConf, []byte(newContent), 0644)
	return err

}

func getUppercaseRequired() (string, error) {
	for _, line := range openConfigFile(pwqualityConf) {

		if strings.Contains(line, "ucredit") {
			parts := strings.Split(line, " = ")
			if len(parts) == 2 {

				if parts[1] == "-1" {
					return "true", nil
				} else {
					return "false", nil
				}
			}
		}
	}
	return "false", fmt.Errorf("no ucredit set")
}

func setUppercaseRequired(required string) error {
	var err error = nil

	var newLines []string
	for _, line := range openConfigFile(pwqualityConf) {

		if strings.Contains(line, "ucredit") {
			if required == "true" {
				line = "ucredit = -1"

			} else {
				line = "# ucredit = -1"

			}
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(pwqualityConf, []byte(newContent), 0644)
	return err

}

func getLowercaseRequired() (string, error) {
	for _, line := range openConfigFile(pwqualityConf) {

		if strings.Contains(line, "lcredit") {
			parts := strings.Split(line, " = ")
			if len(parts) == 2 {

				if parts[1] == "-1" {
					return "true", nil
				} else {
					return "false", nil
				}
			}
		}
	}
	return "false", fmt.Errorf("no lcredit set")
}

func setLowercaseRequired(required string) error {
	var err error = nil

	var newLines []string
	for _, line := range openConfigFile(pwqualityConf) {

		if strings.Contains(line, "lcredit") {
			if required == "true" {
				line = "lcredit = -1"

			} else {
				line = "# lcredit = -1"

			}
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(pwqualityConf, []byte(newContent), 0644)
	return err

}

func getNumberRequired() (string, error) {
	for _, line := range openConfigFile(pwqualityConf) {

		if strings.Contains(line, "dcredit") {
			parts := strings.Split(line, " = ")
			if len(parts) == 2 {

				if parts[1] == "-1" {
					return "true", nil
				} else {
					return "false", nil
				}
			}
		}
	}
	return "false", fmt.Errorf("no dcredit set")
}

func setNumberRequired(required string) error {
	var err error = nil

	var newLines []string
	for _, line := range openConfigFile(pwqualityConf) {

		if strings.Contains(line, "dcredit") {
			if required == "true" {
				line = "dcredit = -1"

			} else {
				line = "# dcredit = -1"

			}
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(pwqualityConf, []byte(newContent), 0644)
	return err

}

func getSpecialRequired() (string, error) {
	for _, line := range openConfigFile(pwqualityConf) {

		if strings.Contains(line, "ocredit") {
			parts := strings.Split(line, " = ")
			if len(parts) == 2 {

				if parts[1] == "-1" {
					return "true", nil
				} else {
					return "false", nil
				}
			}
		}
	}
	return "false", fmt.Errorf("no ocredit set")
}

func setSpecialRequired(required string) error {
	var err error = nil

	var newLines []string
	for _, line := range openConfigFile(pwqualityConf) {

		if strings.Contains(line, "ocredit") {
			if required == "true" {
				line = "ocredit = -1"

			} else {
				line = "# ocredit = -1"

			}
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(pwqualityConf, []byte(newContent), 0644)
	return err

}

func getNoUserRequired() (string, error) {
	for _, line := range openConfigFile(pwqualityConf) {

		if strings.Contains(strings.TrimSpace(line), "usercheck = 1") {
			return "true", nil
		}

		if strings.Contains(strings.TrimSpace(line), "# usercheck = 1") {
			return "false", nil
		}
	}
	return "false", fmt.Errorf("no usercheck failure")
}

func setNoUserRequired(required string) error {
	var err error = nil

	var newLines []string
	for _, line := range openConfigFile(pwqualityConf) {

		if strings.Contains(line, "usercheck") {
			if required == "true" {
				line = "usercheck = 1"

			} else {
				line = "# usercheck = 1"

			}
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(pwqualityConf, []byte(newContent), 0644)
	return err

}

func getMinimumPasswordAge() (string, error) {

	for _, line := range openConfigFile(loginConf) {
		print(line)
		trimmed := strings.TrimSpace(line)
		if trimmed == "" || strings.HasPrefix(trimmed, "#") {
			continue
		}
		if strings.HasPrefix(trimmed, "PASS_MIN_DAYS") {
			print(trimmed)
			parts := strings.Fields(trimmed)
			if len(parts) >= 2 {
				return parts[1], nil
			}
		}
	}
	return "0", fmt.Errorf("no PASS_MIN_DAYS set")
}

func setMinimumPasswordAge(days string) error {
	var err error = nil

	var newLines []string
	for _, line := range openConfigFile(loginConf) {

		trimmed := strings.TrimSpace(line)
		if trimmed == "" || strings.HasPrefix(trimmed, "#") {
			continue
		}

		if strings.HasPrefix(trimmed, "PASS_MIN_DAYS") {
			line = "PASS_MIN_DAYS   " + days
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(loginConf, []byte(newContent), 0644)
	return err

}

func getMaximumPasswordAge() (string, error) {

	for _, line := range openConfigFile(loginConf) {
		print(line)
		trimmed := strings.TrimSpace(line)
		if trimmed == "" || strings.HasPrefix(trimmed, "#") {
			continue
		}
		if strings.HasPrefix(trimmed, "PASS_MAX_DAYS") {
			print(trimmed)
			parts := strings.Fields(trimmed)
			if len(parts) >= 2 {
				return parts[1], nil
			}
		}
	}
	return "0", fmt.Errorf("no PASS_MAX_DAYS set")
}

func setMaximumPasswordAge(days string) error {
	var err error = nil

	var newLines []string
	for _, line := range openConfigFile(loginConf) {

		trimmed := strings.TrimSpace(line)
		if trimmed == "" || strings.HasPrefix(trimmed, "#") {
			continue
		}

		if strings.HasPrefix(trimmed, "PASS_MAX_DAYS") {
			line = "PASS_MAX_DAYS   " + days
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(loginConf, []byte(newContent), 0644)
	return err

}

func getPasswordAgeWarning() (string, error) {

	for _, line := range openConfigFile(loginConf) {
		print(line)
		trimmed := strings.TrimSpace(line)
		if trimmed == "" || strings.HasPrefix(trimmed, "#") {
			continue
		}
		if strings.HasPrefix(trimmed, "PASS_WARN_AGE") {
			print(trimmed)
			parts := strings.Fields(trimmed)
			if len(parts) >= 2 {
				return parts[1], nil
			}
		}
	}
	return "0", fmt.Errorf("no PASS_WARN_AGE set")
}

func setPasswordAgeWarning(days string) error {
	var err error = nil

	var newLines []string
	for _, line := range openConfigFile(loginConf) {

		trimmed := strings.TrimSpace(line)
		if trimmed == "" || strings.HasPrefix(trimmed, "#") {
			continue
		}

		if strings.HasPrefix(trimmed, "PASS_WARN_AGE") {
			line = "PASS_WARN_AGE   " + days
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(loginConf, []byte(newContent), 0644)
	return err

}

package lib

import (
	"fmt"
	"log"
	"net/http"
	"os"
	"strconv"
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

func getMinimumPasswordLength() (int, error) {

	for _, line := range OpenConfigFile(AppConfig.Security.Pwquality) {

		if strings.Contains(line, "minlen") {
			//parts := strings.Split(line, "=")
			parts := strings.Fields(line)
			if len(parts) == 3 {
				minlen, err := strconv.Atoi(parts[2])
				if err != nil {
					log.Println("atoi error")
					log.Println(err.Error())
				}
				return minlen, nil
			}
		}
	}
	return 0, fmt.Errorf("no minlen set")
}

func setMinimumPasswordLength(minLen int) error {
	var err error = nil

	strMinLen := fmt.Sprintf("%d", minLen) // "%d" for decimal integer

	var newLines []string
	for _, line := range OpenConfigFile(AppConfig.Security.Pwquality) {

		if strings.Contains(line, "minlen = ") {
			line = "minlen = " + strMinLen
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(AppConfig.Security.Pwquality, []byte(newContent), 0644)
	return err

}

func getUppercaseRequired() (bool, error) {
	for _, line := range OpenConfigFile(AppConfig.Security.Pwquality) {

		if strings.HasPrefix(line, "# ucredit = ") {
			return false, nil
		}
		if strings.HasPrefix(line, "ucredit = -1") {
			return true, nil
		}

	}
	return false, fmt.Errorf("no ucredit set")
}

func setUppercaseRequired(required bool) error {
	var err error = nil

	var newLines []string
	for _, line := range OpenConfigFile(AppConfig.Security.Pwquality) {

		if strings.Contains(line, "ucredit") {
			if required {
				line = "ucredit = -1"

			} else {
				line = "# ucredit = -1"

			}
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(AppConfig.Security.Pwquality, []byte(newContent), 0644)
	return err

}

func getLowercaseRequired() (bool, error) {
	for _, line := range OpenConfigFile(AppConfig.Security.Pwquality) {

		if strings.HasPrefix(line, "# lcredit = ") {
			return false, nil
		}
		if strings.HasPrefix(line, "lcredit = -1") {
			return true, nil
		}
	}
	return false, fmt.Errorf("no lcredit set")
}

func setLowercaseRequired(required bool) error {
	var err error = nil

	var newLines []string
	for _, line := range OpenConfigFile(AppConfig.Security.Pwquality) {

		if strings.Contains(line, "lcredit") {
			if required {
				line = "lcredit = -1"

			} else {
				line = "# lcredit = -1"

			}
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(AppConfig.Security.Pwquality, []byte(newContent), 0644)
	return err

}

func getNumberRequired() (bool, error) {
	for _, line := range OpenConfigFile(AppConfig.Security.Pwquality) {

		if strings.HasPrefix(line, "# dcredit = ") {
			return false, nil
		}
		if strings.HasPrefix(line, "dcredit = -1") {
			return true, nil
		}
	}
	return false, fmt.Errorf("no dcredit set")
}

func setNumberRequired(required bool) error {
	var err error = nil

	var newLines []string
	for _, line := range OpenConfigFile(AppConfig.Security.Pwquality) {

		if strings.Contains(line, "dcredit") {
			if required {
				line = "dcredit = -1"

			} else {
				line = "# dcredit = -1"

			}
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(AppConfig.Security.Pwquality, []byte(newContent), 0644)
	return err

}

func getSpecialRequired() (bool, error) {
	for _, line := range OpenConfigFile(AppConfig.Security.Pwquality) {

		if strings.HasPrefix(line, "# ocredit = ") {
			return false, nil
		}
		if strings.HasPrefix(line, "ocredit = -1") {
			return true, nil
		}
	}
	return false, fmt.Errorf("no ocredit set")
}

func setSpecialRequired(required bool) error {
	var err error = nil

	var newLines []string
	for _, line := range OpenConfigFile(AppConfig.Security.Pwquality) {

		if strings.Contains(line, "ocredit") {
			if required {
				line = "ocredit = -1"

			} else {
				line = "# ocredit = -1"

			}
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(AppConfig.Security.Pwquality, []byte(newContent), 0644)
	return err

}

func getNoUserRequired() (bool, error) {
	for _, line := range OpenConfigFile(AppConfig.Security.Pwquality) {

		if strings.HasPrefix(line, "# usercheck = ") {
			return false, nil
		}
		if strings.HasPrefix(line, "usercheck = 1") {
			return true, nil
		}
	}
	return false, fmt.Errorf("no usercheck failure")
}

func setNoUserRequired(required bool) error {
	var err error = nil

	var newLines []string
	for _, line := range OpenConfigFile(AppConfig.Security.Pwquality) {

		if strings.Contains(line, "usercheck") {
			if required {
				line = "usercheck = 1"

			} else {
				line = "# usercheck = 1"

			}
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(AppConfig.Security.Pwquality, []byte(newContent), 0644)
	return err

}

func getMinimumPasswordAge() (int, error) {

	for _, line := range OpenConfigFile(AppConfig.Security.Login) {

		trimmed := strings.TrimSpace(line)
		if trimmed == "" || strings.HasPrefix(trimmed, "#") {
			continue
		}
		if strings.HasPrefix(trimmed, "PASS_MIN_DAYS") {

			parts := strings.Fields(trimmed)
			if len(parts) >= 2 {
				minDays, err := strconv.Atoi(parts[1])
				if err != nil {
					log.Println(err.Error())
				}
				return minDays, nil
			}
		}
	}
	return 0, fmt.Errorf("no PASS_MIN_DAYS set")
}

func setMinimumPasswordAge(days int) error {
	var err error = nil
	strDays := fmt.Sprintf("%d", days) // "%d" for decimal integer

	var newLines []string
	for _, line := range OpenConfigFile(AppConfig.Security.Login) {

		trimmed := strings.TrimSpace(line)
		if trimmed == "" || strings.HasPrefix(trimmed, "#") {
			continue
		}

		if strings.HasPrefix(trimmed, "PASS_MIN_DAYS") {
			line = "PASS_MIN_DAYS   " + strDays
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(AppConfig.Security.Login, []byte(newContent), 0644)
	return err

}

func getMaximumPasswordAge() (int, error) {

	for _, line := range OpenConfigFile(AppConfig.Security.Login) {

		trimmed := strings.TrimSpace(line)
		if trimmed == "" || strings.HasPrefix(trimmed, "#") {
			continue
		}
		if strings.HasPrefix(trimmed, "PASS_MAX_DAYS") {

			parts := strings.Fields(trimmed)
			if len(parts) == 2 {
				maxage, err := strconv.Atoi(parts[1])
				if err != nil {
					log.Println(err.Error())
				}
				return maxage, nil
			}
		}
	}
	return 0, fmt.Errorf("no PASS_MAX_DAYS set")
}

func setMaximumPasswordAge(days int) error {
	var err error = nil
	strDays := fmt.Sprintf("%d", days) // "%d" for decimal integer

	var newLines []string
	for _, line := range OpenConfigFile(AppConfig.Security.Login) {

		trimmed := strings.TrimSpace(line)
		if trimmed == "" || strings.HasPrefix(trimmed, "#") {
			continue
		}

		if strings.HasPrefix(trimmed, "PASS_MAX_DAYS") {
			line = "PASS_MAX_DAYS   " + strDays
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(AppConfig.Security.Login, []byte(newContent), 0644)
	return err

}

func getPasswordAgeWarning() (int, error) {

	for _, line := range OpenConfigFile(AppConfig.Security.Login) {
		trimmed := strings.TrimSpace(line)
		if trimmed == "" || strings.HasPrefix(trimmed, "#") {
			continue
		}
		if strings.HasPrefix(trimmed, "PASS_WARN_AGE") {
			parts := strings.Fields(trimmed)
			if len(parts) == 2 {
				age, err := strconv.Atoi(parts[1])
				if err != nil {
					log.Println(err.Error())
				}
				return age, nil
			}
		}
	}
	return 0, fmt.Errorf("no PASS_WARN_AGE set")
}

func setPasswordAgeWarning(days int) error {
	var err error = nil
	strDays := fmt.Sprintf("%d", days) // "%d" for decimal integer

	var newLines []string
	for _, line := range OpenConfigFile(AppConfig.Security.Login) {

		trimmed := strings.TrimSpace(line)
		if trimmed == "" || strings.HasPrefix(trimmed, "#") {
			continue
		}

		if strings.HasPrefix(trimmed, "PASS_WARN_AGE") {
			line = "PASS_WARN_AGE   " + strDays
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(AppConfig.Security.Login, []byte(newContent), 0644)
	return err

}

func SetPasswordEnforcement(required bool) error {
	var err error = nil

	var newLines []string
	for _, line := range OpenConfigFile(AppConfig.Security.Pwquality) {

		if strings.Contains(line, "enforcing") {
			if required {
				line = "enforcing = 1"

			} else {
				line = "# enforcing = 1"

			}
		}

		newLines = append(newLines, line)
	}

	newContent := strings.Join(newLines, "\n")
	err = os.WriteFile(AppConfig.Security.Pwquality, []byte(newContent), 0644)
	return err

}

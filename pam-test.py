import pam
import sys

def authenticate(username, password):
    p = pam.pam()
    return p.authenticate(username, password)

if __name__ == "__main__":
    username = "%s"
    password = "%s"
    
    if authenticate(username, password):
        print(username)
        sys.exit(0)
    else:
        sys.exit(1)
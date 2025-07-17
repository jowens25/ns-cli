FROM golang:1.23.10-bullseye

# Set working directory (optional)
WORKDIR /app

# Optionally copy your app's files (if you want to build something)
# COPY . .

# For demonstration, start a shell when the container runs
CMD ["bash"]

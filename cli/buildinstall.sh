go build -o ns
echo "Build complete"

rm -f /usr/bin/ns
echo "Removed old bin"

cp ns /usr/bin/ns
echo "Installed new bin"

chmod +x /usr/bin/ns
echo "Added permissions"

rm ./ns
echo "Remove local bin"

ns -h
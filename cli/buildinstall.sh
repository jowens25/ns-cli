go build -o ns
echo "Build complete"

# Prepare package folder
PKG_DIR=ns_pkg
rm -rf $PKG_DIR
mkdir -p $PKG_DIR/usr/bin
mkdir -p $PKG_DIR/DEBIAN

# Copy binary
cp ns $PKG_DIR/usr/bin/ns
chmod +x $PKG_DIR/usr/bin/ns

# Create control file
cat <<EOF > $PKG_DIR/DEBIAN/control
Package: ns
Version: 1.0.0
Section: utils
Priority: optional
Architecture: arm64
Maintainer: Your Name <youremail@example.com>
Description: ns command line tool
EOF

# Build .deb package
dpkg-deb --build $PKG_DIR

echo "Debian package created: ${PKG_DIR}.deb"

# Optionally install
#sudo dpkg -i ${PKG_DIR}.deb






rm -f /usr/bin/ns
echo "Removed old bin"

cp ns /usr/bin/ns
echo "Installed new bin"

chmod +x /usr/bin/ns
echo "Added permissions"

rm ./ns
echo "Remove local bin"

source /etc/bash_completion
source /etc/bash_completion.d/ns.bash

ns -h



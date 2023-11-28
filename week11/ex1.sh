#!/bin/bash
if [ "$(whoami)" != "root" ]; then
    echo "You must run this script with root privelegies"
    echo "sudo ./ex1.sh"
    exit 1
fi

gcc ex1.c -o ex1 -Wextra -Wpedantic
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Create a file lofs.img of size 50 MiB using dd
dd if=/dev/zero of=lofs.img bs=1M count=50

# Setup a loop device on the created file
LOOP_DEVICE=$(losetup -fPL --show lofs.img)

# Create an ext4 filesystem on the loop device
mkfs -t ext4 ${LOOP_DEVICE}

# Create a new directory for mounting the loop filesystem
mkdir -p ./lofsdisk

# Mount the filesystem on the directory
mount ${LOOP_DEVICE} ./lofsdisk

# Change to the mounted directory
cd ./lofsdisk

# Create two files, file1 and file2, with your first and last name
echo 'Danil' > file1
echo 'Nesterov' > file2

# Define a function to return the paths of shared libraries of a binary
get_libs() {
    ldd $1 | tr -s '[:space:]' | cut -d ' ' -f 3 | sed '/^$/d'
}

# Copy shared libraries for specified binaries to the LOFS
for CMD in /bin/bash /bin/cat /bin/echo /bin/ls /bin/touch /bin/nano
do
    BIN_PATH=$(which ${CMD})
    BIN_DIR=$(dirname ${CMD})
    mkdir -p .${BIN_DIR}
    cp ${BIN_PATH} .${BIN_PATH}
    for LIB in $(get_libs ${BIN_PATH})
    do
        LIB_DIR=$(dirname ${LIB})
        mkdir -p .${LIB_DIR}
        cp ${LIB} .${LIB_DIR}
    done
done

# Exit the lofsdisk directory before chroot
cd ..

# Change the root directory to the mount point
chroot ./lofsdisk

# Run the C program and save output to ex1.txt
echo "$PWD"
ls -la
/bin/bash ex1 > ex1.txt

# Exit chroot environment
exit

# Change back to the lofsdisk directory
cd ./lofsdisk

# Run the C program again and append output to ex1.txt
./ex1 >> ex1.txt

# Exit the lofsdisk directory
cd ..

# Compare outputs and add findings to ex1.txt
echo "Differences in outputs:" >> ./lofsdisk/ex1.txt
diff <(sed -n '/^\/$/,/^exit$/p' ./lofsdisk/ex1.txt) <(sed -n '/^exit$/,/^Differences$/p' ./lofsdisk/ex1.txt) >> ./lofsdisk/ex1.txt

# Cleanup: unmount and delete the loop device and image file
sudo umount ./lofsdisk
sudo losetup -d ${LOOP_DEVICE}
sudo rm -rf ./lofsdisk lofs.img ex1
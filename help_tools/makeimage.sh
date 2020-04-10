#! /bin/sh

IMG_NAME="OrangePiZero.img"

# Create a new disk image: 128MB
dd if=/dev/zero of=$IMG_NAME bs=1M count=128

# Partition the image, part1: 8MB@FAT32, part2:@EXT4
echo 'n\np\n1\n2048\n18432\nt\nb\nn\np\n2\n18433\n\nw\n' |fdisk $IMG_NAME > /dev/null

fdisk -l $IMG_NAME |tail -n 3

# Mapping the image to host system
sudo kpartx -av $IMG_NAME
[ $? -ne 0 ] && echo "Please use right root password." && exit 1

sleep 0.1s

# Format the partion: part1@FAT32
sudo mkfs.vfat /dev/mapper/loop0p1

# Format the partion: part2@EXT4
sudo mkfs.ext4 /dev/mapper/loop0p2

sleep 0.1s

# Change the label of partion: part1@BOOT
sudo dosfslabel /dev/mapper/loop0p1 "BOOT"

# Change the label of partion: part2@Linux
sudo e2label /dev/mapper/loop0p2 "Linux"

# Create mountpoints
sudo mkdir -p /mnt/BOOT
sudo mkdir -p /mnt/Linux

# Mount partions
sudo mount -o loop -t vfat /dev/mapper/loop0p1 /mnt/BOOT
sudo mount -o loop -t ext4 /dev/mapper/loop0p2 /mnt/Linux

# Copy files to image partions
sudo cp -v zImage sun8i-h2-plus-orangepi-zero.dtb /mnt/BOOT
sudo cp -v -r rootfs/* /mnt/Linux/
sync

# Unmount partions
sudo umount -l /mnt/BOOT
sudo umount -l /mnt/Linux

# Delete mountpoints
sudo rm -rf /mnt/BOOT
sudo rm -rf /mnt/Linux

# Install u-boot to image
sudo dd if=u-boot-sunxi-with-spl.bin of=/dev/loop0 bs=1024 seek=8

# Unmap the image from host system
sudo kpartx -d $IMG_NAME
[ $? -ne 0 ] && echo "Please use right root password." && exit 1

echo "Done!"

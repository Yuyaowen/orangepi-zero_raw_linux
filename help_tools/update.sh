#! /bin/sh

# Set PATHs
UBOOT_BIN='../u-boot-2019.10'
ZIMAGE_BIN='../linux-5.6.2/arch/arm/boot'
BUSYBOX_INSTALL='../busybox-1.31.1/_install'
CROSS_COMPILE_PATH='../../programs/gcc-linaro-7.4.1-2019.02-i686_arm-linux-gnueabihf/arm-linux-gnueabihf'
UTILS_PATH='../utils'

cp -rfv ${UBOOT_BIN}/u-boot-sunxi-with-spl.bin ./
cp -rfv ${ZIMAGE_BIN}/zImage ./
cp -rfv ${ZIMAGE_BIN}/dts/sun8i-h2-plus-orangepi-zero.dtb ./
cp -rfv ${BUSYBOX_INSTALL}/* ./rootfs/
cp -rfv ${CROSS_COMPILE_PATH}/lib/*.so* ./rootfs/lib/
cp -rfv ${CROSS_COMPILE_PATH}/libc/lib/*.so* ./rootfs/lib/

cp -rfv ${UTILS_PATH}/e2fsprogs/_install/* ./rootfs/

./makeimage.sh

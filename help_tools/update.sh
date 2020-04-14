#! /bin/sh

LINUX_VER="5.6.2"

# Set PATHs
LINUX_KERNEL='../linux-5.6.2'
UBOOT_BIN='../u-boot-2019.10'
ZIMAGE_BIN="${LINUX_KERNEL}/arch/arm/boot"
BUSYBOX_INSTALL='../busybox-1.31.1/_install'
CROSS_COMPILE_PATH='../../programs/gcc-linaro-7.4.1-2019.02-i686_arm-linux-gnueabihf/arm-linux-gnueabihf'
UTILS_PATH='../utils'

cp -rfv ${UBOOT_BIN}/u-boot-sunxi-with-spl.bin ./
cp -rfv ${ZIMAGE_BIN}/zImage ./
cp -rfv ${ZIMAGE_BIN}/dts/sun8i-h2-plus-orangepi-zero.dtb ./
cp -rfv ${BUSYBOX_INSTALL}/* ./rootfs/
cp -rfv ${CROSS_COMPILE_PATH}/lib/*.so* ./rootfs/lib/
cp -rfv ${CROSS_COMPILE_PATH}/libc/lib/*.so* ./rootfs/lib/

cp -rfv ${UTILS_PATH}/e2fsprogs-1.45.6/_install/sbin/resize2fs ./rootfs/sbin/

# Start: For WiFi
mkdir -p ./rootfs/lib/firmware/xr819
cp -v ./xr819_firmware/* ./rootfs/lib/firmware/xr819

mkdir -p ./rootfs/lib/modules/"${LINUX_VER}"
cp -v "${LINUX_KERNEL}/drivers/net/wireless/xradio/xradio_wlan.ko" ./rootfs/lib/modules/"${LINUX_VER}"/

cp -v ${UTILS_PATH}/wpa_supplicant-2.9/wpa_supplicant/wpa_supplicant ./rootfs/bin/
cp -v ${UTILS_PATH}/wpa_supplicant-2.9/wpa_supplicant/wpa_passphrase ./rootfs/bin/
cp -v ${UTILS_PATH}/wpa_supplicant-2.9/wpa_supplicant/wpa_cli ./rootfs/bin/

cp -v ${UTILS_PATH}/libnl-3.2.25/_install/lib/libnl-3.so.200 ./rootfs/lib/
cp -v ${UTILS_PATH}/libnl-3.2.25/_install/lib/libnl-3.so.200.20.0 ./rootfs/lib/
cp -v ${UTILS_PATH}/libnl-3.2.25/_install/lib/libnl-genl-3.so.200 ./rootfs/lib/
cp -v ${UTILS_PATH}/libnl-3.2.25/_install/lib/libnl-genl-3.so.200.20.0 ./rootfs/lib/

cp -v ${UTILS_PATH}/openssl-1.1.1f/_install/lib/libssl.so.1.1 ./rootfs/lib/
cp -v ${UTILS_PATH}/openssl-1.1.1f/_install/lib/libcrypto.so.1.1 ./rootfs/lib/
# End: For WiFi

./makeimage.sh

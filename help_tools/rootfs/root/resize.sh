#! /bin/sh

cat << EOF > /tmp/partargs.txt
d
2
n
p
2
18433

w
EOF

#echo 'd\n2\nn\np\n2\n18433\n\nw\n' |fdisk /dev/mmcblk0 > /dev/null
fdisk /dev/mmcblk0 < /tmp/partargs.txt

sed -i '/#Remove this line/d' /etc/init.d/rcS
sync

echo "resize2fs /dev/mmcblk0p2 #Remove this line" >> /etc/init.d/rcS
echo "rm $0 #Remove this line" >> /etc/init.d/rcS
echo "sed -i '/#Remove this line/d' /etc/init.d/rcS" >> /etc/init.d/rcS
sync

reboot -f

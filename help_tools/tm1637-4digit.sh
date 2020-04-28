#! /bin/sh

# The MT1637 is not use the fucking standard I2C interface!
# Don't connect it to I2C bus, it will broke the bus!
# And the busybox's ash don't support for fucking array!

# Define pins:
clk_pin_num=199
dio_pin_num=198

init_pins()
{
	echo "${clk_pin_num}" >> /sys/class/gpio/export 2> /dev/null
	echo "out" >> /sys/class/gpio/gpio${clk_pin_num}/direction
	echo "1" >> /sys/class/gpio/gpio${clk_pin_num}/value

	echo "${dio_pin_num}" >> /sys/class/gpio/export 2> /dev/null
	echo "out" >> /sys/class/gpio/gpio${dio_pin_num}/direction
	echo "1" >> /sys/class/gpio/gpio${dio_pin_num}/value
}

set_dio_dir_out()
{
	echo "out" >> /sys/class/gpio/gpio${dio_pin_num}/direction
}

set_dio_dir_in()
{
	echo "in" >> /sys/class/gpio/gpio${dio_pin_num}/direction
}

# Using: "set_clk 1" or "set_clk 0"
set_clk()
{
	local val="$1"

	echo ${val} >> /sys/class/gpio/gpio${clk_pin_num}/value
}

set_dio()
{
	local val="$1"

	echo ${val} >> /sys/class/gpio/gpio${dio_pin_num}/value
}

get_dio()
{
	cat /sys/class/gpio/gpio${dio_pin_num}/value
}

tm1637_start()
{
	set_dio_dir_out

	set_clk 1
	set_dio 1
	usleep 2
	set_dio 0
}

tm1637_ack()
{
	set_clk 0
	set_dio_dir_in
	usleep 10
	set_clk 1
	usleep 2
	set_clk 0
	set_dio_dir_out
}

tm1637_stop()
{
	set_dio_dir_out
	
	set_clk 0
	usleep 2
	set_dio 0
	usleep 2
	set_clk 1
	usleep 2
	set_dio 1
}

# Using: "tm1637_write_byte 0xff"
tm1637_write_byte()
{
	local dat=$1
	
	set_dio_dir_out

	local i=
	for i in $(seq 0 7)
	do
		set_clk 0
		set_dio $((dat & 0x01))
		dat=$((dat >> 1))
		set_clk 1
	done
}

#num_code="0x3f 0x06 0x5b 0x4f 0x66 0x6d 0x7d 0x07 0x7f 0x6f 0x77 0x7c 0x39 0x5e 0x79 0x71"
point_code=0x80
code_convert()
{
	local num=$1

	if [ $num -eq 0 ]; then
		echo 0x3f
	elif [ $num -eq 1 ]; then
		echo 0x06
	elif [ $num -eq 2 ]; then
		echo 0x5b
	elif [ $num -eq 3 ]; then
		echo 0x4f
	elif [ $num -eq 4 ]; then
		echo 0x66
	elif [ $num -eq 5 ]; then
		echo 0x6d
	elif [ $num -eq 6 ]; then
		echo 0x7d
	elif [ $num -eq 7 ]; then
		echo 0x07
	elif [ $num -eq 8 ]; then
		echo 0x7f
	elif [ $num -eq 9 ]; then
		echo 0x6f
	fi
}

# Usage: tm1637_display_time <HH> <MM> <show_second_flag(0/1)>
tm1637_display_time()
{
	local hour=$1
	local min=$2
	local flag=$3
	local data=

	tm1637_start
	tm1637_write_byte 0x44
	tm1637_ack
	tm1637_stop

	############# 1st number #############
	tm1637_start
	tm1637_write_byte 0xc0 # 1st addr
	tm1637_ack

	data=`expr $hour / 10`
	local code=`code_convert $data`
	tm1637_write_byte $code
	tm1637_ack
	tm1637_stop

	############# 2st number #############
	tm1637_start
	tm1637_write_byte 0xc1 # 2st addr
	tm1637_ack

	data=`expr $hour % 10`
	local code=`code_convert $data`
	[ $flag -eq 1 ] && code=$(($code | 0x80))
	tm1637_write_byte $code
	tm1637_ack
	tm1637_stop

	############# 3st number #############
	tm1637_start
	tm1637_write_byte 0xc2 # 3st addr
	tm1637_ack

	data=`expr $min / 10`
	local code=`code_convert $data`
	tm1637_write_byte $code
	tm1637_ack
	tm1637_stop

	############# 4st number #############
	tm1637_start
	tm1637_write_byte 0xc3 # 4st addr
	tm1637_ack

	data=`expr $min % 10`
	local code=`code_convert $data`
	tm1637_write_byte $code
	tm1637_ack
	tm1637_stop

	tm1637_start
	tm1637_write_byte 0x8f
	tm1637_ack
	tm1637_stop
}

echo "Start time: `date`"
init_pins
while true
do
	hour=`date "+%H"`
	min=`date "+%M"`
	tm1637_display_time $hour $min 1
	sleep 0.3s
	tm1637_display_time $hour $min 0
	sleep 0.7s
done

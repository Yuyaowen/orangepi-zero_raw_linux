#! /bin/sh

# The MT1637 is not use the fucking standard I2C interface!
# Don't connect it to I2C bus, it will broke the bus!
# And the busybox's ash don't support for fucking array!

######################### Start: For GPIO #############################
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
######################### End: For GPIO #############################

##################### Start: For Sequential #########################
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

# Using: "tm1637_write_byte <Byte>"
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
###################### End: For Sequential ##########################

#################### Start: For Chip Driver #########################
tm1637_init()
{
	init_pins

	tm1637_start
	tm1637_write_byte 0x44
	tm1637_ack
	tm1637_stop

	tm1637_start
	tm1637_write_byte 0x8f
	tm1637_ack
	tm1637_stop
}

# Usage: tm1637_show_char <char_code> <position(0~3)>
tm1637_show_char()
{
	local char_code=$1
	local position=$2

	tm1637_start
	tm1637_write_byte 0xc$position 
	tm1637_ack

	tm1637_write_byte $char_code
	tm1637_ack
	tm1637_stop
}
#################### End: For Chip Driver #########################

#num_code="0x3f 0x06 0x5b 0x4f 0x66 0x6d 0x7d 0x07 0x7f 0x6f 0x77 0x7c 0x39 0x5e 0x79 0x71"
point_code=0x80
# Usage: code_convert <0~9>
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

# Usage: display_face <wait_time>
display_face()
{
	local wait=$1

	tm1637_show_char 0x00 0 # 1st dark
	tm1637_show_char 0xe3 1 # 2st o
	tm1637_show_char 0xe3 2 # 3st o
	tm1637_show_char 0x00 3 # 4st dark
	sleep $wait

	tm1637_show_char 0x00 0 # 1st dark
	tm1637_show_char 0x40 1 # 2st -
	tm1637_show_char 0x40 2 # 3st -
	tm1637_show_char 0x00 3 # 4st dark
	sleep $wait
}

# Usage: display_time <HH> <MM> <show_second_flag(0/1)>
display_time()
{
	local hour=$1
	local min=$2
	local flag=$3
	local data=

	data=`expr $hour / 10`
	local code=`code_convert $data`
	tm1637_show_char $code 0 # 1st

	data=`expr $hour % 10`
	local code=`code_convert $data`
	[ $flag -eq 1 ] && code=$(($code | 0x80))
	tm1637_show_char $code 1 # 2st

	data=`expr $min / 10`
	local code=`code_convert $data`
	tm1637_show_char $code 2 # 3st

	data=`expr $min % 10`
	local code=`code_convert $data`
	tm1637_show_char $code 3 # 4st
}

#echo "Start time: `date`"
tm1637_init
for i in `seq 1 5`
do
	display_face 1
done

while true
do
	hour=`date "+%H"`
	min=`date "+%M"`
	sec=`date "+%S"`
	if [ $sec -lt 55 ]; then
		display_time $hour $min 1
		sleep 0.3s
		display_time $hour $min 0
		sleep 0.7s
	else
		display_face 1
	fi
done

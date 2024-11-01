#!/usr/bin/python3.9

# Map: https://www.spikenzielabs.com/learn/images/new4x20-580.png
# Tutorial: https://www.circuitbasics.com/raspberry-pi-i2c-lcd-set-up-and-programming/
# Charachters: https://omerk.github.io/lcdchargen/
# Driver: I2C_LCD_driver.py

import I2C_LCD_driver;
driver = I2C_LCD_driver.lcd();
driver.lcd_clear();
driver.backlight(0);

# Register exit handler while backlight off
from atexit import register;
from time import sleep;
def exit_handler():
	driver.lcd_clear();
	driver.lcd_display_string("Exiting...", 2, 4);
	sleep(1);
	driver.lcd_clear();
	driver.backlight(0);
register(exit_handler);

# Start
driver.backlight(1);
driver.lcd_display_string("Starting...", 1, 4);
sleep(1);

# Load needed data and modules (Fontdata: 0 is clock, 1-5 is spinny, 6 is connections)
fontdata = [
      [ 0b00000,
	0b01110,
	0b10101,
	0b10111,
	0b10001,
	0b01110,
	0b00000,
	0b00000 ],
      [ 0b01100,
	0b01100,
	0b00101,
	0b11011,
	0b11011,
	0b10100,
	0b00110,
	0b00110 ],
      [ 0b11000,
	0b01101,
	0b00101,
	0b01011,
	0b11010,
	0b10100,
	0b10110,
	0b00011 ],
      [ 0b11001,
	0b11011,
	0b01111,
	0b01010,
	0b01010,
	0b11110,
	0b11011,
	0b10011 ],
      [ 0b00011,
	0b10110,
	0b10100,
	0b11010,
	0b01011,
	0b00101,
	0b01101,
	0b11000 ],
      [ 0b00110,
	0b00110,
	0b10100,
	0b11011,
	0b11011,
	0b00101,
	0b01100,
	0b01100 ],
      [ 0b00000,
	0b00011,
	0b00000,
	0b00111,
	0b00000,
	0b01111,
	0b00000,
	0b11111 ],
];
driver.lcd_load_custom_chars(fontdata);	# Load fontdata
from psutil import sensors_temperatures, disk_usage, virtual_memory, cpu_percent;
from time import strftime;
from subprocess import run, PIPE;

# Prepare special charachters and spinny icons
driver.lcd_clear();
driver.lcd_write(0x94);	# Location, see map
driver.lcd_write_char(0);	# Fontdata 0, clock
driver.lcd_write(0x91);	# Location, see map
driver.lcd_write_char(6);	# Fontdata 6, connections

# Start the animation cycle
animation = 1;

# Main loop, exits with exit_handler()
while True:
	# Line 1
	driver.lcd_display_string("%05.2f" %sensors_temperatures(fahrenheit=False)['cpu_thermal'][0].current, 1, 0);
	driver.lcd_display_string(chr(223), 1, 5);
	driver.lcd_display_string("C", 1, 6);
	driver.lcd_display_string("%s" %strftime("%H:%M:%S"), 1, 8);
	driver.lcd_display_string("%02.0f" %float(run("netstat -an | grep ESTABLISHED | wc -l", stdout=PIPE, universal_newlines=True, shell=True).stdout.rstrip()), 1, 18);

	# Line 2
	driver.lcd_display_string("CPU %05.2f " %cpu_percent(), 2, 0);
	driver.lcd_display_string("RAM %05.2f" %virtual_memory().percent, 2, 11);

	# Line 3
	driver.lcd_display_string("%s" %run("uptime | awk -F'( |,|:)+' '{d=h=m=0; if ($7==\"min\") m=$6; else {if ($7~/^day/) {d=$6;h=$8;m=$9} else {h=$6;m=$7}}} {print d+0,\"D \",h+0,\"H \",m+0,\"M  \"}'", stdout=PIPE, universal_newlines=True, shell=True).stdout.rstrip(), 3, 2);

	# Line 4
	### YOUR TURN! Implement something fun :)
	driver.lcd_display_string("%s" %('Ofline' if "inactive" in run("systemctl is-active networking.service", stdout=PIPE, universal_newlines=True, shell=True).stdout.rstrip() else 'Online'), 4, 0);	# Typo intentional for space
	driver.lcd_display_string("Disk %05.2f" %(disk_usage('/').used / disk_usage('/').total * 100), 4, 10);

	# Special charachters and fan icon
	driver.lcd_write(0xDB);	# Location, see map
	if not "inactive" in run("systemctl is-active networking.service":	# Spinny symbol on
		driver.lcd_write_char(animation);
		animation += 1;
		if (animation == 6):
			animation = 1;
	else:
		driver.lcd_display_string("X", 4, 7);	# Spinny symbol off

	# Sleep and restart loop
	sleep(1);

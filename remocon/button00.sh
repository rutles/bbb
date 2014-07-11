#!/bin/bash
# button00.sh
echo timer > /sys/class/leds/beaglebone:green:usr3/trigger
echo 100 > /sys/class/leds/beaglebone:green:usr3/delay_on
echo 100 > /sys/class/leds/beaglebone:green:usr3/delay_off
echo 1 > /sys/class/leds/beaglebone:green:usr3/brightness


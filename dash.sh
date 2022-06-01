#!/usr/bin/bash

sudo modprobe can
sudo modprobe can-raw
sudo modprobe slcan

sudo slcand -o -c -s8 /dev/ttyACM0 can0
sudo ifconfig can0 up

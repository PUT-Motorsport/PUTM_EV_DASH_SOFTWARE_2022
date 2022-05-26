#!/usr/bin/bash

sudo slcan_attach -f -s8 -o /dev/ttyACM0
sudo slcand ttyACM0 slcan0
sudo ifconfig slcan0 up

startx #run the gui environment

cd ~/Dash

./Dash

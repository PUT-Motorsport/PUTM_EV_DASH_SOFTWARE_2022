#!/usr/bin/bash
[ "$UID" -eq 0 ] || exec sudo bash "$0" "$@"
modprobe vcan
ip link add dev vcan0 type vcan
ip link set up vcan0
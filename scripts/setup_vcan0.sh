#!/bin/bash

set -e

/usr/sbin/modprobe vcan

if ! /usr/sbin/ip link show vcan0 > /dev/null 2>&1;then
/usr/sbin/ip link add dev vcan0 type vcan
fi

/usr/sbin/ip link set up vcan0

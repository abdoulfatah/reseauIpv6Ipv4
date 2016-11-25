#! /bin/bash

sudo ip addr add 172.16.2.1/28 dev tun0

sudo ip link set tun0 up



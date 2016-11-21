#! /bin/bash

sudo ip addr add 172.16.2.1/28 dev tun0

sudo ip link set tun0 up

sudo ip route add 172.16.2.176/28 via 172.16.2.10


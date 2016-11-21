#! /bin/bash

sudo ip addr add 172.16.2.10/28 dev tun3

sudo ip link set tun3 up

sudo ip route add 172.16.2.144/28 via 172.16.2.1


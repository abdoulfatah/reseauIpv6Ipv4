#! /bin/bash

# C'est un script vierge à personnaliser pour la VM en question (VM1-6
# ou VM3-6). Ne pas oublier le petit chmod qui va bien !

# Ajout d'une adresse IPv6 pour l'interface tun0
#    sudo ip addr add fc00:1234::1/64 dev tun0
#    sudo ip link set tun0 up

# Creation d'une route par le tunnel pour atteindre l'autre réseau
#    sudo ip -6 route add fc00:1234::10/64 dev "$1"
#                    \              /
#                   adresse a atteindre


#sudo sysctl -w net.ipv6.conf.all.forwarding=1

#sudo /sbin/ifconfig "$1" inet6 add fc00:1234:ffff::1/64

#sudo ip link set "$1" up

#sudo ip -6 route add fc00:1234:2::/64 via fc00:1234:ffff::10 dev "$1"

#sudo /sbin/ifconfig "$1" inet add 172.16.2.10/28

#sudo ip link set "$1" up

#sudo ip route add 172.16.2.144 via 172.16.2.10 dev "$1"


sudo ip addr add 172.16.2.1/28 dev $1
sudo ip link set $1 up
sudo ip route add 172.16.2.176/28 via 172.16.2.1 dev $1








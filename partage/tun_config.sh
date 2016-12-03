#! /bin/bash

# C'est un script vierge à personnaliser pour la VM en question (VM1-6
# ou VM3-6). Ne pas oublier le petit chmod qui va bien !

# Ajout d'une adresse IPv6 pour l'interface tun0
sudo ip addr add fc00:1234::1/64 dev tun0
sudo ip link set tun0 up

# Creation d'une route par le tunnel pour atteindre l'autre réseau
sudo ip -6 route add fc00:1234:3::/64 via fc00:1234:ffff::10 dev "$1"
#                    \              /
#                   adresse a atteindre




#! /bin/bash

# C'est un script vierge à personnaliser pour la VM en question (VM1-6
# ou VM3-6). Ne pas oublier le petit chmod qui va bien !

# Ajout d'une adresse IPv6 pour l'interface tun0
sudo sysctl -w net.ipv4.ip_forward=1
sudo ip addr add 172.16.2.1 dev tun0
sudo ip link set tun0 up

# Creation d'une route par le tunnel pour atteindre l'autre réseau
sudo ip route add 172.16.2.144/28 dev "$1"
#                    \             /
#                  adresse a atteindre




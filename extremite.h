#ifndef EXTREMITE_H
#define EXTREMITE_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define MAXLIGNE 80
#define CIAO "Au revoir ...\n"
#define BUFFER_SIZE_MAX 1024


int tun_alloc(char *dev);
void tun_copy (int src, int dst);

/*
Cree un serveur ecoutant sur le port <port> et redirige les donnees recu
sur la sortie standard.
*/
void ext_out(char * port, int tun);

/*
Ouvre une connexion TCP avec l'autre extremite du tunnel, puis lit le
traffic dans tun0 et le retransmet dans la socket.
*/
void ext_in(char * hote, char * port, int tun);


#endif // EXTREMITE_H

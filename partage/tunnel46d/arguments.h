/*
Reconnait le format suivant dans un fichier dont le nom est donné en argument.

  # interface tun
  tun=
  tunip=
  # adresse locale
  inip=
  inport=
  # adresse distante
  outip=
  outport=
*/

#ifndef ARGUMENTS
#define ARGUMENTS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_READ_BUFFER        128
#define ARGUMENTS_IP_STR_SIZE   32
#define ARGUMENTS_PORT_STR_SIZE 16
#define ARGUMENTS_TUN_STR_SIZE  32

typedef struct
{
    char arg_tun[ARGUMENTS_TUN_STR_SIZE];
    char arg_tunip[ARGUMENTS_IP_STR_SIZE];
    char arg_inip[ARGUMENTS_IP_STR_SIZE];
    char arg_inport[ARGUMENTS_PORT_STR_SIZE];
    char arg_outip[ARGUMENTS_IP_STR_SIZE];
    char arg_outport[ARGUMENTS_PORT_STR_SIZE];
} Arguments;

// split les arguments autour d'un '='
// considere que la longueur des chaines est de SIZE_READ_BUFFER
void splitArg(char * src, char ** arg_name, char ** arg_value);

// parse les arguments du programmes dans la structure Arguments
void parseArguments(int argc, char ** argv, Arguments * arg);

#endif // ARGUMENTS

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

#define SIZE_READ_BUFFER 64


typedef struct
{
    char arg_tun[32];
    char arg_tunip[16];
    char arg_inip[16];
    int arg_inport;
    char arg_outip[16];
    int arg_outport;
} Arguments;

// split les arguments autour d'un '='
// considere que la longueur des chaines est de SIZE_READ_BUFFER
void splitArg(char * src, char ** arg_name, char ** arg_value);

// parse les arguments du programmes dans la structure Arguments
void parseArguments(int argc, char ** argv, Arguments * arg);

#endif // ARGUMENTS

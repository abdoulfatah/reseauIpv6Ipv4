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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_READ_BUFFER 64

char arg_tun[32];
char arg_tunip[16];
char arg_inip[16];
int arg_inport;
char arg_outip[16];
int arg_outport;

// split les arguments autour d'un '='
// considere que la longueur des chaines est de SIZE_READ_BUFFER
void splitArg(char * src, char ** arg_name, char ** arg_value);

// parse les arguments du programmes dans les variables globales prevues
void parseArguments(int argc, char ** argv);

int main(int argc, char ** argv)
{
    parseArguments(argc, argv);

    return 0;
}

void splitArg(char * src, char ** arg_name, char ** arg_value)
{
    int i, j;
    (*arg_name)[0] = (*arg_value)[0] = '\0';
    for(i=0 ; src[i]!='=' && src[i]!=' ' && src[i]!='\n' && i<(SIZE_READ_BUFFER-1) ; i++)
        (*arg_name)[i] = src[i];
    (*arg_name)[i] = '\0';
    for(j=0, i++ ; src[i]!=' ' && src[i]!='\n' && i<(SIZE_READ_BUFFER-1) ; j++, i++)
        (*arg_value)[j] = src[i];
    (*arg_value)[j] = '\0';
}

void parseArguments(int argc, char ** argv)
{
    char * filename;

    size_t size_read = 0;
    char *strtmp;

    // vérification du nombre d'argument
    if(argc != 2)
    {
        fprintf(stderr, "Erreur : mauvais nombre d'argument\n");
        fprintf(stderr, "\tusage : %s <fichier>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    filename = argv[1];

    // ouverture du fichier
    FILE * f_in = fopen(filename, "r");
    if(f_in == NULL)
    {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier \"%s\"\n", filename);
        exit(EXIT_FAILURE);
    }

    // alloc du buffer de lecture
    strtmp = malloc(sizeof(char) * SIZE_READ_BUFFER);

    // lecture
    char * argname = malloc(sizeof(char) * SIZE_READ_BUFFER);
    char * argvalue = malloc(sizeof(char) * SIZE_READ_BUFFER);

    int i;
    while(getline(&strtmp, &size_read, f_in) != -1)
    {
        if(strtmp[0] == '#') // ligne de commentaire
            continue;
        splitArg(strtmp, &argname, &argvalue);
        if(strcmp(argname, "tun") == 0)
            sprintf(arg_tun, "%s", argvalue);
        else if(strcmp(argname, "tunip") == 0)
            sprintf(arg_tunip, "%s", argvalue);
        else if(strcmp(argname, "inip") == 0)
            sprintf(arg_inip, "%s", argvalue);
        else if(strcmp(argname, "inport") == 0)
            arg_inport = atoi(argvalue);
        else if(strcmp(argname, "outip") == 0)
            sprintf(arg_outip, "%s", argvalue);
        else if(strcmp(argname, "outport") == 0)
            arg_outport = atoi(argvalue);
        else
            fprintf(stderr, "Erreur : erreur dans le fichier de configuration : impossible de reconnaitre l'argument \"%s\"\n", argvalue);
    }

    free(argname);
    free(argvalue);

    // liberation du buffer de lecture
    free(strtmp);

    // fermeture du fichier
    close(f_in);
}

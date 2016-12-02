#include <stdlib.h>
#include <stdio.h>

#include "extremite.h"

int main(int argc, char* argv[])
{
    char * port = "1234";
    char adresseIPv6[50];

    int tun_fd;
    int choix = 0;
    char interface[] = "tun0";

    do
    {
        printf("Serveur(1)/Client(2)/pkill(3)\n");
        scanf("%d",&choix);
        switch(choix)
        {
        case 1 :
            printf("Lancement du serveur sur le port %s... \n", port);
            ext_out(port);
            break;
        case 2:
            printf("Entrez l'addresse IP : ");
            scanf("%s", adresseIPv6);
            printf("Lancement du client sur le port %s / interface %s... \n", port, interface);

            /* creation tun avec tun_alloc */
            printf("Creation de l'interface tun0... ");
            char interface[1024] = "tun0";

            tun_fd = tun_alloc (interface);

            if (tun_fd < 0)
            {
                printf ("Bad file descriptor !!!\n");
                return -1;
            }
            if (fork () == 0)
            {
                tun_copy (tun_fd, 1);
            }
            printf("Done !\n");

            /* Exec duscript de config de la route de tun0 */
            system("/mnt/partage/config.sh");

            /* Lancement du client */
            ext_in(adresseIPv6,port);

            printf("Le client est connecte !\n");
            break;
        case 3 :
            choix = 0;
            printf("Assassinat du tun0 et du reste...\n");
            system("pkill aucune_idee"); // bullshit
            break;
        }
    }
    while(choix ==0);

    return 0;
}




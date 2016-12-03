#include <stdlib.h>
#include <stdio.h>

#include "extremite.h"

#define PORT            "1234"
#define INTERFACE_NAME  "tun0"

int main(int argc, char* argv[])
{
    char port[] = PORT;
    char * adresseIPv6 = argv[1];

    int tun_fd;
    char interface[] = INTERFACE_NAME;

    char cmd[128];

    // allocation du tunnel
    tun_fd = tun_alloc(interface);

    // configuration du tunnnel
    sprintf(cmd, "sudo /vagrant/tun_config.sh %s", interface);
    system(cmd);

    // fork
    int process = fork();
    if(process < 0)
    {
        perror("demande de fork echouee...\n");
        exit(EXIT_FAILURE);
    }
    if(process == 0)
    {
        // code du fils
        ext_out(port, tun_fd);
    }
    else
    {
        // code du père
        ext_in(adresseIPv6, port, tun_fd);
    }

    close(tun_fd);
    return 0;

}




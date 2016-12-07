#include <stdlib.h>
#include <stdio.h>

#include "arguments.h"
#include "extremite.h"


int main(int argc, char* argv[])
{
    // parsing des arguments
    Arguments args;
    parseArguments(argc, argv, &args);

    // definitions des variables
    char port[ARGUMENTS_PORT_STR_SIZE];
    sprintf(port, "%s", args.arg_outport);
    char adresseIPv6[ARGUMENTS_IP_STR_SIZE];
    sprintf(adresseIPv6, "%s", args.arg_inip);

    int tun_fd;
    char interface[ARGUMENTS_TUN_STR_SIZE];
    sprintf(interface, "%s", args.arg_tun);

    char cmd[128];

    // allocation du tunnel
    tun_fd = tun_alloc(interface);
    sleep(1);

    // configuration du tunnnel
    sprintf(cmd, "sudo /vagrant/tun_config.sh %s", interface);
    system(cmd);
     sleep(1);

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
        // demarrage du serveur
        ext_out(port, tun_fd);
    }
    else
    {
        // code du père
        // demarrage du client
        sleep(5);
        ext_in(adresseIPv6, port, tun_fd);
    }

    close(tun_fd);
    return 0;

}




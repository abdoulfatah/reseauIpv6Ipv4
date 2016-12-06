#include "extremite.h"



//=============================================================================
// Prototypes des fonctions privees

void redirect_serveur(int src, int dst);
void redirect_client(int src, int dst);

//=============================================================================
// Definitions des fonctions privees

void redirect_serveur(int src, int dst)
{
    ssize_t lu; /* nb d'octets reçus */
    char tampon[MAXLIGNE+1];

    do
    {
        lu = read(src,tampon,MAXLIGNE);
        tampon[lu] = '\0';
        if (lu > 0 )
        {
            write(dst, tampon, lu+1);
        }
        else
        {
            break;
        }
    }
    while ( 1 );

  //  close(dst);
}

void redirect_client(int src, int dst)
{
    ssize_t size;
    char* buffer = malloc (sizeof (char) * BUFFER_SIZE_MAX);

    while (1)
    {
        // on lit la source
        size = read (src, buffer, BUFFER_SIZE_MAX);
        if(size == 0) // si src n'envoie plus
        {
            // on sort de la boucle
            break;
        }
        // on recopie dans la socket
        write (dst, buffer, size);
    }

   // free (buffer);
}




// Impl
void ext_out(char * port, int tun)
{
    int s, n; /* descripteurs de socket */
    int len, on; /* utilitaires divers */
    struct addrinfo * resol; /* résolution */
    struct addrinfo indic = {AI_PASSIVE, /* Toute interface */
               PF_INET6,SOCK_STREAM,0, /* IP mode connecté */
               0,NULL,NULL,NULL
    };
    struct sockaddr_in6 client; /* adresse de socket du client */
    int err; /* code d'erreur */

    err = getaddrinfo(NULL,port,&indic,&resol);
    if (err<0)
    {
        fprintf(stderr,"Résolution: %s\n",gai_strerror(err));
        exit(2);
    }

    /* Création de la socket, de type TCP / IP */
    if ((s=socket(resol->ai_family,resol->ai_socktype,resol->ai_protocol))<0)
    {
        perror("allocation de socket");
        exit(3);
    }
    fprintf(stderr,"le n° de la socket est : %i\n",s);

    /* On rend le port réutilisable rapidement /!\ */
    on = 1;
    if (setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0)
    {
        perror("option socket");
        exit(4);
    }
    fprintf(stderr,"Option(s) OK!\n");

    /* Association de la socket s à l'adresse obtenue par résolution */
    if (bind(s,resol->ai_addr,sizeof(struct sockaddr_in6))<0)
    {
        perror("bind");
        exit(5);
    }
    freeaddrinfo(resol); /* /!\ Libération mémoire */
    fprintf(stderr,"bind!\n");

    /* la socket est prête à recevoir */
    if (listen(s,SOMAXCONN)<0)
    {
        perror("listen");
        exit(6);
    }
    fprintf(stderr,"listen!\n");

    while(1)
    {
        /* attendre et gérer indéfiniment les connexions entrantes */
        len=sizeof(struct sockaddr_in6);
        if( (n=accept(s,(struct sockaddr *)&client,(socklen_t*)&len)) < 0 )
        {
            perror("accept");
            exit(7);
        }
        /* Nom réseau du client */
        char hotec[NI_MAXHOST];
        char portc[NI_MAXSERV];
        err = getnameinfo((struct sockaddr*)&client,len,hotec,NI_MAXHOST,portc,NI_MAXSERV,0);
        if (err < 0 )
        {
            fprintf(stderr,"résolution client (%i): %s\n",n,gai_strerror(err));
        }
        else
        {
            fprintf(stderr,"accept! (%i) ip=%s port=%s\n",n,hotec,portc);
        }
        /* traitement */
        redirect_serveur(n,tun);
    }
}



void ext_in(char * hote, char * port, int tun)
{
    void *addr;
    char ip[NI_MAXHOST], ipstr[INET6_ADDRSTRLEN];; /* adresse IPv4 en notation pointée */
    struct addrinfo *resol; /* struct pour la résolution de nom */
    int s; /* descripteur de socket */

    /* Résolution de l'hôte */
    if ( getaddrinfo(hote,port,NULL, &resol) < 0 )
    {
        perror("résolution adresse");
        exit(2);
    }

    /****/
    struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)resol->ai_addr;
    addr = &(ipv6->sin6_addr);
    // conversion de l'adresse IP en une chaîne de caractères
    inet_ntop(resol->ai_family, addr, ipstr, sizeof ipstr);

    /* On extrait l'addresse IP */
    sprintf(ip,"%s",ipstr);


    /* Création de la socket, de type TCP / IP */
    /* On ne considère que la première adresse renvoyée par getaddrinfo */
    if ((s=socket(resol->ai_family,resol->ai_socktype, resol->ai_protocol))<0)
    {
        perror("allocation de socket");
        exit(3);
    }
    fprintf(stderr,"le n° de la socket est : %i\n",s);

    /* Connexion */
    fprintf(stderr,"Essai de connexion à %s (%s) sur le port %s\n\n",
            hote,ip,port);
    if (connect(s,resol->ai_addr,sizeof(struct sockaddr_in6))<0)
    {
        perror("connexion");
        exit(4);
    }
    freeaddrinfo(resol); /* /!\ Libération mémoire */

    redirect_client(tun, s);

    fprintf(stderr,"Fin de la session.\n");
}


int tun_alloc(char *dev)
{
    struct ifreq ifr;
    int fd, err;

    if( (fd = open("/dev/net/tun", O_RDWR)) < 0 )
    {
        perror ("Tun n'est pas trouvable.");
        return -1;
    }
    memset (&ifr, 0, sizeof (ifr));

    /* Flags:
     *       IFF_TUN   - TUN device (no Ethernet headers)
     *       IFF_TAP   - TAP device
     *       IFF_NO_PI - Do not provide packet information
    */
    ifr.ifr_flags = IFF_NO_PI | IFF_TUN;
    if (*dev)
    {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if ((err = ioctl (fd, TUNSETIFF, (void *) &ifr)) < 0)
    {
        close (fd);
        perror("Tentaive d'ouverture du tunnel.");
        return err;
    }

    strcpy(dev, ifr.ifr_name);
    return fd;
}




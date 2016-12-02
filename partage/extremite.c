#include "extremite.h"


// Proto a la volle

void print_serveur(int f, char* hote, char* port);


// Impl
void ext_out(char * port)
{
    int s,n; /* descripteurs de socket */
    int len,on; /* utilitaires divers */
    struct addrinfo * resol; /* résolution */
    struct addrinfo indic = {AI_PASSIVE, /* Toute interface */
               PF_INET6,SOCK_STREAM,0, /* IP mode connecté */
               0,NULL,NULL,NULL
    };
    struct sockaddr_in6 client; /* adresse de socket du client */
    int err; /* code d'erreur */

    fprintf(stderr,"Ecoute sur le port %s\n",port);
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
        print_serveur(n,hotec,portc);
    }
}


void print_serveur(int f, char* hote, char* port)
{
    ssize_t lu; /* nb d'octets reçus */
    char msg[MAXLIGNE+1]; /* tampons pour les communications */
    char tampon[MAXLIGNE+1];
    int pid = getpid(); /* pid du processus */
    int compteur=0;

    /* message d'accueil */
    snprintf(msg,MAXLIGNE,"Bonjour %s! (vous utilisez le port %s)\n",hote,port);
    /* envoi du message d'accueil */
    send(f,msg,strlen(msg),0);

    do   /* Faire echo et logguer */
    {
        lu = recv(f,tampon,MAXLIGNE,0);
        if (lu > 0 )
        {
            compteur++;
            tampon[lu] = '\0';
            /* log */
            fprintf(stderr,"[%s:%s](%i): %3i :%s",hote,port,pid,compteur,tampon);
            snprintf(msg,MAXLIGNE,"> %s",tampon);
            /* echo vers le client */
            printf("%s", msg);
        }
        else
        {
            break;
        }
    }
    while ( 1 );

    /* le correspondant a quitté */
    send(f,CIAO,strlen(CIAO),0);
    close(f);
    fprintf(stderr,"[%s:%s](%i): Terminé.\n",hote,port,pid);
}


void ext_in(char * hote, char * port)
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

    /* Session */
    char tampon[MAXLIGNE + 3]; /* tampons pour les communications */
    ssize_t lu;
    int fini=0;
    while( 1 )
    {
        /* Jusqu'à fermeture de la socket (ou de stdin)     */
        /* recopier à l'écran ce qui est lu dans la socket  */
        /* recopier dans la socket ce qui est lu dans stdin */

        /* réception des données */
        lu = recv(s,tampon,MAXLIGNE,0); /* bloquant */
        if (lu == 0 )
        {
            fprintf(stderr,"Connexion terminée par l'hôte distant\n");
            break; /* On sort de la boucle infinie */
        }
        tampon[lu] = '\0';
        //printf("reçu: %s",tampon);
        if ( fini == 1 )
            break;  /* on sort de la boucle infinie*/

        /* recopier dans la socket ce qui est entré au clavier */
        if ( fgets(tampon,MAXLIGNE - 2,stdin) == NULL ) /* entrée standard fermée */
        {
            fini=1;
            fprintf(stderr,"Connexion terminée !!\n");
            fprintf(stderr,"Hôte distant informé...\n");
            shutdown(s, SHUT_WR); /* terminaison explicite de la socket
			     dans le sens client -> serveur */
            /* On ne sort pas de la boucle tout de suite ... */
        }
        else     /* envoi des données */
        {
            send(s,tampon,strlen(tampon),0);
        }
    }
    /* Destruction de la socket */
    close(s);

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


void tun_copy (int src, int dst)
{
    ssize_t size;
    char* buffer = malloc (sizeof (char) * BUFFER_SIZE_MAX);

    while (1)
    {
        printf ("Read:\n");
        size = read (src, buffer, BUFFER_SIZE_MAX);

        printf ("Write:\n");
        write (dst, buffer, size);
    }

    free (buffer);
}


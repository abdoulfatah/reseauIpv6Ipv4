#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h> 
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#define BUFFER_SIZE_MAX 1024

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

int main (int argc, char** argv)
{
    int tun_fd;
    char *interface;

    if (argc != 2)
    {
        perror ("Bad number of argument.");
        return -2;
    }

    
    interface = malloc(1024);
    sprintf (interface, "tun%s", argv[1]);

    tun_fd = tun_alloc (interface);
    printf("  %d",tun_fd);
    if (tun_fd < 0)
    {
        printf ("Bad file descriptor.\n");
        free (interface);
        return -1;
    }


    printf ("%s -> fd = %d\n", interface, tun_fd);
    
    if (fork () == 0)
    {
        tun_copy (tun_fd, 1);
    }

    free (interface);
    return 0;
}


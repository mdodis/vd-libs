#include "vd_net.h"
#include <string.h>
#include <stdio.h>

static void print_commands(void);
static int  cmd_show_ip(int argc, char **argv); 
static int  cmd_telnet(int argc, char **argv); 

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    if (argc <= 1) {
        puts("Not enough arguments. Expected at least one command argument");
        print_commands();
        return -1;
    }

    if (0) {
    } else if (strcmp(argv[1], "show_ip") == 0) {
        return cmd_show_ip(argc - 2, argv + 2);
    } else if (strcmp(argv[1], "telnet") == 0) {
        return cmd_telnet(argc - 2, argv + 2);
    }

    return 0;
}

static void print_commands(void)
{
    puts("net_socket.c -- Sample");
    puts("COMMANDS");

    puts("show_ip <Host Name>                   Shows IP Address of Host");
    puts("telnet <Host Name>                    Telnet Client");
}

static int cmd_show_ip(int argc, char **argv)
{
    if (argc < 1) {
        puts("show_ip: Not enough arguments");
        print_commands();
        return -1;
    }

    VdNetAddrInfo hints = {0};
    VdNetAddrInfo *servinfo = 0;
    hints.ai_family = VD_NET_AF_UNSPEC;
    hints.ai_socktype = VD_NET_SOCK_STREAM;

    printf("Resolving %s\n", argv[0]);
    if (vd_net_getaddrinfo(argv[0], 0, &hints, &servinfo) != 0) {
        printf("show_ip: getaddrinfo failed\n");
        return -1;
    }

    for (VdNetAddrInfo *p = servinfo; p != 0; p = p->ai_next) {
        const char *ipver;
        void *addr;

        if (p->ai_family == VD_NET_AF_INET) {
            VdNetSockAddrIn *ipv4 = (VdNetSockAddrIn*)p->ai_addr;
            ipver = "IPv4";
            addr = &ipv4->sin_addr;
        } else {
            VdNetSockAddrIn6 *ipv6 = (VdNetSockAddrIn6*)p->ai_addr;
            ipver = "IPv6";
            addr = &ipv6->sin6_addr;
        }

        char ipstr[VD_NET_INET6_ADDRSTRLEN];
        vd_net_inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));

        printf("\t%s: %s\n", ipver, ipstr);
    }

    return 0;
}

static int cmd_telnet(int argc, char **argv)
{
    if (argc != 1) {
        puts("telnet: Expected exactly one argument.");
        print_commands();
        return -1;
    }
    const char *hostname = argv[0];
    return 0;
}

#define VD_NET_IMPL
#include "vd_net.h"
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){
    printf("client starting");
    int sock;
    struct sockaddr_in addr;
    char buf[80];
    sock = socket(AF_INET, SOCK_STREAM, 0); // setting up socket. AF_INET = IP, SOCK_STREAM = TCP, 0 = Auto setup
    if (sock==-1)
    {
        perror("opening socket"); //error opening socket (print error)
        exit(-1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(32351); //makes sure port is stored in big-endian
    addr.sin_addr.s_addr = inet_network("10.130.26.28"); //stores ip string in big-endian
    if(connect(sock, (const struct sockaddr *) &addr, sizeof(struct sockaddr_in))==-1);
    {
        perror("on connect");
        exit(-1);
    }

    send(sock, "client calling server, do you read me", 38, 0);
    recv(sock, buf, 80, 0);

    if(shutdown(sock, 1)==-1)
    {
        perror("on shutdown");
        exit(-1);
    }
    printf("Client is done\n");
    close(sock);
    return 0;
}
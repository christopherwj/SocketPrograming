#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef struct {
    int intvalue;
    char charvalue;
    float floatvalue;
} dataPacket;

int main(){
    int sock;
    struct sockaddr_in serverAddr;
    dataPacket clientData;
    char buf[80];
    char SYN[] = "SYN";
    sock = socket(AF_INET, SOCK_STREAM, 0); // setting up socket. AF_INET = IP, SOCK_STREAM = TCP, 0 = Auto setup
    if (sock==-1)
    {
        printf("opening socket"); //error opening socket (print error)
        exit(-1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(32351); //makes sure port is stored in big-endian
    serverAddr.sin_addr.s_addr = inet_network("1.1.0.127"); //stores ip string in big-endian

    if(connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        printf("on connect\n");
        exit(-1);
    }

    clientData.intvalue = 5;
    clientData.charvalue = 'C';
    clientData.floatvalue = 3.14;

    // send(sock, SYN, sizeof(SYN), 0);
    printf("Data: %d", clientData.intvalue);
    send(sock, (dataPacket*) &clientData, sizeof(clientData), 0);
    recv(sock, buf, 80, 0);

    if(shutdown(sock, 1)==-1)
    {
        printf("on shutdown");
        exit(-1);
    }
    printf("Client is done\n");
    close(sock);
    return 0;
}

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

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
        perror("opening socket"); //error opening socket (print error)
        exit(-1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(32351); //makes sure port is stored in big-endian

    // TODO: CHRIS CHANGE LOGIC HERE FOR CONNECTING TO SERVER!
    serverAddr.sin_addr.s_addr = inet_network("1.1.0.127"); //stores ip string in big-endian

    if(connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("on connect\n");
        exit(-1);
    }

    send(sock, SYN, sizeof(SYN), 0);
    recv(sock, buf, sizeof(buf), 0);

    if (strcmp(buf, "ACK1") == 0)
    {
        printf("This Client has been assigned #1\n");
        clientData.intvalue = 5;
        clientData.charvalue = 'C';
        clientData.floatvalue = 3.14;
        send(sock, (dataPacket*)&clientData, sizeof(clientData), 0);
    }
    else if (strcmp(buf, "ACK2") == 0)
    {
        printf("This Client has been assigned #2\n");
	    recv(sock, (dataPacket*)&clientData, sizeof(clientData), 0);
	    printf("Message Received: %c\n", clientData.charvalue);
	    printf("Message Received: %d\n", clientData.intvalue);
	    printf("Message Received: %f\n", clientData.floatvalue);
    }
    else {
        printf("Server or Client are in unstable state. Restart both\n");
    }
    if(shutdown(sock, 2)==-1)
    {
        perror("on shutdown");
        exit(-1);
    }
    printf("Client is done\n");
    close(sock);
    return 0;
}

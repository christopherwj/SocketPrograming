#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

//Defining a struct to hold all the data values. Aliasing the struct as "dataPacket"
typedef struct {
    int intvalue;
    char charvalue;
    float floatvalue;
} dataPacket;

int main()
{
    /*******************************************************
     * Defining all variables
     ******************************************************/
    int sock;
    struct sockaddr_in serverAddr;
    dataPacket clientData;
    char buf[80];
    char SYN[] = "SYN";
    struct hostent* hostInfo;
    struct in_addr** addrList;
    char* serverDNS, *dnsIP;

    /*******************************************************
     * Setting up client socket. 
     * AF_INET = IP 
     * SOCK_STREAM = TCP
     * 0 = Auto setup
     ******************************************************/
    sock = socket(AF_INET, SOCK_STREAM, 0); 
    if (sock==-1)
    {
        perror("opening socket"); //error opening socket (print error)
        exit(-1);
    }
	
    /*******************************************************
     * Query hostname by DNS 
     ******************************************************/
    serverDNS = "elon.sdsurocketproject.org";
    if ((hostInfo = gethostbyname((const char*)serverDNS)) == NULL)
	{
        perror("getting host info"); //error getting host info
        exit(-1);
	}
	addrList = (struct in_addr**)hostInfo->h_addr_list;
	dnsIP = inet_ntoa(*addrList[0]);
	printf("DNS from %s returned IP: %s\n\n", serverDNS, dnsIP);


    /*******************************************************
     * Setting up server address information and make connection attempt 
     * AF_INET = IP 
     * Port Number = 32351
     * IP Address = DNS resolved
     *******************************************************/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(32351); //makes sure port is stored in big-endian
    serverAddr.sin_addr.s_addr = inet_network(dnsIP); //stores ip string in big-endian
    if(connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("on connect");
        exit(-1);
    }

    /*******************************************************
     * After connection is established with server, attempt to ACK 
     *******************************************************/
    send(sock, SYN, sizeof(SYN), 0);
    recv(sock, buf, sizeof(buf), 0);

    /*******************************************************
     * Use string comparison to uniquely determine which 
     * client this machine is. If the server does an ACK1 we
     * know we are the 1st client and we must send data. If
     * the server does an ACK2 we know we are the 2nd client
     * and we must receive modified data. 
     *******************************************************/
    if (strcmp(buf, "ACK1") == 0)
    {
        printf("This Client has been assigned #1\n");
        clientData.intvalue = 5;
        clientData.charvalue = 'C';
        clientData.floatvalue = 3.14;
        send(sock, (dataPacket*)&clientData, sizeof(clientData), 0);
	    printf("INT Data Sent: %d\n", clientData.intvalue);
	    printf("FLOAT Data Sent: %f\n", clientData.floatvalue);
	    printf("CHAR Data Sent: %c\n", clientData.charvalue);
    }
    else if (strcmp(buf, "ACK2") == 0)
    {
        printf("This Client has been assigned #2\n");
	    recv(sock, (dataPacket*)&clientData, sizeof(clientData), 0);
	    printf("INT Data Received: %d\n", clientData.intvalue);
	    printf("FLOAT Data Received: %f\n", clientData.floatvalue);
	    printf("CHAR Data Received: %c\n", clientData.charvalue);
    }
    else {
        printf("Server or Client are in unstable state. Restart both\n");
    }

    /*******************************************************
     * Once process is complete, shutdown socket and close connection 
     *******************************************************/
    if(shutdown(sock, 2)==-1)
    {
        perror("on shutdown");
        exit(-1);
    }
    printf("Client is done\n");
    close(sock);
    return 0;
}

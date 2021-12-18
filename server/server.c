#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

//Defining a struct to hold all the data values. Aliasing the struct as "dataPacket"
typedef struct {
    int intvalue;
    char charvalue;
    float floatvalue;
} dataPacket;

void main()
{
    /*******************************************************
     * Defining all variables
     ******************************************************/
	int sock, clientSock1, clientSock2, mlen, addrsize, msgct, chc, chct;
	struct sockaddr_in serverAddr, clientAddr;
	dataPacket serverData;
	char ch, buf[80];

	printf("Server is running\n\n");

    /*******************************************************
     * Setting up server socket. 
     * AF_INET = IP 
     * SOCK_STREAM = TCP
     * 0 = Auto setup
     ******************************************************/
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		perror("While opening socket");
		exit(-1);
	}

    /*******************************************************
     * Setting up server address information and make connection attempt 
	 * bind a name to the socket since the server will bind with any client
	 * the address is zero or INADDR_ANY
     * AF_INET = IP 
     * Port Number = 32351
     * IP Address = DNS resolved
     *******************************************************/
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(32351);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
	{
		perror("While binding");
		exit(-1);
	}

	if(listen(sock, 2) == -1) //make socket available 
	{
		perror("While attempting to listen");
		exit(-1);
	}

	//wait for a client to connect. Once connected print out a message.

	addrsize = sizeof(struct sockaddr_in); 
	clientSock1 = accept(sock, (struct sockaddr*)&clientAddr, &addrsize);
	if(clientSock1 == -1)
	{
		perror("On accept");
		exit(-1);
	}
	printf("Connection made with client %s\n", inet_ntoa(clientAddr.sin_addr));
	
	//receive and print a client message where a null char terminates 
	//note a single receive may not work in some cases, but is OK for a simple 
	//example
	recv(clientSock1, (char*)&buf, sizeof(buf), 0);
	if (strcmp(buf, "SYN"))
	{
		send(clientSock1, "Ban SYN", 8, 0);
		printf("Bad SYN from client 1! Repeat Request.\n");
		exit(-1);
	}
	send(clientSock1, "ACK1", 5, 0);
	recv(clientSock1, (dataPacket*)&serverData, sizeof(serverData), 0);

	printf("INT Data Received: %d\n", serverData.intvalue);
	printf("FLOAT Data Received: %f\n\n", serverData.floatvalue);
	printf("CHAR Data Received: %c\n", serverData.charvalue);

	serverData.intvalue = 2*serverData.intvalue;
	serverData.floatvalue = serverData.floatvalue + 1;
	serverData.charvalue = (char)((int)serverData.charvalue + 1);

	clientSock2 = accept(sock, (struct sockaddr*)&clientAddr, &addrsize);
	send(clientSock2, "ACK2", 5, 0);
	if(clientSock2 == -1)
	{
		perror("On accept");
		exit(-1);
	}
	printf("Connection made with client %s\n", inet_ntoa(clientAddr.sin_addr));

	printf("INT Data Sent: %d\n", serverData.intvalue);
	printf("FLOAT Data Sent: %f\n", serverData.floatvalue);
	printf("CHAR Data Sent: %c\n", serverData.charvalue);

    send(clientSock2, (dataPacket*)&serverData, sizeof(serverData), 0);

	//close the client socket and also the server socket
	printf("Server - process complete - connection being closed\n");
	close(clientSock1);
	close(clientSock2);
	close(sock);
}

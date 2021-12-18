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

    /*******************************************************
     * Make the socket available and allow for 2 connections in listen buffer.
	 * Once a client has reached out to connect, accept the connection and
	 * indicate to user that a connection has been made.
     ******************************************************/
	if(listen(sock, 2) == -1) 
	{
		perror("While attempting to listen");
		exit(-1);
	}
	addrsize = sizeof(struct sockaddr_in); 
	clientSock1 = accept(sock, (struct sockaddr*)&clientAddr, &addrsize);
	if(clientSock1 == -1)
	{
		perror("On accept");
		exit(-1);
	}
	printf("Connection made with client %s\n", inet_ntoa(clientAddr.sin_addr));
	
    /*******************************************************
	 * Wait for a SYN packet from the 1st client. Once received
	 * send an ACK1 to indicate to client that it must send data.
     ******************************************************/
	recv(clientSock1, (char*)&buf, sizeof(buf), 0);
	if (strcmp(buf, "SYN"))
	{
		send(clientSock1, "Ban SYN", 8, 0);
		printf("Bad SYN from client 1! Repeat Request.\n");
		exit(-1);
	}
	send(clientSock1, "ACK1", 5, 0);
	

    /*******************************************************
	 * Get stuct data from 1st client. Print data to console
	 * and manipulate data as required by the program.
     ******************************************************/
	recv(clientSock1, (dataPacket*)&serverData, sizeof(serverData), 0);

	printf("INT Data Received: %d\n", serverData.intvalue);
	printf("FLOAT Data Received: %f\n", serverData.floatvalue);
	printf("CHAR Data Received: %c\n\n", serverData.charvalue);

	serverData.intvalue = 2*serverData.intvalue;
	serverData.floatvalue = serverData.floatvalue + 1;
	serverData.charvalue = (char)((int)serverData.charvalue + 1);

    /*******************************************************
	 * Connect to 2nd client and wait for SYN packet
	 * Wait for a SYN packet from the 2nd client. Once received
	 * send an ACK1 to indicate to client that it must send data.
     ******************************************************/
	clientSock2 = accept(sock, (struct sockaddr*)&clientAddr, &addrsize);
	if(clientSock2 == -1)
	{
		perror("On accept");
		exit(-1);
	}
	printf("Connection made with client %s\n", inet_ntoa(clientAddr.sin_addr));

    /*******************************************************
	 * Wait for a SYN packet from the 2nd client. Once received
	 * send an ACK2 to indicate to client that it must receive
	 * modified data.
     ******************************************************/
	recv(clientSock2, (char*)&buf, sizeof(buf), 0);
	if (strcmp(buf, "SYN"))
	{
		send(clientSock2, "Ban SYN", 8, 0);
		printf("Bad SYN from client 2! Repeat Request.\n");
		exit(-1);
	}
	send(clientSock2, "ACK2", 5, 0);


    /*******************************************************
	 * Get stuct data from 1st client. Print data to console
	 * and manipulate data as required by the program.
     ******************************************************/
    send(clientSock2, (dataPacket*)&serverData, sizeof(serverData), 0);

	printf("INT Data Sent: %d\n", serverData.intvalue);
	printf("FLOAT Data Sent: %f\n", serverData.floatvalue);
	printf("CHAR Data Sent: %c\n", serverData.charvalue);

    /*******************************************************
	 * Close both client sockets and server socket
     ******************************************************/
	printf("Server - process complete - connection being closed\n");
	close(clientSock1);
	close(clientSock2);
	close(sock);
}

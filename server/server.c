#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef struct {
    int intvalue;
    char charvalue;
    float floatvalue;
} dataPacket;

int main()
{
	int sock, clientsock, mlen, addrsize, msgct, chc, chct;
	struct sockaddr_in serverAddr, clientAddr;
	dataPacket serverData;
	char ch, buf[80];

	printf("Server is running\n\n");
	//create a socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	//socket doesn't exit
	if(sock == -1)
	{
		printf("ERROR: opening socket");
		exit(-1);
	}
	/*
	* bind a name to the socket since the server will bind with any client
	*the address is zero or INADDR_ANY
	*/
	serverAddr.sin_family =AF_INET;
	serverAddr.sin_port = htons(32351);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
	{
		printf("ERROR: on bind");
		exit(-1);
	}

	if(listen(sock,1) == -1) //make socket available 
	{
		perror("on listen");
		exit(-1);
	}

	//wait for a client to connect. Once connected print out a message.

	addrsize = sizeof(struct sockaddr_in); 
	clientsock = accept(sock, (struct sockaddr*)&clientAddr, &addrsize);
	if(clientsock == -1)
	{
		perror("on accept");
		exit(-1);
	}
	printf("Connection made with client %s\n", inet_ntoa(clientAddr.sin_addr));
	
	//receive and print a client message where a null char terminates 
	//note a single receive may not work in some cases, but is OK for a simple 
	//example
	mlen = recv(clientsock, (dataPacket*)&serverData, sizeof(serverData), 0);
	send(clientsock, "Got your message", 17, 0);
	printf("Server - all messages read - connection being closed\n");
	printf("Message Received: %c\n", serverData.charvalue);
	printf("Message Received: %d\n", serverData.intvalue);
	printf("Message Received: %f\n", serverData.floatvalue);

	// int l;
	// while (1)
	// {
	// 	l = recv(clientsock, buf, sizeof(buf), 0);
	// 	if (l > 0){
	// 		printf("%s", buf);
	// 	}
	// }

	//close the client socket and also the server socket

	close(clientsock);
	close(sock);
}

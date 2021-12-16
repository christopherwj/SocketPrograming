#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>


int main()
{
	int sock, clientsock, mlen, addrsize, msgct, chc, chct;
	struct sockaddr_in addr;
	char ch,buf[80];

	printf("server is running");
	//create a socket
	sock = socket(AF_INET, SOCK_STREAM,0);
	//socket doesn't exit
	if(sock == -1)
	{
		perror("openning socket");
		exit(-1);
	}
	/*
	* bind a name to the socket since the server will bind with any client
	*the address is zero or INADDR_ANY
	*/
	addr.sin_family =AF_INET;
	addr.sin_port = htons(32351);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sock,&addr, sizeof(struct sockaddr_in)) == -1)
	{
		perror("on bind");
		exit(-1);
	}

	if(listen(sock,1) == -1) //make socket available 
	{
		perror("on listen");
		exit(-1);
	}

	//wait for a client to connect. Once connected print out a message.

	addrsize = sizeof(struct sockaddr_in); 
	clientsock = accept(sock, &addr, &addrsize);
	if(clientsock == -1)
	{
		perror("on accept");
		exit(-1);
	}
	printf("connection made with client %s", inet_ntoa(addr.sin_addr));
	
	//receive and print a client message where a null char terminates 
	//note a single receive may not work in some cases, but is OK for a simple 
	//example
	mlen = recv(clientsock,buf,80,0);
	send(clientsock, "Got your message", 17,0);
	printf("Server - all messages read - connection being closed\n");

	//close the client socket and also the server socket

	close(clientsock);
	close(sock);
}

/////////////////////////////////////////
//   the server code for TCP socket    //
/////////////////////////////////////////

#include <netinet/in.h> //for sockaddr_in
#include <sys/types.h>  //for socket
#include <sys/socket.h> //for socket
#include <stdio.h>      //for printf
#include <stdlib.h>     //for exit
#include <string.h>     //for memset
#include <time.h>       //for time_t and time
#include <fcntl.h>
#include <errno.h>      //for errno

#define SERVER_PORT 5790
#define LENGTH_OF_QUEUE 20
#define BUFFER_SIZE 1024

int main(int argc, char const *argv[])
{
	struct sockaddr_in server_addr;//define a universal socket struct

	bzero(&server_addr,sizeof(server_addr));//clear the memory to zero
	server_addr.sin_family=AF_INET;//IPv4 protocol
	server_addr.sin_addr.s_addr=htons(INADDR_ANY);//set the IP address with localhost IP address
	server_addr.sin_port=htons(SERVER_PORT);//set the nameed port

	//time_t now;
	

	//create the socket for the server
	int server_socket;
	if ((server_socket=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		fprintf(stderr, "socket:%s\n", strerror(errno));
		exit(1);
	}

	//bind the socket with the socket address
	if ((bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))==-1)
	{
		fprintf(stderr, "bind:%s\n", strerror(errno));
		exit(1);
	}

	//set the server's socket to listen for the request from the clients
	if ((listen(server_socket,LENGTH_OF_QUEUE))==-1)
	{
		fprintf(stderr, "listen:%s\n", strerror(errno));
		exit(1);
	}

	//set the server running all the time
	while(1)
	{
		struct sockaddr_in client_addr;//define a socket struct for each client
		socklen_t length=sizeof(client_addr);
		int new_server_socket;
		if ((new_server_socket=accept(server_socket,(struct sockaddr*)&client_addr,&length))<0)
		{
			fprintf(stderr, "accept:%s\n", strerror(errno));
			break;
		}

		//set the notice info to send to the client
		char buffer[BUFFER_SIZE];
		bzero(buffer,BUFFER_SIZE);
		strcpy(buffer,"Hello,coming from the server!");
		strcat(buffer,"\n");

		send(new_server_socket,buffer,BUFFER_SIZE,0);

		bzero(buffer,BUFFER_SIZE);

		//receive the data to send to the client
		length=recv(new_server_socket,buffer,BUFFER_SIZE,0);
		if (length<0)
		{
			fprintf(stderr, "recv:%s\n", strerror(errno));
			exit(1);
		}
		//display the data receive from the client
		printf("%s\n",buffer);

		//send the local file stream to the client
		int stream;
		if ((stream=open("send_data",O_RDWR))==-1)
		{
			fprintf(stderr, "open:%s\n", strerror(errno));
			exit(1);
		}else
		{
			printf("The file send_data was opened.\n");
		}
		 

		int lengthsize=0;
		int i=0;
		while((lengthsize=read(stream,buffer,sizeof(buffer)))>0)
		{
			printf("lengthsize=%d\n",lengthsize);
			//printf("%s\n", buffer);
			if (send(new_server_socket,buffer,lengthsize,0)<0)
			{
				printf("send file failed!\n");
				break;
			}
			else
			{
				printf("the %d time to send...\n",++i);
			}
			bzero(buffer,BUFFER_SIZE);
		}
		if (close(stream)==-1)
		{
			fprintf(stderr, "close%s\n", strerror(errno));
			exit(1);
		}
		close(new_server_socket);//close the connection with the client
	}

	//close the socket for listening
	close(server_socket);
	return 0;
}

/*
[Test]
gcc -o tcp_server tcp_server.c
./tcp_server 

Hello,this is client A!

The file send_data was opened.
lengthsize=1024
the 1 time to send...
lengthsize=1024
the 2 time to send...
lengthsize=1024
the 3 time to send...
lengthsize=1024
the 4 time to send...
lengthsize=1024
the 5 time to send...
lengthsize=805
the 6 time to send...



*/
/////////////////////////////////////////
//    the client code for TCP socket   //
/////////////////////////////////////////

#include <netinet/in.h> //for sockaddr_in
#include <sys/types.h>  //for socket type
#include <sys/socket.h> //for socket()
#include <stdio.h>      //for printf
#include <stdlib.h>     //for exit
#include <string.h>     //for memset
#include <time.h>       //for time_t and time
#include <arpa/inet.h>  //for INTERNET definition
#include <fcntl.h>
#include <errno.h>      //for errno

#define SERVER_PORT 8888
#define BUFFER_SIZE 1024

int main(int argc, char const *argv[])
{
	if (argc!=2)
	{
		printf("Usage:%s ServerIPAddress\n",argv[0]);
		exit(1);
	}

	//定义一个通用的套接字结构
	struct sockaddr_in client_addr;
	bzero(&client_addr,sizeof(client_addr));//清空缓存
	client_addr.sin_family=AF_INET;
	client_addr.sin_addr.s_addr=htons(INADDR_ANY);//INADDR_ANY represents the localhost ip address
	client_addr.sin_port=htons(0);//0 represents the system will automatically allocate a free port 

	int client_socket;
	if ((client_socket=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		fprintf(stderr, "socket:%s\n", strerror(errno));
		exit(1);
	}

	if (bind(client_socket,(struct sockaddr*)&client_addr,sizeof(client_addr))<0)
	{
		fprintf(stderr, "bind:%s\n", strerror(errno));
		exit(1);
	}

	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;//IPv4 protocol
	if (inet_aton(argv[1],&server_addr.sin_addr)==0)//convert the input ip address 
	{
		fprintf(stderr, "server address:%s\n", strerror(errno));
		exit(1);
	}
	server_addr.sin_port=htons(SERVER_PORT);


	socklen_t server_addr_lentth=sizeof(server_addr);
	if (connect(client_socket,(struct sockaddr*)&server_addr,server_addr_lentth)<0)
	{
		fprintf(stderr, "can't connect to:%s because that %s:\n", argv[1],strerror(errno));
		exit(1);
	}

	char buffer[BUFFER_SIZE];
	bzero(buffer,BUFFER_SIZE);

	int length;
	if ((length=recv(client_socket,buffer,BUFFER_SIZE,0))<0)
	{
		fprintf(stderr, "recv:%s\n", strerror(errno));
		exit(1);
	}
	printf("\n%s\n",buffer);

	bzero(buffer,BUFFER_SIZE);

	strcpy(buffer,"Hello,this is client A!\n");
	send(client_socket,buffer,BUFFER_SIZE,0);

	int stream;
	if ((stream=open("receive_data",O_RDWR))==-1)
	{
		printf("The file data was not opened!\n");
	}else
		bzero(buffer,BUFFER_SIZE);

	//length=0;
	while(length=recv(client_socket,buffer,BUFFER_SIZE,0))
	{
		if (length<0)
		{
			printf("Receive data from server %s failed!\n", argv[1]);
			break;
		}

		int write_lenth=write(stream,buffer,sizeof(buffer));
		if (write_lenth<length)
		{
			printf("File write Failed!\n");
			break;
		}
		bzero(buffer,BUFFER_SIZE);
	}
	printf("Receive File From Server [%s] Finished.\n",argv[1]);

	//关闭文件
	close(stream);

	//关闭套接字
	close(client_socket);
	return 0;
}

/*
[Test]
gcc -o tcp_client tcp_client.c
./tcp_client 127.0.0.1


Hello,coming from the server!

Receive File From Server [127.0.0.1] Finished.


*/

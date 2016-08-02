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

#define SERVER_PORT 8888
#define LENGTH_OF_QUEUE 20
#define BUFFER_SIZE 1024

int main(int argc, char const *argv[])
{
    //定义一个通用的套接字结构
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;//IPv4 协议族
	server_addr.sin_addr.s_addr=htons(INADDR_ANY);//使用localhost IP地址设置服务器地址
	server_addr.sin_port=htons(SERVER_PORT);//指定监听端口

	//为服务端创建套接字
	int server_socket;
	if ((server_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1)
	{
		//fprintf(stderr, "socket:%s\n", strerror(errno));
	    perror("socket");
		exit(1);
	}

	//绑定套接字到套接字地址
	if ((bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))==-1)
	{
		//fprintf(stderr, "bind:%s\n", strerror(errno));
	    perror("bind");
		exit(1);
	}

	//使得服务器的套接字监听客户端的请求
	if ((listen(server_socket,LENGTH_OF_QUEUE))==-1)
	{
		perror("listen");
		exit(1);
	}

	//使得服务器一直运行
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

		//接收来自客户端的请求
		length=recv(new_server_socket,buffer,BUFFER_SIZE,0);
		if (length<0)
		{
			fprintf(stderr, "recv:%s\n", strerror(errno));
			exit(1);
		}
		//显示从客户端接收到的请求数据
		printf("%s\n",buffer);

		//发送本地的文件流给客户端
		int stream;
		if ((stream=open("../TestData/send_data",O_RDWR))==-1)
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

	//关闭服务器监听套接字
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

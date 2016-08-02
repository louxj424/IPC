#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>

#define BUFFER_SIZE 10


int main(int argc, char const *argv[])
{
	int n;
	int fd[2];
	pid_t pid;
	//发送缓冲区，初始化数据为welcome!
	char sendBuffer[BUFFER_SIZE]="welcome!";
	//接收缓冲区
	char receiveBuff[BUFFER_SIZE];
	//清空接口缓冲区
	memset(receiveBuff,0,sizeof(receiveBuff));

	//创建匿名管道
	if (pipe(fd)==-1)
	{
		fprintf(stderr, "pipe:%s\n",strerror(errno));
		exit(1);
	}

	//利用fork方式创建子进程
	if ((pid=fork())<0)
	{
		fprintf(stderr, "fork:%s\n",strerror(errno));
		exit(1);
	}else if (pid>0)//父进程
	{
		close(fd[0]);//关闭读端
		write(fd[1],sendBuffer,BUFFER_SIZE);
		printf("parent process %d send:%s\n", getpid(),sendBuffer);
	}else//子进程
	{
		close(fd[1]);//关闭写端
		read(fd[0],receiveBuff,BUFFER_SIZE); 
		printf("child process %d receive:%s\n",getpid(),receiveBuff);
	}
	return 0;
}

/*
[Test]
gcc -o pipe pipe.c 
./pipe 
parent process send:welcome!
child process receive:welcome
*/

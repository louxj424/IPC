#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>

#define BUFFER_SIZE 9


int main(int argc, char const *argv[])
{
	int n;
	int fd[2];
	pid_t pid;
	char sendBuffer[BUFFER_SIZE]="welcome!";
	char receiveBuff[BUFFER_SIZE];
	memset(receiveBuff,0,sizeof(receiveBuff));

	if (pipe(fd)==-1)
	{
		fprintf(stderr, "pipe:%s\n",strerror(errno));
		exit(1);
	}

	if ((pid=fork())<0)
	{
		fprintf(stderr, "fork:%s\n",strerror(errno));
		exit(1);
	}else if (pid>0)//parent process 
	{
		close(fd[0]);//close the read part
		write(fd[1],sendBuffer,BUFFER_SIZE);
		printf("parent process send:%s\n", sendBuffer);
	}else
	{
		close(fd[1]);//close the write part
		read(fd[0],receiveBuff,BUFFER_SIZE); 
		printf("child process receive:%s\n",receiveBuff);
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
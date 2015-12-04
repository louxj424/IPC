#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define FIFO_NAME "/home/louxj/workspace/networking/ipc/myfifo"
#define PIPE_BUFF 12

int main(int argc, char const *argv[])
{
	char buffer[PIPE_BUFF];
	int pipe_id;
	int size = sizeof(buffer);
	
	if ((pipe_id=open(FIFO_NAME,O_RDWR))!=-1)
	{
		read(pipe_id,(void *)buffer,size);
		printf("%s\n",(char *)buffer);
	}else
	{
		fprintf(stderr, "open:%s\n", strerror(errno));
		exit(1);
	}
	return 0;
}
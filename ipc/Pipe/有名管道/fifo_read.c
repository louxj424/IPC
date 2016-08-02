#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define FIFO_NAME "/tmp/myfifo"
#define FIFO_BUFF 12

int main(int argc, char const *argv[])
{
	char buffer[FIFO_BUFF];
	int fifo_id;
	int size = sizeof(buffer);
	
	if ((fifo_id=open(FIFO_NAME,O_RDWR))!=-1)
	{
		read(fifo_id,(void *)buffer,size);
		printf("%s\n",(char *)buffer);
	}else
	{
		fprintf(stderr, "open:%s\n", strerror(errno));
		exit(1);
	}
	return 0;
}

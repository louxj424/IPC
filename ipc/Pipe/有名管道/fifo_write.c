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
	int res=0;
	int pipe_id;
	char buffer[PIPE_BUFF]="hello,fifo";

	if (access(FIFO_NAME,F_OK)==0)//test wether the file exits
	{
		if ((res=mkfifo(FIFO_NAME,0777)))!=0)
		{
			fprintf(stderr, "mkfifo:%s\n", strerror(errno));
		}
		if ((pipe_id=open(FIFO_NAME,O_RDWR))!=-1)
		{
			if (write(pipe_id,buffer,sizeof(buffer)))
			{
				printf("write success\n");
				close(pipe_id);
			}else
			{
				fprintf(stderr, "write:%s\n",strerror(errno));
				exit(1);
			}
		}else
		{
			fprintf(stderr, "open:%s\n",strerror(errno));
		}
	}
	return 0;
}
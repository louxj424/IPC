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
	int res=0;
	int fifo_id;
	char buffer[FIFO_BUFF]="hello,fifo";

	if (access(FIFO_NAME,F_OK)==0)//检查文件是否存在，此处为不存在
	{
		if (res=mkfifo(FIFO_NAME,0777))!=0)
		{
			fprintf(stderr, "mkfifo:%s\n", strerror(errno));
		}
		if ((fifo_id=open(FIFO_NAME,O_RDWR))!=-1)
		{
			if (write(fifo_id,buffer,sizeof(buffer)))
			{
				printf("write success\n");
				close(fifo_id);
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

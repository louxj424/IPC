#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <strings.h>


int main(int argc, char const *argv[])
{
	int stream;
	int lengthsize=0;
	char buffer[1024];
	bzero(buffer,sizeof(buffer));

	if ((stream=open("data1",O_RDWR))==-1)
	{
		fprintf(stderr, "open:%s\n", strerror(errno));
		exit(1);
	}else
	{
		printf("The file data1 was opened.\n");
	}
	if(lengthsize=read(stream,buffer,sizeof(buffer))>0)
	{
		printf("lengthsize=%d\n",lengthsize);
		printf("%s\n", buffer);
	}else
		printf("read error\n");
	close(stream);
	return 0;
}
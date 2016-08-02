#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>

#define PATH "/tmp"
#define SIZE 1024
#define ID 0

int main(int argc, char const *argv[])
{
	char * shmAddr;
	char * dataAddr="world";
	key_t key=ftok(PATH,ID);

	//create a shared memory area int the current pprocess memory
	int shmid;
	if ((shmid=shmget((key_t)key,SIZE,0666|IPC_CREAT))==-1)
	{
		fprintf(stderr, "shmget:%s\n", strerror(errno));
		exit(1);
	}


	shmAddr=shmat(shmid,(void*)0,0);//map the shared memory to the process memory
	if(shmAddr==(void*)-1)
	{
		fprintf(stderr, "shmat:%s\n", strerror(errno));
	}	

	printf("%s\n",shmAddr);


	shmdt(shmAddr);
	shmctl(shmid,IPC_RMID,NULL);

	return 0;
}

/*
[Test]
gcc -o shm_read shm_read.c
./shm_read
Hello

*/
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
	void *shmAddr=NULL;
	char dataAddr[]="Hello";
	key_t key=ftok(PATH,ID);//create an unique key for the current IPC
	
	//create a shared memory area in the current process memory
	int shmid;
	if ((shmid=shmget(key,SIZE,0666|IPC_CREAT))==-1)
	{
		fprintf(stderr, "shmget:%s\n", strerror(errno));
		exit(1);
	}



	shmAddr=shmat(shmid,(void*)0,0);//map the shared memory to the process memory
	if(shmAddr==(void*)-1)
	{
		fprintf(stderr, "shmat:%s\n", strerror(errno));
	}	
	strcpy(shmAddr,dataAddr);//copy the dataAddr to shmAddr
	



	//disconnect the process memory with the shared memory
	if (shmdt(shmAddr)==-1)
	{
		fprintf(stderr, "shmdt:%s\n", strerror(errno));
	}
	return 0;
}

/*
[Test]
gcc -o shm_write shm_write.c
./shm_write

*/
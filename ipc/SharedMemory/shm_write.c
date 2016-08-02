#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>

#define PATH "/tmp"
#define BUFFER_SIZE 1024
#define ID 0

int main(int argc, char const *argv[])
{
	void *shmAddr=NULL;
	char dataAddr[]="Hello";
	key_t key=ftok(PATH,ID);//为当前的进程间通信创建一个唯一key进行标识
	
	//创建一个共享内存段，返回值为该共享你内存块的id，即shmid
	int shmid;
	if ((shmid=shmget(key,BUFFER_SIZE,0666|IPC_CREAT))==-1)
	{
		fprintf(stderr, "shmget:%s\n", strerror(errno));
		exit(1);
	}

	//将共享内存段映射到进程地址空间
	//extern void *shmat (int __shmid, const void *__shmaddr, int __shmflg)
	shmAddr=shmat(shmid,(void*)0,0);
	if(shmAddr==(void*)-1)
	{
		fprintf(stderr, "shmat:%s\n", strerror(errno));
	}

	//将dataAddr拷贝至shmAddr
	strcpy(shmAddr,dataAddr);

	//disconnect the process memory with the shared memory
	//断开进程地址空间与共享内存的连接
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

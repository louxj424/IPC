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
	char * shmAddr;
	char * dataAddr="world";
	key_t key=ftok(PATH,ID);//为当前的进程间通信创建一个唯一key进行标识

	//创建一个共享内存段，返回值为该共享你内存块的id，即shmid
	int shmid;
	if ((shmid=shmget(key,BUFFER_SIZE,0666|IPC_CREAT))==-1)
	{
		fprintf(stderr, "shmget:%s\n", strerror(errno));
		exit(1);
	}

	//将共享内存段映射到进程地址空间
	shmAddr=shmat(shmid,(void*)0,0);
	if(shmAddr==(void*)-1)
	{
		fprintf(stderr, "shmat:%s\n", strerror(errno));
	}	

	printf("%s\n",shmAddr);

	//断开连接
	shmdt(shmAddr);
	//移除共享内存段
	shmctl(shmid,IPC_RMID,NULL);

	return 0;
}

/*
[Test]
gcc -o shm_read shm_read.c
./shm_read
Hello
，执行完成之后程序立即退出
*/

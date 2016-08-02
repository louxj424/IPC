#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_FILE "/tmp"       //一个产生唯一的key的路径名
#define BUFFER 512            //设置缓冲区大小为512字节
#define PERM S_IRUSR|S_IWUSR  //允许用户读写

struct msgbuffer
{	
	long mtype;
	char mtext[BUFFER+1];
};
typedef struct msgbuffer msgbuf;

int main(int argc, char const *argv[])
{
	//创建一个唯一的key
	key_t key;	
	if ((key=ftok(MSG_FILE,BUFFER))==-1)//使用同样的参数产生一个与发送端相同的key
	{
	 	fprintf(stderr, "ftok:%s\n", strerror(errno));
	 	exit(1);
	}else
	{
		printf("generate a key=%d\n", key);
	}

	//产生一个消息队列
	int msgid;	
	msgbuf msg;
	if ((msgid=msgget(key,PERM|IPC_CREAT))==-1)
	{
		fprintf(stderr, "msgget:%s\n", strerror(errno));
	 	exit(1);
	}

	//从消息队列中接收消息
	int i;
	for (i = 0; i < 3; ++i)
	{
	    //从消息队列中接收消息
	    //extern ssize_t msgrcv (int __msqid, void *__msgp, size_t __msgsz,long int __msgtyp, int __msgflg);
	    //TODO：判断操作的是否成功
		msgrcv(msgid,&msg,sizeof(msgbuf),1,0);
		printf("Receiver receive: %s\n", msg.mtext);
	}

	//发送一条消息以提示发送者
	//为了与发送者消息类型进行区分，这里设定自定义消息类型为2
	msg.mtype=2;
	char * myask="3 messages have received from you";
	strncpy(msg.mtext,myask,BUFFER);
	//以费阻塞的方式将消息发送出去
	//TODO：判断操作的是否成功
	msgsnd(msgid,&msg,sizeof(msgbuf),IPC_NOWAIT);

	return 1;
}


/*
[Test]
gcc -o msg_receiver msg_receiver.c
./msg_reveiver

generate a key=589827
Receiver receive: I'm sender,there are some messages for you.
Receiver receive: Message 1
Receiver receive: Message 2


*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>  //for msg_get(),msg_rcv() 

#define MSG_FILE "/tmp"       //a pathname for generating a unique key
#define BUFFER 512            //set the buffer size to 255 bytes
#define PERM S_IRUSR|S_IWUSR  //allow the user to read and write

struct msgbuffer
{	
	long mtype;
	char mtext[BUFFER+1];
};
typedef struct msgbuffer msgbuf;

//create three message
char* message[3]={"I'm sender,there are some messages for you.","Message 1","Message 2"};

int main(int argc, char const *argv[])
{
	msgbuf msg;//自定义的消息格式
	key_t key;//发送和接收约定的唯一通信标识
	int msgid;//消息队列的ID

	//利用ftok产生一个唯一的key，参数为路径和缓冲区大小
	if ((key=ftok(MSG_FILE,BUFFER))==-1)
	 {
	 	fprintf(stderr, "ftok:%s\n", strerror(errno));
	 	exit(1);
	 }else
	{
		printf("generate a key=%d\n", key);
	}

	//产生一个消息队列，IPC_CREAT表示创建一个key如果key不存在的话
	if ((msgid=msgget(key,PERM|IPC_CREAT))==-1)
	{
		fprintf(stderr, "msgget:%s\n", strerror(errno));
	 	exit(1);
	}

	//设置消息的类型
	msg.mtype=1;

	//发送三个消息给接收者
	int i;
	for (i = 0; i <3; ++i)
	{
	    //将字符消息拷贝到消息队列消息格式中
	    //char * strncpy (char *restrict to, const char *restrict from, size_t size)
	 	strncpy(msg.mtext,message[i],BUFFER);
	 	//以非阻塞的方式发送，如果发送失败则返回错误
	 	msgsnd(msgid,&msg,sizeof(msgbuf),IPC_NOWAIT);
	}

	//接收来自接收者的响应
	memset(&msg,'\0',sizeof(msgbuf));//清空缓存msgbuf
	/*receive the message,the third arguments show the type of message will be received */
	//接收消息，第三个参数显示将被接收的消息类型
	msgrcv(msgid,&msg,sizeof(msgbuf),2,0);
	printf("This is a message from the receiver,%s\n", msg.mtext);


	//删除消息队列，msgctl为消息队列的控制操作，通过传入参数进行操作处理，IPC_RMID表示移除标识符
	if (msgctl(msgid,IPC_RMID,0)==-1)
	{
		fprintf(stderr, "ftok:%s\n", strerror(errno));
	 	exit(1);
	}
	return 1;
}

/*
[Test]
gcc -o msg_sender msg_sender.c
./msg_sender

generate a key=589827
This is a message from the receiver,3 messages have received from you

*/

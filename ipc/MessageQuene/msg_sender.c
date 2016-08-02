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
	msgbuf msg;
	key_t key;
	int msgid;

	//create a unique key
	if ((key=ftok(MSG_FILE,BUFFER))==-1)
	 {
	 	fprintf(stderr, "ftok:%s\n", strerror(errno));
	 	exit(1);
	 }else
	{
		printf("generate a key=%d\n", key);
	}

	//get a message queue 
	if ((msgid=msgget(key,PERM|IPC_CREAT))==-1)
	{
		fprintf(stderr, "msgget:%s\n", strerror(errno));
	 	exit(1);
	}

	//set the type of the message
	msg.mtype=1;

	//send three messages to the receiver
	int i;
	for (i = 0; i <3; ++i)
	{
	 	strncpy(msg.mtext,message[i],BUFFER);
	 	msgsnd(msgid,&msg,sizeof(msgbuf),IPC_NOWAIT);
	}

	//receive the response from the receiver
	memset(&msg,'\0',sizeof(msgbuf));//clear the msgbuf 
	/*receive the message,the third arguments show the type of message will be received */
	msgrcv(msgid,&msg,sizeof(msgbuf),2,0);
	printf("This is a message from the receiver,%s\n", msg.mtext);


	//delete the message queue.
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

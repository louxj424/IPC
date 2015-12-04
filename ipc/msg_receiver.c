#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_FILE "/tmp"       //a pathname for generating a unique key
#define BUFFER 512            //set the buffer size to 255 bytes
#define PERM S_IRUSR|S_IWUSR  //allow the user to read and write

struct msgbuffer
{	
	long mtype;
	char mtext[BUFFER+1];
};
typedef struct msgbuffer msgbuf;

int main(int argc, char const *argv[])
{
	//create a unique key
	key_t key;	
	if ((key=ftok(MSG_FILE,BUFFER))==-1)
	{
	 	fprintf(stderr, "ftok:%s\n", strerror(errno));
	 	exit(1);
	}else
	{
		printf("generate a key=%d\n", key);
	}

	//get a message queue 
	int msgid;	
	msgbuf msg;
	if ((msgid=msgget(key,PERM|IPC_CREAT))==-1)
	{
		fprintf(stderr, "msgget:%s\n", strerror(errno));
	 	exit(1);
	}

	//get a message from the queue everytime
	int i;
	for (i = 0; i < 3; ++i)
	{
		msgrcv(msgid,&msg,sizeof(msgbuf),1,0);
		printf("Receiver receive: %s\n", msg.mtext);
	}

	//send the message to notice the sender
	msg.mtype=2;
	char * myask="3 messages have received from you";
	strncpy(msg.mtext,myask,BUFFER);
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
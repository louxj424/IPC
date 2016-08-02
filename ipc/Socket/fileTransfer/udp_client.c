/*
**     client端代码:udp_client.c
**    send the file to the server
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define    FINISH_FLAG    "FILE_TRANSPORT_FINISH"//define the end of the transmission
#define    MAXLINE        1024

void usage(char *command)
{
    printf("usage :%s [ipaddr] [port] [filename]\n", command);
    exit(0);
}


int main(int argc,char **argv)
{
    FILE                  *fp;
    struct sockaddr_in    serv_addr;
    char                  buf[MAXLINE];
    int                    sock_id;
    int                    read_len;
    int                    send_len;
    int                    serv_addr_len;
    int                    res;
    int                    i;

    if (argc != 4) {
        usage(argv[0]);
    }

    /* open the file to be transported */
    if ((fp = fopen(argv[3],"r")) == NULL) {
        perror("Open file failed\n");
        exit(0);
    }

    /* create the socket */
    if ((sock_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Create socket failed");
        exit(0);
    }


    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);


    /* connect the server */
    serv_addr_len = sizeof(serv_addr);
    res= connect(sock_id, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
    if (-1 == res) {
        perror("Connect socket failed!\n");
        exit(0);
    }

    /* transport the file*/
    bzero(buf, MAXLINE);
    while ( (read_len = fread(buf, sizeof(char), MAXLINE, fp)) > 0 ) {
        send_len = send(sock_id, buf, read_len, 0);
        if ( send_len < 0 ) {
            perror("Send data failed\n");
            exit(0);
        }
        bzero(buf, MAXLINE);
    }
    fclose(fp);


    /* send the end_flag */
    bzero(buf, MAXLINE);
    strcpy(buf, FINISH_FLAG);
    buf[strlen(buf)] = '\0';//append '\o' to the end of the end_flag 
    for (i = 1000; i>0; i--) {//try at most 1000 times
        send_len = send(sock_id, buf, strlen(buf)+1, 0);
        if ( send_len > 0 ) {
            printf("Finish send the end string\n");
            break;
        }
    }

    close(sock_id);
    printf("Send file finish\n");
    return 0;
}
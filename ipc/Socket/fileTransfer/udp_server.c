/*
**  server端代码:udp_server.c
**  receive the file from the client
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define    FINISH_FLAG    "FILE_TRANSPORT_FINISH"
#define    MAXLINE        1024

void usage(char *command)
{
    printf("usage :%s  [port]  [filename]\n", command);
    exit(0);
}

int main(int argc,char **argv)
{
    struct sockaddr_in    serv_addr;
    struct sockaddr_in    clie_addr;
    char                 buf[MAXLINE];
    int                    sock_id;
    int                    recv_len;
    int                    clie_addr_len;
    FILE                  *fp;

    if (argc != 3) {
        usage(argv[0]);
    }

    //create the file to send
    if ((fp = fopen(argv[2], "w")) == NULL) {
        perror("Creat file failed");
        exit(0);
    }

    //create the socket for the server
    if ((sock_id = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
        perror("Create socket failed\n");
        exit(0);
    }

    //fill the server sockaddr_in struct 
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind the socket with the sockaddr_in
    if (bind(sock_id,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0 ) {
        perror("Bind socket faild\n");
        exit(0);
    }

    // server:receive the file data  
    clie_addr_len = sizeof(clie_addr);
    bzero(buf, MAXLINE); 
    while (recv_len = recvfrom(sock_id, buf, MAXLINE, 0,(struct sockaddr *)&clie_addr, &clie_addr_len)) {
        if(recv_len < 0) {
            printf("Recieve data from client failed!\n");
            break;
        }
        printf("#");
        //to judge the if FINISH_FLAG is the sub string of the buf
        if ( strstr(buf, FINISH_FLAG) != NULL ) {
            printf("\nFinish receiver finish_flag\n");
            break;
        }

        //write received data from  buf to the created file
        int write_length = fwrite(buf, sizeof(char), recv_len, fp);
        if (write_length < recv_len) {
            printf("File write failed\n");
            break;
        }

        //clear the buf for the another receive
        bzero(buf, MAXLINE);
    }

    //close the socket and release the resources
    printf("Finish recieve\n");
    fclose(fp); 
    close(sock_id); 
    return 0;
}
/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<signal.h>
 int sockfd; 
void my_handler_for_sigint(int );
struct my_msgbuf {
    long mtype;
    char mtext[200];
};
 // get sockaddr, IPv4or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    int numbytes;  
    struct my_msgbuf bufa;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 3) {
        fprintf(stderr,"Ip and Port number is missing\n");
        exit(1);
    }

     /* Registering the Signal handler */
  if (signal(SIGINT, my_handler_for_sigint) == SIG_ERR)
      printf("\ncan't catch SIGINT\n");
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure
printf("Please enter the data\n");

//Waiting for the messages from user
while(fgets(bufa.mtext, sizeof bufa.mtext, stdin) != NULL) {
        int len = strlen(bufa.mtext);

        /* ditch newline at end, if it exists */
        if (bufa.mtext[len-1] == '\n')
	 bufa.mtext[len-1] = '\0';
 if (send(sockfd, &(bufa.mtext), len+1, 0) == -1)
                perror("send");  }  
return 0;
}
void my_handler_for_sigint(int signumber)//signal handler
{
  char ans[2];
  if (signumber == SIGINT)
  {
    printf("received SIGINT\n");
    printf("Program received a CTRL-C\n");
    printf("Terminate Y/N : "); 
    scanf("%s", ans);
    if (strcmp(ans,"Y") == 0)
    {
       printf("Exiting ....\n");
       close(sockfd);       
       exit(0); 
    }
    else
    {
       printf("Continung ..\n");
    }
  }
}















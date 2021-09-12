/*Fatih Unal*/
/*1250885*/

/*necessary libraries*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

/*for error condition*/
void error(char *msg)
{
    perror(msg);
    exit(0);
}

int debugclient;

/*I found this server-client connection part only from the web but I wrote all of the modules of the communication on my own*/
int main(int argc, char *argv[])
{
    int sockfd, portno, n,m;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256]; /*socket buffer*/

    /*evaluating -p -d options*/
    if (argc < 5) {
       fprintf(stderr,"usage: %s -s servername -p portnumber [-d]\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[4]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[2]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
m=0;
n=0;

/*controls whether the debugging option is set at the beginning*/
if(argc==6 && strcmp(argv[5],"-d")==0)
{	        
	debugclient=1; 
}
else
debugclient=0;

/*continuous loop for server client connection*/
while(1)
{
/*command line %*/
printf("%%");

/*clearing buffer and writing and sending the command to the server*/
bzero(buffer,256);
fgets(buffer,256,stdin);
write(sockfd,buffer,strlen(buffer)-1);

/*time command to server and its reply*/
if(strcmp(buffer,"time\n")==0)
{                
	if(debugclient==1)
        {
                printf("Client 1 asked for current time\n");
                printf("Client 1 responded\n");
        }
bzero(buffer,256);
read(sockfd,buffer,256);
printf("%s\n",buffer);
}else

/*diff command to server and its reply*/
if(strcmp(buffer,"diff\n")==0)
        {
                if(debugclient==1)
                {
                printf("Client 1 asked for time difference\n");
                printf("Client 1 responded\n");
                }
bzero(buffer,256);
read(sockfd,buffer,256);
printf("%s\n",buffer);
}else

/*think command to server and its reply*/
if(strncmp(buffer,"think ",6)==0)
        {
                if(debugclient==1)
                {
                printf("Client 1 asked for think time\n");
                printf("Client 1 responded\n");
                }
bzero(buffer,256);
read(sockfd,buffer,256);
printf("%s\n",buffer);
}else 

/*unknown command to server and its reply*/
{
                if(debugclient==1)
                {
                printf("Client1 sent an error message\n");
                printf("Client 1 responded\n");
                }
bzero(buffer,256);
read(sockfd,buffer,256);
printf("%s\n",buffer);
}

/*quit command to server and its reply*/
if(strcmp(buffer,"quit")==0){break;}
}
    return 0;
}


/*Fatih Unal*/
/*1250885*/

/*necessary libraries*/

#define _REENTRANT
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <netdb.h>
#include <unistd.h> 
#include <time.h>
#define SIZE 256

/* function prototypes */
void* serverWatch( void* );
void* serveClient( void* );
void error(char *msg)
{
    perror(msg);
    exit(1);
}

/*flags for debug options*/
int debugserver;
int flag;

/*think time*/
int wait;

/*time difference*/
int dif;

/*for random port option*/
int max=8000;
int portno;

/*time buffer*/
char timebuffer[SIZE];

/*for time functions and structures*/
time_t tim1;
time_t tim2;
struct tm *now;


/*threaded server code*/
int main(int argc, char *argv[])
{

  pthread_t watcher_thr;
		     

/*for evaluating -p -d options*/
  
if (strcmp(argv[1],"-p")!=0) 
{
	srand(1);
	portno=rand()/(int)(((unsigned)RAND_MAX + 1) / max);
	printf("random port number:%d\n",portno);
}

if(strcmp(argv[1],"-p")==0) 
{		
	portno = atoi(argv[2]);	
}

if(argc==4 && strcmp(argv[3],"-d")==0)
{
	debugserver=1;
}

pthread_create(&watcher_thr, NULL, serverWatch, (void*)NULL);

/*continuous loop for clients*/
  for(;;) {sleep(1);}
}

/*I found this threaded server-client connection part only from the web but I wrote all of the modules of the communication on my own*/
void* serverWatch( void* dummy )
{
  pthread_t dummy_thr;
  int srv_socket;
  int accepted_socket;
  int size;
  struct sockaddr_in port_addr, accept_addr;

  int set_opt = 1;
  
  fd_set read_set;
  int ready_fd;

  /* set up the socket on the server port */
  port_addr.sin_family = AF_INET;
  port_addr.sin_addr.s_addr = htonl( INADDR_ANY );
  port_addr.sin_port = htons(portno);

  srv_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  setsockopt( srv_socket, SOL_SOCKET, SO_REUSEADDR,
	      (char*)&set_opt, sizeof( set_opt ) );  
  bind( srv_socket, (struct sockaddr*)&port_addr, sizeof(port_addr));

  for(;;) {
    /* wait for the client to try to connect */
    listen( srv_socket,1 );

    FD_ZERO(&read_set);
    FD_SET( srv_socket, &read_set );

    do {
      ready_fd = select(srv_socket+1, &read_set, NULL, NULL, NULL);
    }
    while (ready_fd <= 0 || !FD_ISSET(srv_socket, &read_set) );

    /* a client has connected. Accept their connection and create a
     * thread to serve the client
     */
    size = sizeof( accept_addr );
    accepted_socket = accept(srv_socket, (struct sockaddr*)&accept_addr,
			      &size);
    pthread_create(&dummy_thr, NULL, serveClient, (void*)accepted_socket);
  }
}


void* serveClient( void* socket )
{
	
/*time variables*/
int duration=0;
int hour=0;
int minute=0;
int second=0;
int n=0;

/*time buffers*/
char buffer[256];
char number1[10];
char number2[10];
char number3[10];

flag=0;
wait=0;


/*if debug option is enabled it enables the comments*/
if(debugserver==1)
{
        printf("Server: Client 1 connected\n");
        printf("Debug flag is set\n");
}       
               
/*continuous loop for clients*/
while(1)
{
	/*command from client is read here*/
     	bzero(buffer,256);
	n=read((int)socket,buffer,255);
if(n>0)
{	      
	if(strcmp(buffer,"time")==0)   /*client is served for time command*/
	{
		if(debugserver==1)
		{
		printf("Client 1 asked for current time\n");
		printf("Client 1 is responded\n");
		}

		tim1=time(NULL);
             	now=gmtime(&tim1);
             	strftime (timebuffer, SIZE, "%a %b %d %H:%M:%S %Z %Y", now);
                
		fputs (timebuffer, stdout);
		printf("\n");
		
		/*current time is sent to client*/
		write((int)socket,timebuffer,strlen(timebuffer));
		continue;
	
	} else if(strcmp(buffer,"diff")==0)	/*client is served for diff command*/
	{
		if(debugserver==1)
		{
		printf("Client 1 asked for time difference\n");         
                printf("Client 1 responded\n");
		}		                

		/*it controls the think <time> option whether it is set before*/
		if(flag==0)
		{
			tim2=time(NULL);
			
		}else if(flag==1)
			{
				sleep(wait);				                        
				tim2=time(NULL);
			}


		dif=difftime(tim2,tim1)/1;
		
		/*time calculations for the time difference between two command*/
		/*all of the variables are initialized*/
		duration=0;      
                hour=0;
                minute=0;
                second=0;
		
		duration=dif;
		hour=duration/3600;
		minute=duration/60;
		second=duration-(hour*3600)-(minute*60);

		/*clears the arrays*/
		 bzero(number1,10);
		 bzero(number2,10);
		 bzero(number3,10);
		
		/*hour calculation whether it is singular or plural*/
		if(hour>0)
		{
			if(hour==1)
			{
			bzero(number1,10);
			
			/*!!!sprintf function converts the integer to string!!!*/
                	sprintf(number1,"%d",hour);
			strcat(number1," hour  ");
			}
			else
			{
			bzero(number1,10);
                        sprintf(number1,"%d",hour);
			strcat(number1," hours  ");
			}
		}
		
		/*minute calculation whether it is singular or plural*/
		if(minute>0)
                {        
                        if(minute==1)
			{
			bzero(number2,10);
                        sprintf(number2,"%d",minute);
			strcat(number2," minute  ");
			}
                        else
			{
			bzero(number2,10);
                        sprintf(number2,"%d",minute);
			strcat(number2," minutes  ");
			}
                }

		/*second calculation whether it is singular or plural*/
                if(second>0)
                {        
                        if(second==1)
			{
			bzero(number3,10);
                        sprintf(number3,"%d",second);
			/*here hour minute and second strings are concatenated*/
			strcat(number3," second  ");
			strcat(number1,number2);
                        strcat(number1,number3);
			write((int)socket,number1,strlen(number1));
			printf("%s\n",number1);

			}
                        else
			{
			bzero(number3,10);
                        sprintf(number3,"%d",second);
                        /*here hour minute and second strings are concatenated*/
			strcat(number3," seconds  ");
			strcat(number1,number2);
                        strcat(number1,number3);  
			write((int)socket,number1,strlen(number1));
			printf("%s\n",number1);
			}
                }					
		
		flag=0;
		continue;

	} else if(strncmp(buffer,"think ",6)==0)	/*client is served for think command*/
	{
		if(debugserver==1)
		{
		printf("Client 1 asked for think time\n");         
                printf("Client 1 responded\n"); 
		}		                

		flag=1;
		write((int)socket,"Agreed",5);
		/*think <time> is calculated*/
		wait=atoi(strrchr(buffer,' ')+1);
		continue;

	} else if(strcmp(buffer,"quit")==0)	/*client is served for quit command*/
	{
		if(debugserver==1)
		{             
		printf("Client1 sent quit command\n");
		}
		
		/*socket is closed for that client*/
		write((int)socket,"quit",4);
		close( (int)socket );
		pthread_exit( NULL );
		break;

	} else if(n>0)		/*client is served for unknown command*/
	{
		
		if(debugserver==1)
                {
                printf("Client1 sent an error message\n");
                printf("Client 1 responded\n");
                }
		
		write((int)socket,"Error Message",13);
		continue;
	}
}
}
 

return 0;
}


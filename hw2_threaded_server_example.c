/**************************
**
** simple multi-threaded server
**
**
*/

#define _REENTRANT

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

/* the port on which to accept connections */
#define SERVERPORT 12345

/* function prototypes */
void* serverWatch( void* );
void* serveClient( void* );


int main( void )
{

  pthread_t watcher_thr;

  pthread_create(&watcher_thr, NULL, serverWatch, (void*)NULL);

  for(;;) {
    /* do something useful */
    printf( "server is running\n" );
    pthread_yield();
    sleep(1);
  }
}


/*************
**
** serverWatch()
**
** watch for a client trying to connect
**
** the argument to the function is not used
**
*/
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
  port_addr.sin_port = htons( SERVERPORT );

  srv_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  setsockopt( srv_socket, SOL_SOCKET, SO_REUSEADDR,
	      (char*)&set_opt, sizeof( set_opt ) );  
  bind( srv_socket, (struct sockaddr*)&port_addr, sizeof(port_addr));

  for(;;) {
    /* wait for the client to try to connect */
    listen( srv_socket, 5 );

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


/*************
**
** serve the client on the specified socket
**
*/
void* serveClient( void* socket )
{
  char buffer[256];

  for(;;) {
    strcpy( buffer, "Type 'X' to quit\n" );
    write( (int)socket, buffer, strlen(buffer) );

    if (read( (int)socket, buffer, 255) > 0 ) {
      if ( buffer[0] == 'X' ) {
	close( (int)socket );
	pthread_exit( NULL );
      }
    }
  }
}


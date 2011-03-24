#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "micetrap.h"

#define BUFSIZE 1024

const char* program_name = "micetrap";
const char* program_version = "0.0.1";

void error(char *msg) {
  perror(msg);
  exit(1);
}

void show_banner() {
  printf("Usage:\n\t\t[sudo] %s <service> [options]", program_name);
  printf("\n\t\t\t-p --port\t[PORT]\t\tA specific port to use\n");
}

void show_version() {
  printf("%s-%s", program_name, program_version);
}

void
populate_service_and_port_from_options(char *service, int *port, int argc, char* argv[])
{

  int next_option;

  const char* const short_options = "hpv:";
  const struct option long_options[] =
    {
      { "help",     0,  NULL,   'h' },
      { "version",  0,  NULL,   'v' },
      { "port",     1,  NULL,   'p' },
      { "service",  1,  NULL,   's' },
      { NULL,       0,  NULL,   0   },
    };

  char*        service_name = NULL;
  unsigned int port_number  = NULL;

  if (argc == 1)
  {
    show_banner();
    exit(EXIT_SUCCESS);
  }

  while(1)
  {

    next_option = getopt_long (argc, argv, short_options, long_options, NULL);

    if (next_option == -1)
    {
      break;
    }

    switch(next_option)
    {
      case 'h' :
        show_banner();
        exit(EXIT_SUCCESS);

      case 'v' :
        show_version();
        exit(EXIT_SUCCESS);

      case 'p' :
        printf("Given a port argument: %s\n", optarg);
        port_number = (int)optarg;
        break;

      case 's' :
        printf("Given a service argument: %s\n", optarg);
        service_name = (char*)optarg;
        break;

      case -1 :
        break;

      default :
        abort();
    }
  
  }

  if (optind < argc)
  {
    printf("Unrecognized options:\n");
    while (optind < argc)
      printf("\t\t%s\n", argv[optind++]);
  }

  if (service_name == NULL)
  {
    printf("You must provide a valid service name.");
    exit(-1);
  }

  service = service_name;
  port = port_number;
  printf("After Service: %s\nPort: %i", service, port);
}

int main(int argc, char **argv) {

  char*        service = NULL;
  unsigned int port    = NULL;

  populate_service_and_port_from_options(&service, &port, argc, argv);

  printf("Service: %s\nPort: %i", service, port);

  int parentfd; /* parent socket */
  int childfd; /* child socket */
  int portno; /* port to listen on */
  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp; /* client host info */
  char buf[BUFSIZE]; /* message buffer */
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  int n; /* message byte size */

  /* 
   * check command line arguments 
   */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  portno = atoi(argv[1]);

  /* 
   * socket: create the parent socket 
   */
  parentfd = socket(AF_INET, SOCK_STREAM, 0);
  if (parentfd < 0) 
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets 
   * us rerun the server immediately after we kill it; 
   * otherwise we have to wait about 20 secs. 
   * Eliminates "ERROR on binding: Address already in use" error. 
   */
  optval = 1;
  setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));

  /* this is an Internet address */
  serveraddr.sin_family = AF_INET;

  /* let the system figure out our IP address */
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* this is the port we will listen on */
  serveraddr.sin_port = htons((unsigned short)portno);

  /* 
   * bind: associate the parent socket with a port 
   */
  if (bind(parentfd, (struct sockaddr *) &serveraddr, 
	   sizeof(serveraddr)) < 0) 
    error("ERROR on binding");

  /* 
   * listen: make this socket ready to accept connection requests 
   */
  if (listen(parentfd, 5) < 0) /* allow 5 requests to queue up */ 
    error("ERROR on listen");

  /* 
   * main loop: wait for a connection request, echo input line, 
   * then close connection.
   */
  clientlen = sizeof(clientaddr);
  while (1) {

    /* 
     * accept: wait for a connection request 
     */
    childfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
    if (childfd < 0) 
      error("ERROR on accept");
    
    /* 
     * gethostbyaddr: determine who sent the message 
     */
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
			  sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL)
      error("ERROR on gethostbyaddr");
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL)
      error("ERROR on inet_ntoa\n");
    printf("server established connection with %s (%s)\n", 
	   hostp->h_name, hostaddrp);
    
    /* 
     * read: read input string from the client
     */
    bzero(buf, BUFSIZE);
    n = read(childfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("server received %d bytes: %s", n, buf);
    
    /* 
     * write: echo the input string back to the client 
     */
    n = write(childfd, buf, strlen(buf));
    if (n < 0) 
      error("ERROR writing to socket");

    close(childfd);
  }
}

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "server.h"

void die(const char *message)
{
  if (errno) {
    perror(message);
  } else {
    printf("ERROR: %s\n", message);
  }

  exit(1);
}

void Server_start(int port)
{
  // Declare file descriptors
  int listen_fd, accept_fd;

  // Declare server and client addresses
  struct sockaddr_in server, client;

  unsigned int sin_size;

  // Try to open the socket
  listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd == -1) die("Could not open socket.");

  server.sin_family = AF_INET;
  server.sin_port = htons(port);

  // Set our IP address automatically
  server.sin_addr.s_addr = INADDR_ANY;

  // Fil the rest of the struct with zeros
  bzero(&(server.sin_zero), 8);

  if (bind(listen_fd, (struct sockaddr* )&server, sizeof(server)) == -1) {
    die("Could not bind the socket.");
  }

  printf("Server ready and listening on port %d...", port);
  listen(listen_fd, 5);

  while(1) {
    sin_size = sizeof(struct sockaddr_in);

    // Accept incoming connection
    if ((accept_fd = accept(listen_fd,
                            (struct sockaddr *)&client,
                            &sin_size)) == -1) {
      die("Could not accept incoming connection.");
    }

    printf("Incoming connection from %s\n", inet_ntoa(client.sin_addr));

    char buf[255];
    int n;
    bzero(buf, 256);
    n = recv(accept_fd, buf, 255, 0);
    printf("Message received: %s", buf);

    send(accept_fd, "Welcome to my server.\n", 22, 0);

    // Close file descriptor
    close(accept_fd);
  }
}

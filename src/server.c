#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "utils.h"
#include "log.h" // Always require after utils.h
#include "service.h"
#include "server.h"

/* TODO: Clean up file descriptors when exiting. */
void goodbye()
{
  printf("Goodbye!");
}

void Server_start(Service *service, int port)
{
  // If no port is specified, get it from the service
  if (port == 0) {
    port = Service_random_port(service);
  }

  // Trap SIGINT and call goodbye()
  signal(SIGINT, goodbye);

  // Declare file descriptors
  int listen_fd, accept_fd;

  int received = 0;

  char *response = Service_random_response(service);
  int response_length = strlen(response);

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

  log("Fake %s server ready and listening on port %d...", service->name, port);
  listen(listen_fd, 5);

  while(1) {
    sin_size = sizeof(struct sockaddr_in);

    // Accept incoming connection
    if ((accept_fd = accept(listen_fd,
                            (struct sockaddr *)&client,
                            &sin_size)) == -1) {
      die("Could not accept incoming connection.");
    }

    log("Incoming connection from %s", inet_ntoa(client.sin_addr));

    // Receive the probe
    char buf[256];
    received = recv(accept_fd, buf, 255, 0);
    log("Message received: %s", buf);
    if (received == 1) {
      send(accept_fd, response, response_length, 0);
      log("Sent a fake probe: %s", response);
    }

    // Close the connection
    close(accept_fd);
  }
}

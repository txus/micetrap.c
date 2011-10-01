#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "server.h"

int main(int argc, char *argv[])
{
  if (argc < 2) die("USAGE: micetrap <port>");

  int port = atoi(argv[1]);

  printf("Starting server...\n");

  Server_start(port);
  return 0;
}

#include <stdio.h>
#include "server.h"

int main(int argc, char *argv[])
{
  printf("Starting server...\n");
  Server_start(9999);
  return 0;
}

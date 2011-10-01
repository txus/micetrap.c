#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "service.h"
#include "server.h"

int main(int argc, char *argv[])
{
  /* if (argc < 2) die("USAGE: micetrap <service> <port>"); */

  int port = 0;
  if (argc == 3) {
    port = atoi(argv[2]);
  }

  /* Service *service = Service_build(argv[1]); */
  Service *service = Service_build("ftp");

  Server_start(service, 9999);
  return 0;
}

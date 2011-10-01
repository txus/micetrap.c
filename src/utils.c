#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "utils.h"

void die(const char *message)
{
  if (errno) {
    perror(message);
  } else {
    printf("ERROR: %s\n", message);
  }

  exit(1);
}

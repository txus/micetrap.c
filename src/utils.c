#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>
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

char *timestamp()
{
  time_t ltime;
  char* retval;

  ltime = time(NULL);
  retval = asctime(localtime(&ltime));

  retval = sub(retval, '\n', '\0');

  return retval;
}

char *sub(char *string, char from, char to)
{
  int length = strlen(string);
  int i = 0;

  char *buffer = malloc(sizeof(char) * length);

  for(i = 0; i < length-1; i++) {
    if (string[i] == from) {
      buffer[i] = to;
    } else {
      buffer[i] = string[i];
    }
  }

  return buffer;
}

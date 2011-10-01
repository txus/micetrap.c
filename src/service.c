#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "service.h"

Service *Service_build(const char *name)
{
  Service *service = malloc(sizeof(Service));
  if(!service) die("Out of memory.");

  if (strcmp(name, "ftp") == 0) {
    int ports[] = {22};
    service->ports = ports;

    char *responses[] = {
      "220-FTP server (lukemftpd 1.1) ready.\r\n",
      "220 Welcome to Pure-FTPd 1.8\r\n",
      "220--------------------------------------------------------------------------------\r\n220-This is the \"Banner\" message for the Mac OS X Server's FTP server process.\r\n",
    };
    service->responses = responses;
    service->response_count = 3;

  } else if (strcmp(name, "http") == 0) {
    int ports[] = {80, 8080};
    service->ports = ports;

    char *responses[] = {
      "HTTP/1\.0 200 OK\r\nContent-Type: text/html\r\n\r\n<html>\n<body>\n<ul><li>\n<i>com\.apple\.KernelEventAgent</i>\n"
    };
    service->responses = responses;
    service->response_count = 1;

  } else {
    die("Unrecognized service name.");
  }

  service->name = strdup(name);

  return service;
}

int Service_random_port(Service *service)
{
  unsigned int iseed = (unsigned int)time(NULL);
  srand(iseed);

  int len = sizeof(service->ports) / sizeof(int);
  int idx = rand() % len;

  return service->ports[idx];
}

char *Service_random_response(Service *service)
{
  unsigned int iseed = (unsigned int)time(NULL);
  srand(iseed);

  int idx = rand() % service->response_count;

  return *service->responses[idx];
}

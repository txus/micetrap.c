#ifndef __service_h__
#define __service_h__

struct Service {
  char *name;
  int *ports;
  int response_count;
  char **responses;
};

typedef struct Service Service;

Service *Service_build(const char *name);
int Service_random_port(Service *service);

#endif

#ifndef __log_h__
#define __log_h__

#define log(M, ...) fprintf(stdout, "[%s] " M "\n", timestamp(), ##__VA_ARGS__)

#endif

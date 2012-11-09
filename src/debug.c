#include "debug.h"

void debug_init(void)
{
  gbuffer = "/tmp/mypas";

  mkfifo(gbuffer, 0666);

  pthread_create(&t_cdb,NULL,call_cdb,NULL);

  debugfd = open(gbuffer, O_WRONLY);

}

void debug_finalize(void)
{
  close(debugfd);
  unlink(gbuffer);
}

void debug_send_message(char *message)
{
  write(debugfd, "Hi", sizeof("Hi"));
}

int debug_isinitialized(void)
{
  return debugfd;
}

void *call_cdb()
{
  char *arguments[1];
  int pid; 

  printf("opa\n");

  pid = fork();
  if(!pid){
    arguments[0] = strdup("/usr/bin/cdb");
    execv("/usr/bin/cdb",arguments);
  }else{
    fprintf(stderr, "Não foi possível executar o debugger.\n");
    exit(1);
  }
}


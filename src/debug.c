#include "debug.h"

initialized = 0;

void debug_init(void)
{

  gbuffer = "/tmp/mypas";

  mkfifo(gbuffer, 0666);

  pthread_create(&t_cdb,NULL,call_cdb,NULL);

  initialized = 1;

}

void debug_finalize(void)
{
  pthread_cancel(t_cdb);

  kill(pidofcdb, SIGKILL);

  unlink(gbuffer);
}

void debug_send_message(char *message)
{
  debugfd = open(gbuffer, O_WRONLY);
  write(debugfd, message, strlen(message));
  close(debugfd);
}

int debug_isinitialized(void)
{
  return initialized;
}

void *call_cdb()
{
  char *arguments[1];

  pidofcdb = fork();
  if(!pidofcdb){
    arguments[0] = strdup("/usr/bin/cdb");
    execv("/usr/bin/cdb",arguments);
  }
}


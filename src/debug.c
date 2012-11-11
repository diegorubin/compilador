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
  debugfd = open(gbuffer, O_WRONLY | O_FSYNC | O_NDELAY);
  write(debugfd, message, strlen(message));
  close(debugfd);
}

int debug_isinitialized(void)
{
  return initialized;
}

/*TODO otimizar esse metodo*/
FILE* debug_change_sourcecode(FILE *in)
{
  int n = 0;
  char c;
  char tmpout[1000000];

  while((c = getc(in)) > 0){
    tmpout[n++] = c;
  }
  tmpout[n] = '\0';
  
  return fmemopen (tmpout, strlen(tmpout), "r");
}

void debug_send_sourcecode(FILE *source)
{
  int n = 0;
  char c;
  char tmpout[1000000];
  char message[1000000];

  while((c = getc(source)) > 0){
    printf("", c);
    tmpout[n++] = c;
  }
  tmpout[n] = '\0';

  fseek(source, 0L, SEEK_SET);

  sprintf(message, "%d\n", DEBUG_INSERTINPUTCODE);
  debug_send_message(message);

  sprintf(message, "%s\n", tmpout);
  debug_send_message(message);
}

void debug_clear(void)
{
  char message[10];
  sprintf(message, "%d\n", DEBUG_CLEAR);
  debug_send_message(message);
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


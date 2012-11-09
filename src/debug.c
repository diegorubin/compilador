#include "debug.h"

void debug_init(void)
{
  gbuffer = "/tmp/mypas";

  mkfifo(gbuffer, 0666);

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


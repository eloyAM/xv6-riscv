#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  struct pstat pi = {0};

	if (getpinfo(&pi) != 0) {
    printf("Error calling getpinfo\n");
  }
  
  for (int i = 0; i < NPROC; i++) {
    if(pi.inuse[i] != 0) {
      printf("PID:%d\t TICKETS:%d\t TICKS:%d\t INUSE:%d\n", pi.pid[i], pi.tickets[i], pi.ticks[i], pi.inuse[i]);
    }
  }

  exit(0);
}

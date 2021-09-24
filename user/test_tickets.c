#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int tickets, retval;
  
  tickets = 5;
  retval = settickets(tickets);
  printf("Setting %d tickets. Syscall return value: %d\n", tickets, retval);
  
  tickets = -1;
  retval = settickets(tickets);
  printf("Setting %d tickets. Syscall return value: %d\n", tickets, retval);
  
  tickets = 0;
  retval = settickets(tickets);
  printf("Setting %d tickets. Syscall return value: %d\n", tickets, retval);

  exit(0);
}

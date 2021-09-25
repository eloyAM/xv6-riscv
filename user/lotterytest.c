#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstat.h"

// Avoid "unused parameter" warning
#define UNUSED(x) (void)(x)

static void spin()
{
  volatile unsigned x = 0;
  volatile unsigned y = 0;
  volatile int pid = getpid();
  UNUSED(pid);

  while (x < 100000)
  {
    y = 0;
    while (y < (10000))
    {
      y++;
    }
    x++;
  }
}

static void printpinfo(int pid)
{
  struct pstat pi = {0};
  getpinfo(&pi);
  for (int i = 0; i < NPROC; i++)
  {
    if (pi.pid[i] == pid)
    {
      printf("Number of tickets that PID %d has: %d\n", pid, pi.tickets[i]);
      printf("Number of ticks that PID %d has: %d\n", pid, pi.ticks[i]);
      printf("Is the process with PID %d in use? (0 or 1): %d\n", pid, pi.inuse[i]);
    }
  }
}

int main(int argc, char *argv[])
{
  int pid1, pid2, pid3;
  int status;

  if ((pid1 = fork()) == 0)
  {
    int pp1 = getpid();
    printf("Process started with PID %d\n\n", pp1);
    settickets(10);
    spin();
    printpinfo(pp1);
    printf("Process with PID %d finished!\n\n", pp1);
    exit(0);
  }
  else if ((pid2 = fork()) == 0)
  {
    int pp2 = getpid();
    printf("Process started with PID %d\n\n", pp2);
    settickets(20);
    spin();
    printpinfo(pp2);
    printf("Process with PID %d finished!\n\n", pp2);
    exit(0);
  }
  else if ((pid3 = fork()) == 0)
  {
    int pp3 = getpid();
    printf("Process started with PID %d\n\n", pp3);
    settickets(30);
    spin();
    printpinfo(pp3);
    printf("Process with PID %d finished!\n\n", pp3);
    exit(0);
  }
  wait(&status);
  wait(&status);
  wait(&status);
  
  exit(0);
}

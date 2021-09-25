#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "pstat.h"

extern struct proc proc[NPROC];

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_settickets(void)
{
  int n;
  // Recuperar el argumento de la función y guardarlo en la variable
  if(argint(0, &n) < 0)
    return -1;
  // Comprobar que sea un número válido
  if (n < 1)
    return -2;
  // Cambiar el número de tickets
  myproc()->tickets = n;

  return 0;
}

uint64
sys_getpinfo(void)
{
  uint64 dstva; // User-space pointer to struct pstat
  if (argaddr(0, &dstva) < 0) // Retrieve the function argument
    return -1;

  struct pstat pi = {0};  // Local proc-info table

  struct proc *p; int i;  // Iterators
  for (p = proc, i = 0; p < &proc[NPROC]; p++, i++)
  {
    pi.inuse  [i] = (p->state == UNUSED)? 0 : 1;
    pi.pid    [i] = p->pid;
    pi.tickets[i] = p->tickets;
    pi.ticks  [i] = -69;                           // TODO: Add field ticks to proc
  }

  // Copy the struct into user space
  if(copyout(myproc()->pagetable, dstva, (char*)&pi, sizeof(struct pstat)) < 0)
    return -1;

 return 0;
}

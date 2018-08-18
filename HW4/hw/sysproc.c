#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
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

int
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

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
int
sys_dump(void)
{
  int pid;
  char *addr;
  char *buffer;
  int size;
  if(argint(0,&pid)<0)
	return -1;
  if(argptr(1,&addr,4)<0)
	return -1;
  if(argptr(2,&buffer,4)<0)
	return -1;
  if(argint(3,&size)<0)
	return -1;	
  return dump(pid,addr,buffer,size);			
}
extern struct proc * getptable_proc(void);
int sys_getprocinfo(void)
{
	int size;
	  char *buf;
	  char *s;
	  struct proc *p = '\0';

	  if (argint(0, &size) <0){
	    return -1;
	  }
	  if (argptr(1, &buf,size) <0){
	    return -1;
	  }

	  s = buf;
	  p = getptable_proc();
	  while(buf + size > s && p->state != UNUSED){
	      *(int *)s = p->state;
	      s+=4;
	      *(int *)s = p -> pid;
	      s+=4;
	      *(int *)s = p->parent->pid;
	      s+=4;
	      memmove(s,p->name,16);
	      s+=16;
	      p++;
	    }
	    return 0;
}
int sys_thread_create(void)
{
	int function;
	int arg;
	int stack;
	if(argint(0,&function)<0)
		return -1;
	if(argint(1,&arg)<0)
		return -1;
	if(argint(2,&stack)<0)
		return -1;
	return thread_create((void*)function,(void*)arg,(void*)stack);
}
int sys_thread_join(void)
{
	return thread_join();
}
int sys_thread_exit(void)
{
	thread_exit();
	return 0;
}
int sys_thread_cond_wait(void) {

	void *cv, *temp;

  if(argptr(0, (void *)&cv, sizeof(void *)) < 0)
    return -1;

  if(argptr(1, (void *)&temp, sizeof(void *)) < 0)
    return -1;

  lock_t *lock = (lock_t *)temp;

	thread_cond_wait(cv,lock);
	return 0;
}

int sys_thread_cond_signal(void) {

    void *cv;

    if(argptr(0, (void *)&cv, sizeof(void *)) < 0)
    return -1;

	thread_cond_signal(cv);
	return 0;
}
int
sys_sem_init(void)
{
  int sem;
  int value;

  if (argint(0, &sem) < 0)
    return -1;
  if (argint(1, &value) < 0)
    return -1;

  return sem_init(sem, value);
}

int
sys_sem_destroy(void)
{
  int sem;

  if (argint(0, &sem) < 0)
    return -1;

  return sem_destroy(sem);
}

int sys_sem_wait(void)
{
  int sem;
  int count;

  if (argint(0, &sem) < 0)
    return -1;
  if (argint(1, &count) < 0)
    return -1;

  return sem_wait(sem, count);
}

int sys_sem_signal(void)
{
  int sem;
  int count;

  if (argint(0, &sem) < 0)
    return -1;
  if (argint(1, &count) < 0)
    return -1;

  return sem_signal(sem, count);
}


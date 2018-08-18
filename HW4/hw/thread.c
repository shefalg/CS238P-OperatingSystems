#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"

struct balance {
    char name[32];
    int amount;
};

/*
Currently shown implementation of mutex lock. For checking implementation of spinlock, we can uncomment lines 70 and 75.
*/
//**************SPINLOCK***********************
struct thread_spinlock {
  uint locked;
}lock;
void thread_spin_init(struct thread_spinlock *lk)
{
	lk->locked=0;
}
void thread_spin_lock(struct thread_spinlock *lk)
{
	while(xchg(&lk->locked,1)!=0);
}
void thread_spin_unlock(struct thread_spinlock *lk)
{
	xchg(&lk->locked,0);
}
//************MUTEX*****************************
//struct thread_mutex {
//  uint locked;
//}m;
struct thread_mutex m;
void thread_mutex_init(struct thread_mutex *m)
{
	m->locked=0;
}
void thread_mutex_lock(struct thread_mutex *m)
{
	while(xchg(&m->locked,1)!=0)
		sleep(1);
}
void
thread_mutex_unlock(struct thread_mutex *m)
{
	xchg(&m->locked,0);
}

//**************FUNCTION************************
volatile int total_balance = 0;

volatile unsigned int delay (unsigned int d) {
   unsigned int i;
   for (i = 0; i < d; i++) {
       __asm volatile( "nop" ::: );
   }

   return i;
}

void do_work(void *arg){
    int i;
    int old;

    struct balance *b = (struct balance*) arg;
    printf(1, "Starting do_work: s:%s\n", b->name);

    for (i = 0; i < b->amount; i++) {
//         thread_spin_lock(&lock);
    		thread_mutex_lock(&m);
         old = total_balance;
         delay(100000);
         total_balance = old + 1;
//         thread_spin_unlock(&lock);
         thread_mutex_unlock(&m);
    }

    printf(1, "Done s:%s\n", b->name);

    thread_exit();
    return;
}

int main(int argc, char *argv[]) {

  struct balance b1 = {"b1", 3200};
  struct balance b2 = {"b2", 2800};

  void *s1, *s2;
  int t1, t2, r1, r2;
  thread_spin_init(&lock);
  thread_mutex_init(&m);	
  s1 = malloc(4096);
  s2 = malloc(4096);

  t1 = thread_create(do_work, (void*)&b1, s1);
  t2 = thread_create(do_work, (void*)&b2, s2);

  r1 = thread_join();
  r2 = thread_join();

  printf(1, "Threads finished: (%d):%d, (%d):%d, shared balance:%d\n",
      t1, r1, t2, r2, total_balance);

  exit();
}

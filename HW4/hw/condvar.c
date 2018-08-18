#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"

struct thread_mutex mutex;

void
thread_mutex_init(struct thread_mutex *m)
{

  m->locked = 0;

}
void thread_mutex_lock(struct thread_mutex *m)
{
   while(xchg(&m->locked, 1) != 0)
    sleep(1);
}

void
thread_mutex_unlock(struct thread_mutex *m)
{
  xchg(&m->locked, 0);
}

/*
 * * Reference: OSTEP: Three easy pieces book
 */
int MAX=5;
int buffer[5];
int fill_ptr=0;
int use_ptr=0;
int count=0;
void put(int value)
{

	buffer[fill_ptr]=value;
	fill_ptr=(fill_ptr+1)%MAX;
	count++;
}
int get()
{
	int tmp=buffer[use_ptr];
	use_ptr=(use_ptr+1)%MAX;
	count--;
	return tmp;
}
struct thread_cond empty,fill;

void thread_cond_init(struct thread_cond *cv)
{

  cv->lock = 0;

}
void producer(void *arg)
{
	int i;
	int loops=(int)arg;
	for(i=0;i<loops;i++)
	{
		thread_mutex_lock(&mutex);
		while(count == 5)
		{
		//	printf(1,"count %d",count);

			thread_cond_wait(&empty,&mutex);
		}
		put(i);
		printf(1,"Produced: %d \n ",i);
		thread_cond_signal(&empty);
		thread_mutex_unlock(&mutex);
	}
  thread_exit();
}
void consumer(void *arg)
{

	int i;
	int loops=(int)arg;
	for(i=0;i<loops;i++)
	{
		thread_mutex_lock(&mutex);
		while(count==0)
			thread_cond_wait(&fill,&mutex);
		int tmp=get();
		printf(1,"Consumed: %d \n",tmp);
		thread_cond_signal(&fill);
		thread_mutex_unlock(&mutex);

	  }
	thread_exit();
}
int main()
{
		printf(1,"Condition Variables Test Case.\nNumber of elements: 10 Buffer size: 5\n");
		void *s1, *s2;
		s1 = malloc(4096);
		s2 = malloc(4096);
		thread_mutex_init(&mutex);
		thread_cond_init(&empty);
		thread_cond_init(&fill);
		thread_create(producer, (void*)10,s1);
		thread_create(consumer, (void*)10,s2);
		thread_join();
		thread_join();
		printf(1,"Threads finished. \n");
		exit();

}

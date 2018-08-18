#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"
/*
 * * Reference: OSTEP: Three easy pieces book
 */
int MAX=5;
int buffer[5];
int fill=0;
int use=0;
#define mutex 0
#define empty 1
#define full 2
void put(int value)
{
	buffer[fill]=value;
	fill=(fill+1)%MAX;
}
int get()
{
	int tmp=buffer[use];
	use=(use+1)%MAX;
	return tmp;
}
void producer(void *arg)
{
	int i;
	int loops=(int)arg;
	for(i=0;i<loops;i++)
	{
		sem_wait(empty,1);
		sem_wait(mutex,1);
		printf(1,"Produced: %d\n",i);
		put(i);
		sem_signal(mutex,1);
		sem_signal(full,1);
	}
	thread_exit();
}
void consumer(void *arg)
{
	int i;
	int loops=(int)arg;
	for(i=0;i<loops;i++)
	{
		sem_wait(full,1);
		sem_wait(mutex,1);
		int tmp=get();
		printf(1,"Consumed : %d\n",tmp);
		sem_signal(mutex,1);
		sem_signal(empty,1);
	}
	thread_exit();
}
int main(int argc,char *argv[])
{
	printf(1,"Semaphores Test Case.\nNumber of elements: 10 Buffer size: 5\n");
	void *s1, *s2;
	s1 = malloc(4096);
	s2 = malloc(4096);
	sem_init(empty,MAX);
	sem_init(full,0);
	sem_init(mutex,1);
	thread_create(producer, (void*)10,s1);
	thread_create(consumer, (void*)10,s2);
	thread_join();
	thread_join();
	printf(1,"Threads finished. \n");
	sem_destroy(empty);
	sem_destroy(full);
	sem_destroy(mutex);
	exit();
}

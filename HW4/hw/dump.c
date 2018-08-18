#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"
#define PGSIZE 4096

void dump1()
{
int size;
int i=0;
int guardValueCheck=-1;
int stackStart=0;
int heapStart=0;
uint pid=fork();
int guardFlag=0;
//printf(1,"PID from fork: %d \n",pid);
if(pid==0)
{
while(1)
{
sleep(5);
}
}
else
{
//printf(1,"Child process PID inside parent: %d \n",pid);
size=(int)sbrk(0);
printf(1,"Process end address found using sbrk : %d \n",size);
void *addr=0;
void *buffer=malloc(size);
memset(buffer,0,size);
dump(pid,addr,buffer,size);
int* bufferIntPtr=(int*)buffer;
printf(1,"\n___________________________________TEXT SECTION______________________________________ \n\n");
while(i<size)
{
	if((*bufferIntPtr)==guardValueCheck)
	{	++guardFlag;
	if(guardFlag==1){
		stackStart=i+PGSIZE;
		heapStart=stackStart+PGSIZE;
//		printf(1,"Stack page and heap page are: %d   %d ",stackStart,heapStart);
		 printf(1,"\n__________________________________GUARD PAGE__________________________________\n\n");
	}
	}
	if(i!=0 && i==stackStart)
		printf(1,"\n__________________________________STACK PAGE__________________________________\n\n");
	if(i!=0 && i==heapStart)
		printf(1,"\n__________________________________HEAP SECTION__________________________________\n\n");
//	printf(1,"0x%x: ",bufferIntPtr);
	printf(1,"0x%x: ",i);
	printf(1,"0x%x\t",(*bufferIntPtr));
	bufferIntPtr+=1;
	printf(1,"0x%x\t",(*bufferIntPtr));
	bufferIntPtr+=1;
	printf(1,"0x%x\t",(*bufferIntPtr));
	bufferIntPtr+=1;
	printf(1,"0x%x\t",(*bufferIntPtr));
	bufferIntPtr+=1;
	printf(1,"\n");
	i+=16;	
}
}
}
int main(void )
{
//printf(1,"Inside dump.c \n");
dump1();
exit();
}
 

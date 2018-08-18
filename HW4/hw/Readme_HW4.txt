{\rtf1\ansi\ansicpg1252\cocoartf1504\cocoasubrtf830
{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\margl1440\margr1440\vieww10800\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

\f0\fs24 \cf0 1) Implemented Kernel threads (File: thread.c)\
\
	thread_create()\
	thread_join()\
	thread_exit()\
\
	as System calls.\
2) Implemented Spinlocks in user space (File: thread.c)\
\
    Implemented Mutexes in user space (File: thread.c)\
\
3) (Extra credit) Implemented Conditional variables (File: condvar.c)\
	Taken test case implementation of Producer Consumer queue example from OSTEP Book.\
	Created system calls for wait [thread_cond_wait()] and signal [thread_cond_signal()] functions.\
}
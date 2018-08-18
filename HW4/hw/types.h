typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;
typedef uint pte_t;
typedef struct thread_mutex {
  uint locked;
} lock_t;

typedef struct thread_cond{
   uint lock;
} cond_t;


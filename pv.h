#ifndef MY_LIB_PV
#define MY_LIB_PV
#include <linux/sem.h>
#include <sys/types.h>
#include <linux/ipc.h>
void P(int semid, int index);
void V(int semid, int index);
#endif


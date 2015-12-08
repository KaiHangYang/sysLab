#ifndef MY_COM
#define MY_COM
#include <stdio.h>
#define NUM 4
#define SEMKEY 199999
#define SHMKEYS 200000
#define SHMKEYT 200001
#define MAXLENGTH 4

union semun { int val; struct semid_ds *buf; unsigned short int *array; struct seminfo *__buf;}; 
typedef struct {
    char data[MAXLENGTH];
    int isEnd;
    size_t len;
} bufType;

#endif


#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <string.h>
#include "common.h"
#include "pv.h"
#include <assert.h>
int semid;
int shmids;

int main () {
    semid = semget((key_t)SEMKEY, NUM, IPC_CREAT);
    if (semid == -1) {
        return 0;
    }
    shmids = shmget((key_t)SHMKEYS, 0, IPC_CREAT);
    
    void * tmpS = shmat(shmids, 0, 0);
    bufType * S = (bufType *)tmpS;
    FILE * f = fopen("/home/fog/Dev/sysLab/from.dat", "rb");
    fseek(f, 0l, SEEK_END);
    size_t filesize = ftell(f);
    fseek(f, 0l, SEEK_SET);
    size_t count = 0;
    size_t sizecount = 0;
    char buf[MAXLENGTH];
    while ((count = fread(buf, 1, MAXLENGTH, f)) != 0) {
        P(semid, 0); // empty1
        memset(S->data, 0, MAXLENGTH);
        fprintf(stderr, "get %s\n", buf);
        memcpy(S->data, buf, count);
        S->len = count;
        sizecount += count;
        if (sizecount >= filesize) {
            S->isEnd = 1;
        }
        V(semid, 1); // full1
        if (sizecount >= filesize){
            break;
        } 
    }
    S->isEnd = 1;
    shmdt((void *)S);
    fclose(f);
    return 0;
}

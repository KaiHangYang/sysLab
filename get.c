#include <stdio.h>
#include <stdlib.h>
#include <linux/shm.h>
#include <string.h>
#include "common.h"
#include "pv.h"

int semid;
int shmids;

int main () {
    semid = semget(SEMKEY, NUM, IPC_CREAT);
    if (semid == -1) {
        printf("Error 1 !\n");
        return 0;
    }
    shmids = shmget(SHMKEYS, 0, IPC_CREAT);
    char * S = (char *)shmat(shmids, NULL, SHM_W);
    memset(S, 0, MEMSZ);
    FILE * f = fopen("./from.dat", "rb");
    char buf[MEMSZ];
    size_t count = 0;
    while ((count = fread(buf, MEMSZ, 1, f)) != 0) {
        P(semid, 0); // empty1
        memset(S, 0, MEMSZ);
        printf("%x\n", S);
        memcpy(S, buf, count);
        V(semid, 1); // full1
    }
    shmdt((void *)S);
    fclose(f);
    return 0;
}

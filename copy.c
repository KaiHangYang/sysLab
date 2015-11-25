#include <stdio.h>
#include <stdlib.h>
#include <linux/shm.h>
#include <string.h>
#include "common.h"
#include "pv.h"

int semid;
int shmids;
int shmidt;

int main() {
    semid = semget(SEMKEY, NUM, IPC_CREAT);

    if (semid == -1) {
        printf("Error 1 !\n");
        return 0;
    }
    shmids = shmget(SHMKEYS, 0, IPC_CREAT);
    shmidt = shmget(SHMKEYT, 0, IPC_CREAT);

    char * S = (char *)shmat(shmids, NULL, SHM_R);
    char * T = (char *)shmat(shmidt, NULL, SHM_W);
    struct shmid_ds state;
    while (1) {
        P(semid, 1); // full1
        P(semid, 2); // empty2
        memset(T, 0, MEMSZ);
        memcpy(T, S, MEMSZ);
        V(semid, 0); // empty1
        V(semid, 3); // full2
        
        shmctl(shmids, IPC_STAT, &state);
        if (state.shm_nattch <= 1) {
            // 表示复制已经结束
            break;
        }
    }
    shmdt((void *)S);
    shmdt((void *)T);
    return 0;
}

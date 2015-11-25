#include <stdio.h>
#include <stdlib.h>
#include <linux/shm.h>
#include <string.h>
#include "common.h"
#include "pv.h"

int semid;
int shmidt;

int main() {
    
    semid = semget(SEMKEY, NUM, IPC_CREAT);

    if (semid == -1) {
        printf("Error 1 !\n");
        return 0;
    }
    
    shmidt = shmget(SHMKEYT, 0, IPC_CREAT);
    char * T = (char *)shmat(shmidt, NULL, SHM_R);

    FILE *f = fopen("./to.dat", "wb");

    struct shmid_ds state;

    while (1) {
        P(semid, 3); // full2
        fwrite(T, MEMSZ, 1, f);
        V(semid, 2); // empty2
        
        shmctl(shmidt, IPC_STAT, &state);;
        if (state.shm_nattch <= 1) {
            // 复制结束
            break;
        }
    }
    shmdt((void *)T);
    fclose(f);
    return 0;
}

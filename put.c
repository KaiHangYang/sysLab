#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <string.h>
#include "common.h"
#include "pv.h"

int semid;
int shmidt;

int main() {
    semid = semget((key_t)SEMKEY, NUM, IPC_CREAT);

    if (semid == -1) {
        printf("Error 1 !\n");
        return 0;
    }
    
    shmidt = shmget((key_t)SHMKEYT, 0, IPC_CREAT);
    bufType * T = (bufType *)shmat(shmidt, NULL, SHM_R);

    FILE *f = fopen("./to.dat", "wb");
    while (1) {
        P(semid, 3); // full2
        fwrite(T->data, T->len, 1, f);
        V(semid, 2); // empty2
        fprintf(stderr, "put %d %d\n, ", T->isEnd, T->len);
        if (T->isEnd == 1) {
            break;
        }
    }
    shmdt((void *)T);
    fclose(f);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <sys/types.h>
#include "common.h"
#include "pv.h"
#include <assert.h>

int semid;
int shmids;
int shmidt;

int main() {
    semid = semget((key_t)SEMKEY, NUM, IPC_CREAT);
    if (semid == -1) {
        assert(semid != -1);
        printf("Error 1 !\n");
        return 0;
    }
    shmids = shmget((key_t)SHMKEYS, 0, IPC_CREAT);
    shmidt = shmget((key_t)SHMKEYT, 0, IPC_CREAT);
    
    assert(shmids != -1);
    assert(shmidt != -1);

    bufType * S = (bufType *)shmat(shmids, NULL, SHM_R);
    bufType * T = (bufType *)shmat(shmidt, NULL, SHM_W);

    while (1) {
        P(semid, 1); // full1
        P(semid, 2); // empty2
        memset(T->data, 0, MAXLENGTH);
        fprintf(stderr, "copy %s\n", S->data);
        memcpy(T->data, S->data, MAXLENGTH);
        T->len = S->len;
        if (S->isEnd == 1) {
            T->isEnd = 1;
        }
        V(semid, 0); // empty1
        V(semid, 3); // full2
        if (S->isEnd == 1) {
            break;
        }
    }
    shmdt((void *)S);
    shmdt((void *)T);
    return 0;
}

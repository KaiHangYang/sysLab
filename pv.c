#include "pv.h"
// P 操作
void P (int semid, int index) {
    struct sembuf sem;

    sem.sem_num = index;
    sem.sem_op = -1;
    sem.sem_flg = 0; // 操作标记： 0 或者IPC_NOWAIT
    semop(semid , &sem, 1); // 1表示执行命令的个数
    return;
}
// V操作
void V(int semid, int index) {
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = 1;
    sem.sem_flg = 0;

    semop(semid, &sem, 1);
    return;
}

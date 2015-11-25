#include <stdio.h>
#include <linux/sem.h>
#include <linux/ipc.h>
#include <linux/types.h>

#define NUM 1
#define KEY 1
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
int main() {
    
    //int result = semget(KEY, NUM, IPC_CREAT);
    //printf("%d\n", result);  
    return 0;
}

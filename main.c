#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <linux/sem.h>
#include <linux/shm.h>
#include <string.h>
#include "common.h"

int semid;
int shmids;
int shmidt;

int main () {
    int result;
    pid_t p1, p2, p3, p4;
    // 创建信号灯
    semid = semget(SEMKEY, NUM, IPC_CREAT);
    union semun arg;
    arg.val = 1; // 初始化信号量 empty1
    result = semctl(semid, 0, SETVAL, arg);
    arg.val = 0; // 初始化信号量 full1
    result = semctl(semid, 1, SETVAL, arg);
    arg.val = 1; // 初始化信号量 empty2
    result = semctl(semid, 2, SETVAL, arg);
    arg.val = 0; // 初始化信号量 full2
    result = semctl(semid, 3, SETVAL, arg);
    
    shmids = shmget(SHMKEYS, MEMSZ, IPC_CREAT);
    shmidt = shmget(SHMKEYT, MEMSZ, IPC_CREAT);

    if ((p1 = fork()) == 0) {
        // sub 进程
        printf("执行get\n");
        execv("./get", NULL);
    }
    else {
        // 主进程
        if ((p2 = fork()) == 0) {
            printf("执行copy\n");
            execv("./copy", NULL);
        }
        else {
            if ((p3 = fork()) == 0) {
                printf("执行put\n");
                execv("./put", NULL);
            }
        }
    }
    // 等待结束
    pid_t pid = wait(NULL); // 等待所有子进程结束
    semctl(semid, 0, IPC_RMID, NULL);
    // 删除共享内存
    shmctl(shmids, IPC_RMID, 0);
    shmctl(shmidt, IPC_RMID, 0);
    return 0;
}


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include "common.h"
#include <assert.h>
#include <errno.h>
#include "pv.h"

/*
#include <linux/sem.h>
#include <linux/ipc.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
*/

/*int P(int semid, int index);*/
/*void V(int semid, int index);*/

int main () {
    /*int result;*/
    /*semid = semget(SEMKEY, 1, IPC_CREAT);*/
    /*if (semid == -1) {*/
        /*printf("Error 1!\n");*/
        /*return 0;*/
    /*}*/
    /*union semun arg;*/
    /*arg.val = 0; // 设置信号量的值*/
    /*result = semctl(semid, 0, SETVAL, arg);*/

    /*P(semid, 0);*/
    int result;
    void * shms = NULL;
    void * shmt = NULL;
    int semid;
    int shmids;
    int shmidt;
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
    if (result == -1) {
        printf("Error 2!\n");
        if (errno == EACCES) {
            printf("1\n");
        }
        else if (errno == EFAULT){
            printf("2\n");
        }
        else if (errno == EINVAL) {
            printf("3\n");
        }
        return 0;
    }

    shmids = shmget((key_t)SHMKEYS, sizeof(bufType), IPC_CREAT|0666);
    shmidt = shmget((key_t)SHMKEYT, sizeof(bufType), IPC_CREAT|0666);
    
    if (shmids == -1 || shmidt == -1)  {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    shms = shmat(shmids, 0, 0);
    shmt = shmat(shmidt, 0, 0);
    
    if(shms == (void*)-1 || shmt == (void*)-1)  {  
        fprintf(stderr, "shmat failed\n");  
        exit(EXIT_FAILURE);  
    }
    bufType *tmpbuf = (bufType *)shms;
    tmpbuf->isEnd = 0;
    memset(tmpbuf->data, 0, MAXLENGTH);
    tmpbuf = (bufType *)shmt;
    tmpbuf->isEnd = 0;
    memset(tmpbuf->data, 0, MAXLENGTH);

    if(shmids == -1 || shmidt == -1)  {  
        fprintf(stderr, "shmget failed\n");  
        exit(EXIT_FAILURE);  
    }

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
    // 发现这里等待程序结束的wait函数有些问题 wait 只是等待一个进程结束就返回了
    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);
    waitpid(p3, NULL, 0);

    semctl(semid, 0, IPC_RMID, NULL);
    printf("结束了\n");
    // 删除共享内存
    shmctl(shmids, IPC_RMID, 0);
    shmctl(shmidt, IPC_RMID, 0);
    return 0;
}
// P 操作
/*int P (int semid, int index) {*/
    /*struct sembuf sem;*/

    /*sem.sem_num = index;*/
    /*sem.sem_op = -1;*/
    /*sem.sem_flg = 0; // 操作标记： 0 或者IPC_NOWAIT*/
     /*// 1表示执行命令的个数*/
    /*return semop(semid , &sem, 1);*/
/*}*/
/*// V操作*/
/*void V(int semid, int index) {*/
    /*struct sembuf sem;*/
    /*sem.sem_num = index;*/
    /*sem.sem_op = 1;*/
    /*sem.sem_flg = 0;*/

    /*semop(semid, &sem, 1);*/
    /*return;*/
/*}*/


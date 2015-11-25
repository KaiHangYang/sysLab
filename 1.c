#include <stdio.h>
#include <pthread.h>
#include <linux/sem.h>
#include <linux/ipc.h>
#include <sys/types.h>
#include <errno.h>

#define NUM 2
#define KEY 1
#define LOOPNUM 100

int semid;
pthread_t p1, p2;
int a = 0;

void P(int semid, int index);
void V(int semid, int index);

void *subp1();
void *subp2();
int main() {
    int result;
    semid = semget(KEY, NUM, IPC_CREAT);
    if (semid == -1) {
        printf("Error 1!\n");
        return 0;
    }
    union semun arg;
    arg.val = 1; // 设置信号量的值
    result = semctl(semid, 0, SETVAL, arg);
    arg.val = 0;
    result = semctl(semid, 1, SETVAL, arg);

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
    result = pthread_create(&p2, NULL, subp2, NULL);
    if (result != 0) {
        printf("Error 3!\n");
        return 0;
    }
    if (result != 0) {
        printf("Error 4!\n");
        return 0;
    }
    result = pthread_create(&p1, NULL, subp1, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    semctl(semid, 0, IPC_RMID, arg);
    printf("Finished!\n");
    return 0;
}
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

void *subp1() {
    int i;
    for (i = 0; i < LOOPNUM; ++i) {
        P(semid, 0);
        printf("The value of a is %d. -- p1\n", a);
        V(semid, 1);
    }

    return NULL;
}
void *subp2() {
    int i;
    for (i=0; i < LOOPNUM; ++i) {
        P(semid, 1);
        ++a;
        V(semid, 0);
    }
    return NULL;
}

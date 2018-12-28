#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
//#include <linux/sem.h>

int main()
{
    key_t key = ftok(".", 1);
    int shmId = shmget(key, 1024 * 8, 0666 | IPC_CREAT);
    if (shmId == -1) {
        printf("shmget fail, error: %d, %s\n", errno, strerror(errno));
        return 0;
    }
    char* segPtr = (char*)shmat(shmId, 0, 0);
    int semId = semget(key, 1, IPC_CREAT | 0666);
    if (semId == -1) {
        printf("semget fail, error: %d, %s\n", errno, strerror(errno));
        shmdt(segPtr);
        shmctl(shmId, IPC_RMID, NULL);
        return 0;
    }
    semctl(semId, 0, SETVAL, 0);
    struct sembuf p = {0, -1, SEM_UNDO};
    semop(semId, &p, 1);
    printf("segPtr: %s\n", segPtr);
    shmdt(segPtr);
    shmctl(shmId, IPC_RMID, NULL);
    semctl(semId, 1, IPC_RMID);
}

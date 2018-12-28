#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/sem.h>

int main()
{
    key_t key = ftok(".", 1);
    int shmId = shmget(key, 0, 0);
    if (shmId == -1) {
        printf("shmget fail, error: %d, %s\n", errno, strerror(errno));
        return 0;
    }
    char* segPtr = (char*)shmat(shmId, 0, 0);
    int semId = semget(key, 1, 0);
    if (semId == -1) {
        printf("semget fail, error: %d, %s\n", errno, strerror(errno));
        shmdt(segPtr);
        return 0;
    }
    struct sembuf v = {0, 1, SEM_UNDO};
    strncpy(segPtr, "fuck!\n", sizeof("fuck!\n"));
    semop(semId, &v, 1);
    shmdt(segPtr);
}

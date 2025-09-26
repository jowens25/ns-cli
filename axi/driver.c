// driver.c
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include "novusAgent.h"
#include "nsStrParser.h"

int main() // switch to main to use
{

    int serlen;

    char buf[MAX_BUFFER_SIZE];

    //		// Set up shared memory
    shm_id = shmget((key_t)SHARED_MEMORY_KEY, SHM_STORAGE_SIZE, 0666 | IPC_CREAT);
    if (shm_id == -1)
    {
        perror("radioTask shmget fail.");
        return 0;
    }

    void *shm_ptr;
    shm_ptr = shmat(shm_id, (void *)0, 0);
    if (shm_ptr == (void *)-1)
    {
        perror("Shared memory shmat() failed\n");
        return 0;
    }
    shm_data = (char *)shm_ptr;

    strParser(buf);

    shmdt(shm_data);
    shmctl(shm_id, IPC_RMID, 0);

    syslog(LOG_INFO, "Exiting radioTask\n");
    return 0;
}
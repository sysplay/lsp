#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

/* Obtain a binary semaphore id, allocating if necessary. */
int binary_semaphore_allocate(key_t key, int sem_flags)
{
	return semget(key, 1, IPC_CREAT | sem_flags);
}

/* Deallocate a binary semaphore. All users must have finished their use. Returns -1 on failure. */
int binary_semaphore_deallocate(int semid)
{
	return semctl(semid, 0 /* Ignored */, IPC_RMID);
}

/* Obtain a pre-existing binary semaphore id without allocating. */
int binary_semaphore_get(key_t key, int sem_flags)
{
	return semget(key, 1, sem_flags);
}

/* Initialize a binary semaphore with a value of 1. */
int binary_semaphore_set(int semid)
{
	return semctl(semid, 0, SETVAL, 1);
}

/* Wait on a binary semaphore. Block until the semaphore value is positive, then decrement it by 1. */
int binary_semaphore_wait(int semid)
{
	struct sembuf operations[1];

	/* Use the first (and only) semaphore. */
	operations[0].sem_num = 0;
	/* Decrement by 1. */
	operations[0].sem_op = -1;
	/* Permit undoing. */
	operations[0].sem_flg = 0;
	return semop(semid, operations, 1);
}

/* Post to a binary semaphore: Increment its value by 1. This returns immediately. */
int binary_semaphore_post(int semid)
{
	struct sembuf operations[1];

	/* Use the first (and only) semaphore. */
	operations[0].sem_num = 0;
	/* Increment by 1. */
	operations[0].sem_op = 1;
	/* Permit undoing. */
	operations[0].sem_flg = 0;
	return semop(semid, operations, 1); 
}

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "binary_sem.h"

#define SEGMENT_SIZE 0x400

int main()
{
	key_t key = 0x12345; // Using same for both shm & sem
	int segment_id;
	char *shared_memory;
	int sem_id;

	/* Allocate a shared memory segment */
	segment_id = shmget(key, SEGMENT_SIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	printf("1: Allocated a shared segment\n");

	/* Attach the shared memory segment */
	shared_memory = (char*)shmat(segment_id, NULL, 0);
	printf("1: Mapped shared memory at address %p\n", shared_memory);

	/* Allocating a binary semaphore */
	sem_id = binary_semaphore_allocate(key, S_IRUSR | S_IWUSR);
	printf("1: Semaphore created with id: %d\n", sem_id);

	/* Initializing the semaphore to 1 */
	binary_semaphore_set(sem_id);
	printf("1: Semaphore set to 1\n");

	/* Wait call */
	binary_semaphore_wait(sem_id);
	printf("1: Wait call returned\n");

	/* Write a string to the shared memory segment */
	sprintf(shared_memory, "Hi how r you?");
	printf("1: Message posted and timepass for half a minute. Now run ./sem_two\n");
	sleep(30);

	/* Post call */
	binary_semaphore_post(sem_id);
	printf("1: Post call returned\n");

	/* Wait call */
	binary_semaphore_wait(sem_id);
	printf("1: Wait call returned\n");	

	/* Print out the string from shared memory */
	printf("1: Shared memory has \"%s\"\n", shared_memory);

	/* Post Call */
	binary_semaphore_post(sem_id);
	printf("1: Post call returned\n");

	/* Detach the shared memory segment */
	shmdt(shared_memory);
	printf("1: Unmapped shared memory\n");

	return 0;
}

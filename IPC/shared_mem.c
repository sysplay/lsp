#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define KEY IPC_PRIVATE // 0x12345 // Would retrieve the same if not detached

int main()
{
	int segment_id;
	char *shared_memory;
	struct shmid_ds shmbuffer;
	int segment_size;
	const int shared_segment_size = 0x6400;

	/* Allocate a shared memory segment */
	//segment_id = shmget(KEY, shared_segment_size, IPC_CREAT | S_IRUSR | S_IWUSR);
	segment_id = shmget(KEY, shared_segment_size, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	printf("The segment id: %d (0x%X)\n", segment_id, segment_id);

	/*Attach the shared memory segment.*/
	shared_memory = (char*)shmat(segment_id, NULL, 0);
	printf("Shared memory attached at address %p\n", shared_memory);

	/* Determine the segment ’s size */
	shmctl(segment_id, IPC_STAT, &shmbuffer);
	segment_size = shmbuffer.shm_segsz;
	printf("Segment size: %d (0x%X) bytes\n", segment_size, segment_size);

	/* Write a string to the shared memory segment */
	sprintf(shared_memory, "Hello World");
	printf("Wrote \"Hello World\" into it\n");
	/* Detach the shared memory segment */
	shmdt(shared_memory);
	printf("Shared memory detached\n");

	/* Reattach the shared memory segment, at a different address */
	shared_memory = (char*)shmat(segment_id, (void*)0x5000000, 0);
	printf("Shared memory reattached at address %p\n", shared_memory);
	/* Print out the string from shared memory */
	printf("It contains: %s\n", shared_memory);
	/* Detach the shared memory segment */
	shmdt(shared_memory);
	printf("Shared memory detached\n");
	/* Deallocate the shared memory segment */
	shmctl(segment_id, IPC_RMID, 0); // TRY: Comment this to see the listing using ipcs -m

	return 0;
}

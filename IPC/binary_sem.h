#ifndef BINARY_SEM_H

#define BINARY_SEM_H

/* Obtain a binary semaphore id, allocating if necessary. */
int binary_semaphore_allocate(key_t key, int sem_flags);
/* Deallocate a binary semaphore. All users must have finished their use. Returns -1 on failure. */
int binary_semaphore_deallocate(int semid);
/* Obtain a pre-existing binary semaphore id without allocating. */
int binary_semaphore_get(key_t key, int sem_flags);
/* Initialize a binary semaphore with a value of 1. */
int binary_semaphore_set(int semid);
/* Wait on a binary semaphore. Block until the semaphore value is positive, then decrement it by 1. */
int binary_semaphore_wait(int semid);
/* Post to a binary semaphore: Increment its value by 1. This returns immediately. */
int binary_semaphore_post(int semid);

#endif

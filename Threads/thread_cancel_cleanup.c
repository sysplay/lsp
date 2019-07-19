#include <malloc.h>
#include <pthread.h>
#include <stdio.h>

/* Allocate a temporary buffer */
void *allocate_buffer(size_t size)
{
	printf("Resource Allocated\n");
	return malloc(size);
}
/* Deallocate a temporary buffer.  */
void deallocate_buffer(void *buffer)
{
	free(buffer);
	printf("\nResource Freed\n");
}
void *do_some_work(void *p)
{
	/* Allocate a temporary buffer.  */
	void *temp_buffer = allocate_buffer(1024); // Is it the right place???
	int i;

	/*
	 * Register a cleanup handler for this buffer, to deallocate it in case
	 * the thread exits or is cancelled.
	 */
	// pthread_exit(NULL); // TRY: Uncomment to see no handler invocation
	pthread_cleanup_push(deallocate_buffer, temp_buffer);
	// pthread_exit(NULL); // TRY: Uncomment to see the handler invocation
	/* Do some work here that might call pthread_exit or might be cancelled */
	for (i = 0; i < 10; i++)
	{
		fputc('c', stderr);	
		fflush(stderr);
		sleep(1);
	}
	/*
	 * Unregister the cleanup handler. Because we pass a nonzero value,
	 * this actually performs the cleanup by calling deallocate_buffer.
	 */
	pthread_cleanup_pop(1); // TRY: Pass 0 with no cancel to see no handler invocation
	return NULL;
}

int main()
{
	pthread_t thread_id;
	void *status;
	
	pthread_create(&thread_id, NULL, &do_some_work, NULL);
	sleep(3);
	pthread_cancel(thread_id);
	pthread_join(thread_id, &status);
	if (status == PTHREAD_CANCELED)
	{
		printf("Thread was cancelled\n");
	}
	else
	{
		printf("Thread terminated normally\n");
	}
	
	return 0;
}

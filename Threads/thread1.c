#include <pthread.h>
#include <stdio.h>

/* Prints x’s to stderr. The parameter is unused. Does not return. */
void *print_xs(void* unused)
{
	while (1)
		fputc('x', stderr);
	return NULL;
}

/* The main program.  */
int main()
{
	pthread_t thread_id;

	/* Create a new thread. The new thread will run the print_xs function. */
	if (pthread_create(&thread_id, NULL, &print_xs, NULL))
	{
		printf("Thread creation failed\n");
		return 1;
	}
	while (1)
		fputc('o', stderr);
	return 0;
}

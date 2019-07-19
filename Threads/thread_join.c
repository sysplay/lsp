#include <pthread.h>
#include <stdio.h>

struct char_print_parms /* Parameters to print_function. */
{
	char character; /* The character to print. */
	int count; /* The number of times to print it. */
};

void *char_print(void* parameters)
{
	struct char_print_parms *p = (struct char_print_parms *)parameters;
	int i;

	for (i = 0; i < p->count; ++i)
	{
		fputc(p->character, stderr);
		fflush(stderr);
		sleep(10 / p->count);
	}

	return NULL;
}

int main()
{
	pthread_t thread1_id;
	pthread_t thread2_id;
	struct char_print_parms thread1_args;
	struct char_print_parms thread2_args;

	printf("Printing ... ");
	fflush(stdout); // TRY: Comment it out & see the fun

	/* Create a new thread to print 3 ’x’s. */
	thread1_args.character = 'x';
	thread1_args.count = 3;
	if (pthread_create(&thread1_id, NULL, &char_print, &thread1_args))
	{
		printf("Thread 1 creation failed\n");
		return 1;
	}
	/* Create a new thread to print 10 o’s. */
	thread2_args.character = 'o';
	thread2_args.count = 10;
	if (pthread_create(&thread2_id, NULL, &char_print, &thread2_args))
	{
		printf("Thread 2 creation failed\n");
		return 1;
	}

	pthread_join(thread1_id, NULL);
	pthread_join(thread2_id, NULL);

	printf(" done\n");

	return 0;
}

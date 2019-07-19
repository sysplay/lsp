#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>

/* The key used to associate a log file pointer with each thread.  */
static pthread_key_t thread_log_key;

/* Write message to the log file for the current thread.  */
void write_to_thread_log(const char *message, ...)
{
	FILE *thread_log = (FILE *)pthread_getspecific(thread_log_key);
	va_list ap;

	va_start(ap, message);
	vfprintf(thread_log, message, ap);
	va_end(ap);
}
/* Close the log file pointer THREAD_LOG.  */
void close_thread_log(void *thread_log)
{
	FILE *fp = (FILE *)thread_log;

	fclose(fp);
}
void *thread_function(void *arg)
{
	int thread_num = (int)(long)(arg);
	char thread_log_filename[22];
	FILE *thread_log;
	int i;

	/* Generate the filename for this thread’s log file. */
	sprintf(thread_log_filename, "thread%d.log", thread_num);
	/* Open the log file */
	thread_log = fopen(thread_log_filename, "a");
	/* Store the file pointer in thread-specific data under thread_log_key */
	pthread_setspecific(thread_log_key, thread_log);
	printf("%d: Working (%s) ...\n", thread_num, thread_log_filename);
	write_to_thread_log("Thread with id 0x%x started\n", (int)pthread_self());
	/* Do work here ... */
	for (i = 0; i < 2 * thread_num; i++)
	{
		write_to_thread_log("%*d", thread_num + 1, i + 1);
		sleep(5 / thread_num);
	}
	write_to_thread_log("\n");
	write_to_thread_log("Thread %d done\n", thread_num);
	printf("%d: done\n", thread_num);
	return NULL;
}

int main()
{
	int i;
	pthread_t threads[5];

	/*
	 * Create a key to associate thread log file pointers in thread-specific
	 * data. Use close_thread_log to clean up the file pointers
	 */
	pthread_key_create(&thread_log_key, close_thread_log);

	/* Create threads to do the work */
	for (i = 0; i < 5; ++i)
		pthread_create(&(threads[i]), NULL, &thread_function, (void *)(long)(i + 1));
	/* Wait for all threads to finish. */
	for (i = 0; i < 5; ++i)
		pthread_join(threads[i], NULL);

	/* Delete the key */
	pthread_key_delete(thread_log_key);

	return 0;
}

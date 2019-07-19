#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <signal.h>

#define CREATE_THREAD

#define STACK_SIZE (8 * 1024)

static void *stack_ptr;
static global_var = 10;

int child_process(void *arg)
{
	int i;

	printf("C: I am in the cloned process\n");
	for(i = 0; i < 10; i++)
	{
		printf(" :) ");
		fflush(stdout);
		sleep(1);
	}
	printf("\n");
	printf("C: Global value is %d\n", global_var);
	printf("C: Exiting the clone\n");
	return 6;
}

int main()
{
	void *ptr = NULL;
	pid_t pid;
	int clone_status;

	if ((stack_ptr = malloc(STACK_SIZE)) == NULL)
	{
		printf("Stack allocation failed\n");
		exit(1);
	}
	printf("P: Trying to clone\n");
#ifdef CREATE_THREAD
	pid = clone(child_process, (stack_ptr + STACK_SIZE), CLONE_VM | CLONE_SIGHAND | CLONE_THREAD, ptr);
#else
	pid = clone(child_process, (stack_ptr + STACK_SIZE), SIGCHLD, ptr);
#endif
	printf("P: pid of the cloned process is %d\n", (int)pid);
	printf("P: Global value is %d\n", global_var);
	global_var = 5;
	printf("P: Global value changed to %d\n", global_var);
	printf("P: Waiting for clone to exit\n");
	wait(&clone_status);
	printf("P: Clone exited with status: %d (0x%X)\n", WEXITSTATUS(clone_status), clone_status);
#ifdef CREATE_THREAD
	sleep(11);
#endif
	printf("P: Exiting the parent\n");
}

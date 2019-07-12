#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

sig_atomic_t child_done = 0;
sig_atomic_t child_exit_status = -26;

void clean_up_child_process(int signal_number)
{
	int child_pid = 0, child_status = 1;

	/* Clean up the child process */
	printf("Cleaning up the child ... ");
	child_pid = wait(&child_status); // Comment this to see the zombie
	printf("done\n");
	child_done = 1;
	/* Store its status */
	if (child_pid)
	{
		if (WIFEXITED(child_status))
		{
			printf("Child %d terminated normally\n", child_pid);
			child_exit_status = WEXITSTATUS(child_status);
		}
		else if (WIFSIGNALED(child_status))
		{
			printf("Child %d terminated by signal\n", child_pid);
			child_exit_status = WTERMSIG(child_status);
		}
		else
		{
			child_exit_status = -5;
		}
		printf("Child's exit status: %d\n", child_exit_status);
	}
}

int main()
{
	struct sigaction sigchld_action;
	pid_t child_pid;

	/* Handle SIGCHLD */
	memset(&sigchld_action, 0, sizeof(sigchld_action));
	sigchld_action.sa_handler = &clean_up_child_process;
	sigaction(SIGCHLD, &sigchld_action, NULL);

	/* Fork the child process */
	child_pid = fork();
	if(child_pid != 0) // Parent
	{
		while (1)
		{
			/* Do some work */
			sleep(1);
			if (child_done)
			{
				break;
			}
		}
		/* Some more work */
		sleep(5);
		printf("\nParent process handled SIGCHLD signal (Child status: %d).\n", child_exit_status);
		return 0;
	}
	else
	{	
		printf("\nChild exiting...\n");
		abort();
	}

	return 0;
}

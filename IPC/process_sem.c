#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main()
{
	int semid, pid;
	struct sembuf sop;

	semid = semget(0x20, 1, IPC_CREAT | S_IRUSR | S_IWUSR);
	semctl(semid, 0, SETVAL, 1);

	pid = fork();
	if (pid == 0)
	{
		printf("Child before semop\n");
		sop.sem_num = 0; 
		sop.sem_op = -1;
		sop.sem_flg = 0;
		semop(semid, &sop, 1);
		printf("Child in critical section for 10 seconds\n");
		printf("Parent may have to wait\n");
		sleep(10);
		printf("Child coming out of critical section\n");
		sop.sem_num = 0; 
		sop.sem_op = 1; 
		sop.sem_flg = 0;
		semop(semid, &sop, 1);
	}
	else
	{
		int child_status;

		printf("Parent before semop\n");
		sop.sem_num = 0; 
		sop.sem_op = -1;
		sop.sem_flg = 0;
		semop(semid, &sop, 1);
		printf("Parent in critical section\n");
		printf("Child may have to wait\n");
		sleep(10);
		printf("Parent coming out of critical section\n");
		sop.sem_num = 0; 
		sop.sem_op = 1; 
		sop.sem_flg = 0;
		semop(semid, &sop, 1);

		//wait(&child_status);
		//semctl(semid, 0, IPC_RMID);
	}

	return 0;
}

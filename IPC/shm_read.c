#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define SIZE 1

static char *detect_key;
static int shmid;

int shared_init()
{
	if ((shmid = shmget(9999, SIZE, S_IRUSR)) < 0)
	{
		perror("shmget");
		return -1;
	}
	if ((detect_key = shmat(shmid, NULL, SHM_RDONLY)) < 0)
	{
		perror("shmat");
		return -1;
	}
	//	detect_key = NULL;
	return 0;
}

void detect_hit()
{
	char c;

	c = *detect_key;
	printf("detect_key is %s\n", detect_key);
	while (1)
	{
		if (c != *detect_key)
		{
			printf("detect_key is %s\n", detect_key);
			if (detect_key[0] == '.')
			{
				shmdt(detect_key);
				shmctl(shmid, IPC_RMID, NULL);
				exit(1);
			}
			c = *detect_key;
		}
	}
}

int main()
{
	if (shared_init() < 0)
	{
		exit(1);
	}
	detect_hit();
	return 0;
}

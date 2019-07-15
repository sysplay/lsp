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

static char *read_key;
static int shmid;

int shared_init()
{
	if ((shmid = shmget(9999, SIZE, IPC_CREAT | S_IRUSR | S_IWUSR)) < 0)
	{
		perror("shmget");
		return -1;
	}
	if ((read_key = shmat(shmid, NULL, 0)) < 0)
	{
		perror("shmat");
		return -1;
	}
	return 0;
}

void read_char()
{
	char c;

	printf("Enter character by character. '.' to terminate\n");
	while (1)
	{
		c = getchar();
		if(c == '\n')
		{
			c = getchar();
		}
		strncpy(read_key, &c, SIZE);
		printf("read_key now is %s\n", read_key);
		if (*read_key == '.')
		{
			shmdt(read_key);
			shmctl(shmid, IPC_RMID, NULL);
			break;
		}
	}
}

int main()
{
	if (shared_init() < 0)
	{
		exit(1);
	}
	read_char();
	return 0;
}

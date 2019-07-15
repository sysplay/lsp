#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


int main(int argc, char *argv[])
{
	int msgq_id, len, i;
	key_t key;

	struct
	{
		long mtype;
		char mtext[15];
	} message;

	key = ftok(argv[0], 'a');
	printf("Generated key: %08lX\n", key);
	//msgq_id = msgget((key_t)10, IPC_CREAT | S_IRUSR | S_IWUSR);
	msgq_id = msgget(key, IPC_CREAT | S_IRUSR | S_IWUSR);
	if(msgq_id == -1)
	{
		perror("msgget");
		exit(1);
	}
	
	strncpy(message.mtext, "Hello World\n", 15);
	message.mtype = 1;
	len = strlen(message.mtext);

	for(i = 0; i < 10000; i++)
	{
		printf("Sending msg #%d ... ", i + 1);
		fflush(stdout);
		if (msgsnd(msgq_id, &message, len, 0) == -1)
		{
			perror("msgsnd");
			exit(1);
		}
		printf("done\n", i);
	}

	return 0;
}

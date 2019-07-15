#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf
{
	long mtype;
	char mtext[200];
};

int main(void)
{
	struct my_msgbuf buf;
	int msqid;
	key_t key;
	int read_cnt;

	if ((key = ftok("./msg_send", 'B')) == -1)
	{
		perror("ftok");
		exit(1);
	}

	if ((msqid = msgget(key, IPC_CREAT | S_IRUSR | S_IWUSR)) == -1)
	{
		perror("msgget");
		exit(1);
	}

	printf("Enter lines of text, ^D (on a separate line) to quit:\n");

	buf.mtype = 1; /* we don't really care in this case */
	while (1)
	{
		read_cnt = scanf("%[^\n]", buf.mtext);
		if (read_cnt == EOF)
		{
			break;
		}
		if (read_cnt == 0) // Null string
		{
			buf.mtext[0] = '\0';
		}
		scanf("%*c"); // Read the '\n'

		if (msgsnd(msqid, (struct msgbuf *)&buf, sizeof(buf.mtext), 0) == -1)
			perror("msgsnd");
	}

	if (msgctl(msqid, IPC_RMID, NULL) == -1)
	{
		perror("msgctl");
		exit(1);
	}

	return 0;
}

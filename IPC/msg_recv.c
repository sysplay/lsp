#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf {
	long mtype;
	char mtext[200];
};

int main(void)
{
	struct my_msgbuf buf;
	int msqid;
	key_t key;

	if ((key = ftok("./msg_send", 'B')) == -1)  /* same key as in msg_send.c */
	{
		perror("ftok");
		exit(1);
	}

	if ((msqid = msgget(key, S_IRUSR | S_IWUSR)) == -1) /* connect to the queue */
	{
		perror("msgget");
		exit(1);
	}

	printf("Ready to receive messages\n");

	for (;;) /* it  never quits! */
	{
		if (msgrcv(msqid, (struct msgbuf *)&buf, sizeof(buf.mtext), 0, 0) == -1)
		{
			perror("msgrcv");
			exit(1);
		}
		printf("spock: \"%s\"\n", buf.mtext);
	}

	return 0;
}

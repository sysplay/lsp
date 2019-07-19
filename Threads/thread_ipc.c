#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *transmitter(void *a)
{
	FILE *stream = (FILE *)a;
	char message[80];
	int count;

	while (1)
	{
		printf("w: Enter transmission cnt: ");
		scanf("%d%*c", &count);
		if (count <= 0)
		{
			break;
		}
		printf("w: Enter transmission msg: ");
		if (scanf("%[^\n]", message) <= 0)
		{
			message[0] = 0;
			getchar();
		}
		printf("w: Sending %d messages ...\n", count);
		for ( ; count > 0; --count)
		{
			fprintf(stream, "%s\n", message);
			fflush(stream);
			sleep(1);
		}
	}
	fclose(stream);
	printf("w: Done\n");
	return NULL;
}

void *receiver(void *a)
{
	FILE *stream = (FILE *)a;
	char buffer[1024];
	char *rc;

	printf("r: Waiting for message ...\n");
	while (!feof(stream) && !ferror(stream) && (fgets(buffer, sizeof(buffer), stream) != NULL))
	{
		printf("r: %s", buffer);
	}
	fclose(stream);
	printf("r: Done\n");
	return NULL;
}

int main()
{
	pthread_t tid1;
	pthread_t tid2;
	int fds[2];
	FILE *stream;

	pipe(fds);
	stream = fdopen(fds[0], "r");
	pthread_create(&tid1, NULL, &receiver, stream);
	stream = fdopen(fds[1], "w");
	pthread_create(&tid2, NULL, &transmitter, stream);

	pthread_join(tid2, NULL);
	pthread_join(tid1, NULL);

	return 0;
}

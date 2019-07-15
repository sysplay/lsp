#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void writer(const char *message, int count, FILE *stream)
{
	for(; count > 0; --count)
	{
		fprintf(stream, "%s\n", message);
		fflush(stream);
		sleep(2);
	}
}

void reader(FILE *stream)
{
	char buffer[1024];

	while (!feof(stream) && !ferror(stream) && fgets(buffer, sizeof(buffer), stream) != NULL)
	{
		printf("In child: ");
		fputs(buffer, stdout);
	}
}
int main()
{
	int fds[2];
	pid_t pid;
	FILE *stream;

	pipe(fds);
	pid = fork();

	if (pid == (pid_t)0)
	{
		close(fds[1]);
		printf("I am in child\n");
		stream = fdopen(fds[0], "r");
		reader(stream);
		//sleep(5);
		close(fds[0]);
	}
	else
	{
		close(fds[0]);
		printf("I am in parent\n");
		stream = fdopen(fds[1], "w");
		sleep(5);
		writer("Hello, World", 5, stream);
		close(fds[1]);
	}

	return 0;
}

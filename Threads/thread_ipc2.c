/* Keyboard hit program */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>

static pthread_t tid_read, tid_obtain;
static pthread_mutex_t mutex_mmap = PTHREAD_MUTEX_INITIALIZER;
static sig_atomic_t char_available = 0;

void *read_char(void *map)
{
	char *cp = (char *)map;
	char c;

	printf("r: Reading characters ('.' to exit) ...%p\n", cp);
	while (1)
	{
		/* Get a character in 'c' except '\n'. */
		c = getchar();
		if (c == '\n')
			c = getchar();
		pthread_mutex_lock(&mutex_mmap);
		*cp = c;
		pthread_mutex_unlock(&mutex_mmap);
		pthread_kill(tid_obtain, SIGUSR1);
		if (c == '.')
		{
			pthread_exit(NULL);
		}
	}
	return NULL;
}

void char_handler(int sno)
{
	char_available = 1;
}
void *obtain_char(void *map)
{
	char *cp = (char *)map;
	struct sigaction action;
	char c;

	printf("o: Setting up the signal handler for the thread\n");
	memset(&action, 0, sizeof(action));
	sigemptyset(&action.sa_mask);
	action.sa_handler = &char_handler;

	if (sigaction(SIGUSR1, &action, NULL))
	{
		printf("o: Registering signal handler failed\n");
		return (void *)(-1);
	}

	printf("o: Obtain character ...%p\n", cp);
	while (1)
	{
		if (char_available)
		{
			char_available = 0;
			pthread_mutex_lock(&mutex_mmap);
			c = *cp;
			pthread_mutex_unlock(&mutex_mmap);
			printf("o: Key obtained: %c\n", c);
			if (c == '.')
			{
				break;
			}
		}
	}
	signal(SIGUSR1, SIG_IGN);
	return NULL;
}

void *map_char()
{
	int fd;
	void *map;

	if ((fd = open("/tmp/char", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
	{
		perror("open");
		return NULL;
	}

	write(fd, "D", 1);

	if ((map = mmap(0, sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		perror("mmap");
		close(fd);
		return NULL;
	}

	close(fd);
	return map;
}
void unmap_char(void *map)
{
	munmap(map, sizeof(char));
}

int main()
{
	void *map;

	if ((map = map_char()) == NULL)
	{
		printf("Mapping character failed\n");
		return 1;
	}
	/* Create thread for reading characters. */
	if (pthread_create(&tid_read, NULL, &read_char, map))
	{
		printf("Reading thread creation failed\n");
		unmap_char(map);
		return 2;
	}
	/* Create thread for writing characters. */
	if (pthread_create(&tid_obtain, NULL, &obtain_char, map))
	{
		printf("Writing thread creation failed\n");
		pthread_cancel(tid_read);
		unmap_char(map);
		return 3;
	}
	pthread_join(tid_obtain, NULL);
	pthread_join(tid_read, NULL);
	unmap_char(map);
	pthread_mutex_destroy(&mutex_mmap);

	return 0;
}

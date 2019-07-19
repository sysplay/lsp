#include <pthread.h>
#include <stdio.h>

struct fact_params
{
	int count;
};

void* fact(void* parameters)
{
	struct fact_params *p = (struct fact_params *)parameters;
	int i, factorial = 1;
	void *pp;

	if(p->count != 0)
	{
		for(i = 1; i <= p->count; i++)
			factorial = factorial * i;
	}
	pp = (void *)(long)factorial;
	return pp;
}

int main()
{
	int n;
	void *p;
	pthread_t thread_id;
	struct fact_params thread_args;
	
	printf("Enter a number: ");
	scanf("%d", &n);
	thread_args.count = n;

	pthread_create(&thread_id, NULL, &fact, &thread_args);
	
	pthread_join(thread_id, &p);
	printf("Factorial = %d\n", p);

	return 0;
}

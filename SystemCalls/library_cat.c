#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	FILE *fp;
	char buf[80];
	int cnt;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		return 1;
	}
	if (!(fp = fopen(argv[1], "r")))
	{
		perror("fopen (actually open)");
		return 2;
	}
	while ((cnt = fread(buf, sizeof(char), 80, fp)) > 0)
	{
		fwrite(buf, sizeof(char), cnt, stdout);
	}
	fclose(fp);

	return 0;
}

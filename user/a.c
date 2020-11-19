#include "../kernel/types.h"
#include "user.h"

int main()
{
	int fd[2];
	int i,j,len;

	pipe(fd);

	if(fork() == 0)
	{
		close(fd[1]); // if forget this the read would block forever

		printf("wait read\n");
		while((len = read(fd[0],&j,sizeof(int))) > 0)
			printf("a %d\n",len);
		printf("len %d\n",len);
	}
	else
	{
		close(fd[0]);
		sleep(5);

		write(fd[1],&i,sizeof(int));
		
		sleep(5);
		close(fd[1]);
	}

	wait(0);
	exit(0);
}

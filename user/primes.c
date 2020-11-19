#include "kernel/types.h"
#include "user.h"

void close_pipe(int *fd);

void prime();

int main()
{
	int fd[2];

	pipe(fd);

	if(fork() == 0)
	{
		close(0);
		dup(fd[0]);
		close_pipe(fd);
		/*
		 * in fact, after dup a new fd point to the same open file table
		 * with fd fd[0], the entry of open file table won't delete.
		 */
		prime();
	}
	else
	{
		close(1);
		dup(fd[1]);
		close_pipe(fd);

		for(int i = 2;i <= 35;++i)
			write(1,&i,sizeof(int));
	
		close(1);
		wait(0);
	}
	exit(0);
}

void prime(){
	int n,p,len,fd[2];
	len = read(0,&n,sizeof(int));

	if(len <= 0)
		exit(0);

	/* this is the first read(the non-primes has been droped) */
	printf("prime %d\n",n);

	pipe(fd);

	if(fork() == 0)
	{
		close(0); /* the thing whic pointed by old_fd[0] get rid */
		dup(fd[0]);
		close_pipe(fd); /* close child pipe */
		prime();
	}
	else
	{
		close(1);
		dup(fd[1]);
		close_pipe(fd);

		while((len = read(0,&p,sizeof(int))) > 0)
		{
			if(p % n != 0)
				write(1,&p,sizeof(int));
		}

		if(len <= 0){
			close(1);
			wait(0);
			exit(0); /* exit will auto close all, and close 1 will tell the reader there's no data and get EOF */		
		}
	}
}

void close_pipe(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}


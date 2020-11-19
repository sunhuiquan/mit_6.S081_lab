#include "kernel/types.h"
#include "user.h"

int main()
{
	int parent_fd[2],child_fd[2];
	char msg = 'a',buf;

	/* pipe is unidirectional way to conmuncate with two processors */
	pipe(parent_fd);
	pipe(child_fd);

	int pid = fork();
	if(pid == 0)
	{
		read(parent_fd[0],&buf,1);
		printf("%d: received ping\n",getpid());
		write(child_fd[1],&msg,1);
	}
	else
	{
		write(parent_fd[1],&msg,1);
		read(child_fd[0],&buf,1);
		printf("%d: received pong\n",getpid());
	}

	exit(0);
}

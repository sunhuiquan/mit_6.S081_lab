#include "kernel/types.h"
#include "user.h"
#include "kernel/param.h"

#define MAXSIZE 32 

int main(int argc,char *argv[])
{	
	int i = 0;
	char command[MAXSIZE],buf[MAXSIZE],*p,*q;
	char *argu[MAXSIZE];

	if(argc < 2)
	{
		printf("usage: %s <command> <may have args...>)",argv[0]);
		exit(0);
	}
	
	read(0,buf,sizeof(buf));
	strcpy(command,argv[1]);
	/*
	 * The '\0' is a char which ascii is 0 but it's still a char,
     * and NULL macro is 0 but it's a pointer,
	 * therefore the though NULL macro and '\0' ascii can all be 0,
	 * they can't convert each other, note that point!
	 *
	 * For example, the p is a pointer to char, so *p is a char you
	 * can *p = '\0' or *p = 0(in this way do implicit convert and 
	 * not a pointer NULL here), but you can't p = '\0', it's totally
	 * two different things!
	 */
	for(i = 1;argv[i] != 0;i++)
		argu[i - 1] = argv[i];
	argu[i - 1] = 0; /* tell where is no more arugments for excute */

	q = buf;
	while((p = strchr(q,'\n')) != 0) /* the lab dosen't have NULL macro */
	{
		*p++ = '\0';
		argu[i - 1] = q;
		argu[i] = 0;
		q = p;
	
		/* don't omit { } to make clear and not make stupid faults */
		if(fork() == 0)
		{
			if(exec(command,argu) < 0)
			{
				fprintf(2,"can't excute %s command\n",argv[1]);
				exit(1);
			}
		}
		else
		{
			wait(0); /* ensure it's in a right order */
		}
	}

	exit(0);
}


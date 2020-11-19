#include "kernel/types.h" /* the user.h will use this too */
#include "user.h"

int main(int argc,char *argv[])
{
	if(argc < 2)
	{
		fprintf(2,"usage: %s <secondes>",argv[0]);
		exit(1);
	}

	sleep(atoi(argv[1])); /* sleep for n ticks(100ms) not seconds */

	exit(0);
}

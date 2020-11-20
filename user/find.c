#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"

/*
 * Note that in xv6, we use xv6's everything, including the xv6's system calls
 * and file system, and we mkdir something use linux not xv6, it won't use xv6's
 * system call, and then only change the linux not xv6, and case confusions.
 */

char *get_name(char *path);
void find(char *path,char *filename);

int main(int argc,char *argv[])
{
	if(argc < 3)
	{
		printf("usage: %s <path> <filename>\n",argv[0]);
		exit(1);
	}

	find(argv[1],argv[2]);

	exit(0);
}

void find(char *path,char *filename)
{
	int fd; 
	char buf[512],*p;
	struct stat st;
	struct dirent de;

	/* note that < is more prior than = and if not have (), it's a bool value */
	if((fd = open(path,0)) < 0) /* the open(sys call) and the fopen */
	{
		fprintf(2,"find: cannot open %s\n",path);
		return;
	}
	/* 0 is used for only reading, we didn't define the macro */
	
	if(fstat(fd,&st) < 0)
	{
		fprintf(2,"find: cannot stat %s\n",path);
		return;
	}

	switch(st.type)
	{
	case T_FILE:
		/* compare and if equal just print it */
		if(!strcmp(get_name(path),filename))
			printf("%s\n",path);
		break;

	case T_DIR:
		/* get into this dir and find every file or dir too */
		if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf))
		{
			printf("find: path too long\n");
			break;
		}
		strcpy(buf,path);
		p = buf + strlen(buf);
		*p++ = '/';
		while(read(fd,&de,sizeof(de)) == sizeof(de))
		{
			if(de.inum == 0) /* ??? */
				continue;
		
			memmove(p,de.name,DIRSIZ); /* the size of de.name is DIRSIZ */
			p[DIRSIZ] = 0; /* it's same as '\0' */
			if(!strcmp(get_name(buf),".") || !strcmp(get_name(buf),".."))
				continue;

			find(buf,filename);
		}

		break;
	}
	close(fd); /* don't forget to close: 1.cause memory leak 2.fd numbers is limited */
}

char *get_name(char *path)
{
	char *p;
	for(p = path + strlen(path);p >= path && *p != '/';p--)
		;
	p++;
	
	return p;
}


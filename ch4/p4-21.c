#include "apue.h"
#include "myerr.h"
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int fd;
	struct stat buf;
	struct timespec time[2];

	for(int i = 0; i < argc; i++)
	{
		if(stat(argv[i], &buf) < 0)
		{
			err_ret("%s : stat error", argv[i]);	
			continue;
		}			
		
		if((fd=open(argv[i], O_RDWR | O_TRUNC)) < 0)
		{
			err_ret("%s : open error", argv[i]);
			continue;
		}	
		
		time[0] = buf.st_atim;
		time[1] = buf.st_mtim;

		if(futimens(fd, time) < 0)
		{
			err_ret("%s : futimens error", argv[i]);
		}
		
		close(fd);
	}
	return 0;
}


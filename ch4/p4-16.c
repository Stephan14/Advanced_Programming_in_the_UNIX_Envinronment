#include"apue.h"
#include"myerr.h"
#include<fcntl.h>

int main()
{
	if(open("tempfile", O_RDWR) < 0)
		err_sys("open error");
	if(unlink("tempfile") < 0)
		err_sys("unlink error");

	printf("file unlink\n");
	sleep(15);
	printf("done");
	return 0;
}

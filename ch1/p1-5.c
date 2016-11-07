#include "apue.h"
#include "myerr.h"
#include <sys/wait.h>

int 
main( int argc, char *argv[] )
{
	char buf[MAXLINE];
	pid_t pid;
	int status;
	
	printf("%%");
	while( fgets( buf, MAXLINE, stdin) != NULL )
	{
		if( buf[ strlen( buf ) - 1] == '\n' )
			//用null代替换行符
			buf[ strlen( buf ) - 1 ] = 0;
		
		if( ( pid = fork() ) < 0 )
		{
			err_sys("fork error");
		}//子进程
		else if( pid == 0 )
		{
			execlp( buf, buf, (char*) 0);
			err_ret("couldn't execute: %s ", buf );
			exit( 127 );
		}
		//等待进程ID为pid的进程终止
		if( pid = waitpid( pid, &status, 0) < 0 )
			err_sys("waitup error");	
		printf("%%");
	}	
	return 0;
}

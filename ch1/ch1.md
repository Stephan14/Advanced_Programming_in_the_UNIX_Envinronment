## 1.5输入和输出
##### 1.文件描述符
##### 2.标准输出、标准输入和标准错误
##### 3.不用缓冲的IO
	函数open，read，write，lseek和close提供不用缓冲的IO
	<unistd.h>以及STDIN_FILENO和STDOUT_FILENO是posix标准的一部分,
	STDIN_FILENO和STDOUT_FILENO是在<unistd.h>中定义的
##### 4.标准IO
	标准IO函数提供了一种对不用缓存IO函数的带缓冲的接口，
	标准IO的优点：
		1.无需考虑设置最佳的缓冲区大小
		2.简化了对输入行的处理
	EOF,stdin,stdout在<stdio.h>文件中

## 1.6程序和进程
##### 1.程序
	程序：存储于某一个磁盘上的可执行程序，使用6个exec函数中的一个
	由内核读入存储器并执行。
##### 2.进程和进程ID
##### 3.进程控制
		

## 1.4文件和目录
##### 1.文件系统
    目录：包含许多目录项的文件，在逻辑上可以认为每个目录项都
    包含一个文件名，同时还包含文件属性的信息，例如：文件类型、
    文件大小、文件所有者等信息
##### 2.文件名
    斜线/和空操作符不能够出现在文件名中，斜线用来分隔路径名，
    空操作符用来终止一个路径名
##### 3.路径名
    绝对路径名、相对路径名
##### 4.工作目录
    所有相对路径都是从工作目录开始解释
##### 5.起始目录
    登录时，工作目录设置为起始目录
##### 相关函数
    opendir
    readdir
    closedir
    chdir:更改工作目录

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
##### 问题
    FILE *stdout 和 STDOUT_FILENO 的区别？
    stdin类型为 FILE*
    STDIN_FILENO类型为 int
    使用stdin的函数主要有：fread、fwrite、fclose等，基本上都以f开头
    使用STDIN_FILENO的函数有：read、write、close等
    操作系统一级提供的文件API都是以文件描述符来表示文件。STDIN_FILENO就是标准输入设备（一般是键盘）的文件描述符,没有缓存的IO
    
## 1.6程序和进程
##### 1.程序
	程序：存储于某一个磁盘上的可执行程序，使用6个exec函数中的一个
	由内核读入存储器并执行。
##### 2.进程和进程ID
##### 3.进程控制
		
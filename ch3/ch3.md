## 1.引言
   不带缓存的IO函数：read,write,open,close,lseek。其中read和write都是调用系统调用，这些函数并不是ISO C的组成部分，但是，是POSIX.1和SUS的组成部分
   
## 2.文件描述符
   文件描述符是一个非负整数，当打开一个现有的文件或者创建一个新文件描时，内核向进程返回一个文件描述符，当读、写一个文件的时候，使用open或者creat返回的文件描述符来标识文件。
   在符合POSIX.1的应用程序中，文件描述符0、1、2被用STDIN_FILENO、STDOUT_FILENO和STDERR_FILENO来表示用来提高可读性

## 3.函数open和openat
   函数原型：
   `
   #include<fcntl.h>
   int open( const char* path, int oflag, .../*mode_t mode*/ );
   int openat( int fd, const char *path, int oflag, ... /*mode_t mode*/);
   `
   以上两个函数返回值：如成功，返回文件描述符；若出错，返回-1。
   其中，最后一个参数为...,表示余下的参数的数量和类型是可变的；path表示要打开的文件路径；oflag表示可以用来说明此函数的多个选项,可以使用或操作组合多个选项，这些选项以常量的形式在<fcntl.h>定义如下:
   O_RDONLY  只读打开
   O_WRONLY  只写打开
   O_RDWR    读、写打开
   O_EXEC    只执行打开
   O_SEARCH  只搜索打开：在目录打开时验证搜索权限，对目录的文件描述符的后续操作就不需要再次检查对该目录的搜索权限
   以上5个常量必须指定一个并且只能执行一个,以下选项是可选的：
   O_APPEND  追加写
   O_CLOEXEC 把FD_CLOEXEC常量设定成文件描述符标志
   O_CREAT   若文件不存在，则创建，使用此选项，需要使用各个函数的最后一个参数mode,通过mode指定文件的访问权限
   O_DIRECTORY 如果path不是目录，则出错
   O_EXCL    如果O_CREAT也同时被指定而文件已存在的话会产生一个错误 。这个文件是否存在的测试以及当文件不存在时文件的创建是一个原子操作
   O_TRUNC   如果此文件存在，而且为只写或者读写成功打开，则将其文件长度设置为0
   O_NOCTTY  如果路径名指向一个终端设备，则不要为这个进程把这个设备作为控制终端分配
   O_NONBLOCK 如果文件名指向一个FIFO，一个块特殊文件或一个字符特殊文件，这个选项为打开文件及后续I/O设置非阻塞模式
   O_SYNC    让每个write都等待物理I／O的完成，包括write引起的文件属性更新的修改
   O_DSYNC   让每个write都等待物理I／O的完成，但如果不影响读取已写入的数据，则不等待文件属性的更新
   O_RSYNC   让每个在文件描述符上的read操作都等待，直到任何正在执行的在文件的相同部分上的写操作都结束为止
   O_DSYNC和O_SYNC标志相似，但稍微有些不同。O_DSYNC标志仅当文件属性的更新反应了文件数据的改变时才会影响文件的属性（例如，更新文件大小来反映更多的数据。）对于O_SYNC标志，数据和属性一直都同步地更新。当使用O_DSYNC选项覆写一个文件已经存在的部分时，文件的时间不会同步更新。相反，如果我们以O_SYNC标志打开这个文件，每个write都会在write返回前更新这个文件的时间，而不管我们在覆写存在的字节还是添加到这个文件

   fd参数将open函数和openat函数区分开来，有三种可能性：
    1. path参数是绝对路径，fd参数被忽略，open函数就相当于openat函数
    2. path参数为相对路径，fd参数指出了相对路径名在文件系统中的开始地址，并且fd参数是通过打开相对路径名所在的目录来获取
    3. path参数指定了相对路径名，fd参数具有特殊值AT_FDCWD。在这种情况下，路径名在当前工作目录中获取，openat函数在操作上与open函数类似

   POSIX.1函数引入的原因：
    1.可以使用相对路径名打开目录中文件，而不在只能打开当前的工作目录
    2.避免TOCTTOU错误(指计算机系统的资料与权限等状态的检查与使用之间，因为特定状态在这段时间已发生改变所产生的软件漏洞)

##### 文件名和路径名截断 
   若_POSIX_NO_TRUNC有效，则在整个路径名超过PATH_MAX或者路径名中任意文件名超过NAME_MAX时，出错返回，并将errno设置为ENAMETOOLONG

## 4.函数creat
   函数原型：
```
#include <fcntl.h>
int creat( const chatr* path, mode_t mode )
```
返回值：若成功，返回为只写打开的文件描述符；若出错，返回-1
这个函数等价于： open( path, O_WRONLY | O_CREAT | O_TRUNC, mode ）
不足之处:creat只能够以只写的方式打开文件，可以使用如下的方法实现此功能：
 open( path, O_RDWR | O_CREAT | O_TRUNC, mode ）

## 5.函数close
   函数原型：
   ```
#include <unisted.h>
int close(int fd);
   ```
返回值：若成功，返回0；失败，返回-1

## 6.函数lseek
   通常读写操作是从当前文件的偏移量开始，并使偏移量增加所读写的字节数。默认情况下，除非指定O_APPEND选项，否则该偏移量被设置为0
   可以使用lseek函数设置偏移量的位置：
   ```
#include <unisted.h>
off_t lseek( int fd, off_t offset, int whence );
   ```
返回值：若成功，返回新的文件的偏移量；否则，返回-1
offse参数的解释与whence参数的值有关：SEEK_SET、SEEK_CUR、SEEK_END
若lseek执行成功，返回新的文件偏移量，故可通过 lseek( fd, 0, SEEK_CUR  ); 确定当前偏移量,可用来确定所涉及的文件是否可以设置偏移量，管道、FIFO或网络套接字不可以，lseek返回-1，errno被设置为ESPIPE
lseek仅将文件的偏移量记录在文件内核中，并不引起任何的IO操作，然后，偏移量用于另一个文件的读写。
文件的偏移量可以大于当前的长度，此时，对文件的下一次写将会增加该文件长度，并在文件中构成一个空洞，文件中的空洞并不要求占用磁盘上的空间，具体的处理方式根据操作系统来决定

## 7.函数read
函数原型
```
#include <unisted.h>
ssize_t read( int fd, void *buf, size_t nbytes );
```
返回值：读到的字节数，若到达文件尾，返回0；若出错，返回-1；

有多种情况可使实际读到的字节数少于要求的求读的字节数：
  读普通文件时，在读到要求字节数之前已经到达文件尾端
  当从终端设备读的时候，通常一次最多只读一行
  当从网络读时，网络中的缓冲机制可能造成返回值小于要求读的字节数
  当从管道或者FIFO读时，若管道包含的字节数少于所需的数量，那么read将只会返回实际可用的字节数
  当从某些面向记录的设备读时，一次最多返回一个记录
  当一个信号造成中断，而且已经读了一部分数据的时候，读操作从文件的当前的偏移量开始，在成功返回之前，该偏移量将会增加实际读到的字节数

## 8.函数write
函数原型：
```
#include <unsitd.h>
ssize_t write( int fd, void *buf, size_t nbytes );
```
返回值：若成功，返回已经写的字节数；若出错，返回-1
其返回值通常与参数nbytes的值相同，否则表示出错。write出错的一个常见原因就是磁盘写满了或者超过了一个给定进程的文件长度限制。

## 9.IO效率
   合理设置缓存区的长度可以进行数据的预读可以提到IO效率

## 10.文件共享
   ![两个独立进程打开同一个文件]( https://github.com/Stephan14/Advanced_Programming_in_the_UNIX_Envinronment/blob/master/ch3/%E4%B8%A4%E4%B8%AA%E8%BF%9B%E7%A8%8B%E6%89%93%E5%BC%80%E5%90%8C%E4%B8%80%E4%B8%AA%E6%96%87%E4%BB%B6.png )


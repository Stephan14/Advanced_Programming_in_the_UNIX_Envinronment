## 1.引言

## 2.函数stat、fstat、fstatat和lstat
```
#inlcude<sys/stat.h>
int stat( const char* restrict pathname, struct stat* restrict buf );
int fstat( int fd, struct stat *buf );
int lstat( const char* restrict pathname, struct stat* restrict buf );
int fstatat( int fd, const char* restrict pathname, struct stat *restrict buf, int flag );
```

stat()返回pathnanme的文件的相关信息

fstat()获得已在此描述符上打开的文件相关信息

lstat()类似于stat，但是当pathname为链接时，返回链接的相关信息，而不是文件的信息

fstatat()返回一个相对于打开当前目录（用open函数打开的目录的fd表示）的路径名返回文件信息；当AT_SYMLINK_NOFOLLOW标志被设置时，fststat（）函数不会跟随链接，而是返回链接的文件的信息；当fd的值为AT_FDCWD并且pathname是相对路径名，fstatat会计算相对于当前目录的pathname的参数，如果pathname是绝对路径名，fd参数会被忽略。

结构体stat的内容如下：
![](https://github.com/Stephan14/Advanced_Programming_in_the_UNIX_Envinronment/blob/master/ch4/stat%E7%BB%93%E6%9E%84%E4%BD%93%E7%9A%84%E5%86%85%E5%AE%B9.png)
其中timespec结构体类型按照秒和纳秒定义了时间，包含以下两个字段：
```
time_t tv_sec;
long tv_nsec;
```

## 3.文件类型
1. 普通文件：包含了文本或者二进制形式的数据。对于二进制可执行程序，所有文件都遵循一种标准化的格式，使内核能够理解。
2. 目录文件：包含其他文件的名字以及指向这些文件的指针。对一个目录具有读权限的任意一个进程都可以读该目录的内容，但是，只有内核可以直接写文件。
3. 块特殊文件：这种文件提供对设备带缓冲的访问，每此访问以固定长度为单位进行。
4. 字符特殊文件：这种文件提供对设备不带缓冲的访问，每此访问的长度不可变。
5. FIFO：命名管道，用于进程之间的通信
6. 套接字：用于进程之间的网络通信
7. 符号链接：这种类型的文件指向另一个文件

可以使用以下文件中的宏确定文件类型，其参数是stat结构中的成员st_mode成员：
![](https://github.com/Stephan14/Advanced_Programming_in_the_UNIX_Envinronment/blob/master/ch4/%E6%96%87%E4%BB%B6%E4%B8%AD%E7%9A%84%E5%AE%8F.png)

## 4.设置用户ID和设置组ID

一个进程的相关ID：

| 相关ID              | 相关用途 |
| 实际用户ID   实际组ID | 我们实际上是谁 |
| 有效用户ID   有效组ID | 用于文件访问权限的检查 |
| 保存的设置用户ID 保存的设置组ID | 由exec函数保存 |

在可以测试常量_POSIX_SAVEID_IDS或者运行时以参数_SC_SAVED_IDS调用函数sysconf判断是否支持以上功能

可以通过在文件st_mode中设置特殊标记从而使当执行此文件时，将进程的有效用户ID设置为文件所有者的用户ID，这个特殊的标记叫做设置用户ID位；设置组ID也是类似。

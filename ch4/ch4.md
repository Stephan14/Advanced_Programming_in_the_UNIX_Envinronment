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

fstatat()返回一个相对于打开当前目录（用open函数打开的目录的fd表示）的路径名返回文件信息；当flag参数被AT_SYMLINK_NOFOLLOW标志被设置时，fststat（）函数不会跟随链接，而是返回链接的文件的信息；当fd的值为AT_FDCWD并且pathname是相对路径名，fstatat会计算相对于当前目录的pathname的参数，如果pathname是绝对路径名，fd参数会被忽略。

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

|              :---:          |     :---:          |

| 相关ID                      | 相关用途             |

| 实际用户ID   实际组ID         | 我们实际上是谁        |

| 有效用户ID   有效组ID         | 用于文件访问权限的检查 |

| 保存的设置用户ID 保存的设置组ID | 由exec函数保存       |

在可以测试常量_POSIX_SAVEID_IDS或者运行时以参数_SC_SAVED_IDS调用函数sysconf判断是否支持以上功能

可以通过在文件st_mode中设置特殊标记从而使当执行此文件时，将进程的有效用户ID设置为文件所有者的用户ID，这个特殊的标记叫做设置用户ID位；设置组ID也是类似。

## 5.文件访问权限

st_mode包含文件访问权限位，每个文件有9个访问权限位，分为3类，这三类以各种不同的方式由不同函数使用。

1. 当打开任何的一个文件时，对文件路径中包含的各个目录，都隐含着具有当前目录的都应该具有的执行权限，这就是为什么对于目录其执行权限位常常被称为搜索位。对于一个目录具有读权限表示可以获取文件中的所有的文件列表；具有执行权限表示可以搜索该目录寻找特定的文件。
2. 对于一个文件的读权限决定了我们是否可以打开现有的文件进行读操作。
3. 对于一个文件的写权限决定了我们是否可以打开现有文件进行写操作。
4. 为了在open函数中对一个文件制定O_TRUNC标志，必须对该文件具有写权限。
5. 为了在一个目录中创建一个新文件，必须对该目录具有写权限和执行权限。
6. 为了在一个目录中删除一个文件，必须对该目录具有写权限和执行权限，对文件本身则不需要读写权限。
7. 如果使用7个exec函数中的任何一个执行某个文件，都必须对该文件具有执行权限。

每次进程打开、创建和删除一个文件的时候，都会使用文件的拥有者ID、组ID合进程的有效用户ID、有效组ID进行访问权限的测试：
1. 若进程的**有效用户ID**是0（表示超级用户），则允许访问。
2. 若进程的**有效用户ID**等于文件的所有ID，如果所有者的适当的访问权限被设置，则允许访问；否则禁止访问。（若进程的有效ID等于文件的所有ID也就是拥有此文件，则按照访问权限批准和拒绝该进程对文件的访问--不查看组访问权限）
3. 若进程的**有效组ID**或者进程的附属组ID等于文件的所有组ID，如果所有者的适当的访问权限被设置，则允许访问；否则禁止访问。（若进程不拥有该文件，但是进程属于某一个组，则按照组访问权限批准或者拒绝该进程）
4. 若其他用户适当的访问权限被设置，则允许访问；否则拒绝访问。

## 6.新文件和目录的所有权限

新文件的用户ID设置为进程的有效文件ID，而新文件的组ID则可以是以下两种方式中的一种：
1. 新文件的组ID是进程的有效组ID
2. 新文件的组ID是新文件所在目录的组ID
在linux3.2.0中由是否设置了设置组ID位决定组ID由哪种方式确定

## 7.函数access和faccessat
函数原型
```
#include <unistd.h>
int access( const char* pathname, int mode )
int faccessat( int fd, const char *pathname, int mode,
  int flag )
```
返回值：成功返回0；出错，返回-1

以上两个函数按照实际用户ID和实际组ID进行文件访问权限的测试，具体流程与按照有效用户ID测试流程相同。

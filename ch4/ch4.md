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

有些情况下用户希望按照实际用户ID和实际组ID来测试其文件访问权限，而不是按照默认的有效用户ID和有效组ID进行为基础执行有效文件访问权限的测试。为了达到这种目的，通常要设置可执行文件的设置组ID位和设置用户ID位

## 8.函数umask
umask函数为进程设置**文件模式创建屏蔽字**，并返回之前的值
函数原型
```
#include <sys/stat.h>
mode_t umask( mode_t cmask )
```
返回值：之前的文件模式创建屏蔽字

#### 注意
1. 用户可以在进程运行时修改umask的值，更改进程的文件模式创建屏蔽字并不影响其父进程的屏蔽字
2. 用户可以设置umask值以控制他们所创建文件的默认权限
3. 通常cmask表示成八进制数字，一位代表一种要屏蔽的权限，相关权限的说明如下![](https://github.com/Stephan14/Advanced_Programming_in_the_UNIX_Envinronment/blob/master/ch4/stat%E7%BB%93%E6%9E%84%E4%BD%93%E7%9A%84%E5%86%85%E5%AE%B9.png)

## 9.函数chmod、fchmod和函数fchmodat
函数原型
```
#inlcude <sys/stat.h>
int chmod(const char* pathname, mode_t mode);
int fchmod(const char* pathname, mode_t mode);
int fchmodat(int fd, const char* pathname, mode_t mode, int flag);
```
以上三个函数可以使我们在现有的基础上修改文件的访问权限，其中参数mode的说明如下：![](https://github.com/Stephan14/Advanced_Programming_in_the_UNIX_Envinronment/blob/master/ch4/chmod%E5%87%BD%E6%95%B0%E7%9A%84mode%E5%B8%B8%E9%87%8F.png)

chmod函数在指定的文件上修改访问权限

fchmod函数对已经打开文件上修改文件进行操作（尝试一下对正在写的文件修改权限）

fchmodat函数与chmod在以下两种情况下是一样的：
1. pathname是绝对路径
2. fd参数为AT_FDCWD而pathname参数为相对路径

否则，fchmodat函数计算相对于打开目录（由参数fd指定）的pathname，当flag参数设置为AT_SYMLINK_NOFOLLOW标志，不跟随符号链接。

#### 注意
1. 为了改变一个文件的权限位，进程的有效用户ID必须等于文件的所有者ID，或者该进程必须拥有超级用户权限
2. chmod更新的只是i节点最后一次被更改的时间，而ls输出文件内容最后一个呗修改的时间
3. chmod函数在下面两种情况下自动清除两个权限位：
  1. 在FreeBSD、Solaris系统中如果试图设置普通文件的粘着位（S_ISVTX），而且又没有超级用户权限，那么mode中的粘着位会被自动关闭，但是，在linux和mac中并没有这种限制
  2. 如果新文件的组ID不等于进程的有效组ID或者进程附属组ID中的一个，而且进程没有超级用户权限，那么设置组ID位会被自动关闭。

## 10.粘着位
对于文件：

  如果对于一个可执行文件设置了粘着位，在文件列表时看到一个可执行文件的访问权限中有“t”，那么这就意味着该脚本或程序在执行时会被放在交换区(虚存），这样使得下次再执行此可执行程序的时候能较快的装在到内存中。

对于目录：

  如果对于一个目录设置了粘着位，只对该目录具有写权限并且满足下面条件之一的用户才能删除或者重命名该目录一下的文件：
    1. 拥有此文件
    2. 拥有此目录
    3. 是超级用户
  在linux中通常/var/tmp和/tmp目录设置了粘着位，这就使用户不能删除或者重命名其他人的文件，只能操作自己的文件。

## 11.函数chown、fchown、fchownat和lchown
函数原型：
```
#include <unistd.h>
int chown(const char *pathname, uid_t owner, gid_t group);
int fchown(int fd, uid_t owner, gid_t group);
int fchownat(int fd, const char *pathname, uid_t owner, gid_t group, int flag)；
int lchown(const char *pathname, uid_t owner, gid_t group);
```

在所引用的文件是符号链接的情况下，fchownat（如果设置了AT_SYMLINK_NOFOLLOW）和lchown函数更改的是符号链接本身的所有者，而不是该符号链接所指向文件的所有者。

chown不能用于改变符号链接所有者

fchownat与chown函数或者lchown函数在两种情况下是相同的：一是pathname是绝对路径，二是fd参数为AT_FDCWD而pathname参数为相对路径。在以上两种情况下，如果flag参数中设置AT_SYSLINK_NOFOLLOW,则fchmodat与lchown行为相同；如果清除AT_SYSLINK_NOFOLLOW，则fchmodat与chown行为相同

当_POSIX_CHOWN_RESTRICTED对指定文件生效时，只有超级用户可以修改文件的用户ID，普通用户不能更改其他用户ID,但是，普通用户可以更改自己所属文件的组ID(owner等于-1或者文件用户ID)，但是只能更改到你所属的组

#### 注意
1. 如果这些函数由非超级用户进程调用，则在成功返回时，该文件的设置用户ID位和设置组ID位都被清除
2. owner和group ID可以从/etc/passswd中查到

## 12.文件长度
tat结构的st_size成员以字节为单位表示文件的长度，此字段只对普通文件，目录文件和符号链接有意义。

stat结构的st_blksize成员是对文件IO较合适的块长度，在介绍系统IO调用时，该值为4096B。

stat结构的st_blocks成员是所分配的实际512字节块数量。

## 13.文件截断
有时候需要将文件缩短或者增长，其中将文件的长度截短为0是个特例，在打开文件的时候设置O_TRUNC标志可以做到这一点。

为了截断文件可以调用函数truncate和ftruncate,函数原型：
```
#include <unistd.h>
int truncate(const char *pathname, off_t length);
int ftruncate(int fd, off_t length);
```

返回值：成功返回0；失败返回-1

以上两个函数将文件长度截断为length，如果文件的原来长度小于length，文件长度将会增加，以前文件尾端和新文件的尾端之间的数据将会读作0（也有可能增加文件空洞）

## 14.文件系统
![磁盘、分区和文件统](https://github.com/Stephan14/Advanced_Programming_in_the_UNIX_Envinronment/blob/master/ch4/%E7%A3%81%E7%9B%98%E3%80%81%E5%88%86%E5%8C%BA%E5%92%8C%E6%96%87%E4%BB%B6%E7%B3%BB%E7%BB%9F.png)
![](https://github.com/Stephan14/Advanced_Programming_in_the_UNIX_Envinronment/blob/master/ch4/%E8%BE%83%E8%AF%A6%E7%BB%86%E7%9A%84%E6%9F%B1%E9%9D%A2%E7%BB%84%E7%9A%84i%E8%8A%82%E7%82%B9%E5%92%8C%E6%95%B0%E6%8D%AE%E5%9D%97.png)
1. 在上面的图中两个目录项指向同一个节点，每个i节点都有一个链接计数，其值是指向该节点的目录项数，存储在st_nlink中，这种类型的链接称为硬链接。只有该链接的值为0时，才可以删除该文件。
2. 符号链接：符号链接的文件包含了该链接指向的文件的名字，文件类型为S_IFLNK。
3. i节点包含了文件的所有吧信息：文件类型，文件访问权限，文件长度和文件所指向的数据块，只有两项重要的数据存放在目录中：文件名和对应的i节点。
4. 由于目录项中的i节点编号指向同一个文件系统中相应的i节点，所以ln命令不能跨文件系统
5. 当不更换文件系统为一个文件重命名的时候，文件内容没有改变，只是删除旧的目录项，创建新的目录项。

目录的链接计数，如下图
![](https://github.com/Stephan14/Advanced_Programming_in_the_UNIX_Envinronment/blob/master/ch4/%E5%88%9B%E5%BB%BA%E4%BA%86testdir%E5%90%8E%E6%96%87%E4%BB%B6%E7%B3%BB%E7%BB%9F%E5%AE%9E%E4%BE%8B.png)

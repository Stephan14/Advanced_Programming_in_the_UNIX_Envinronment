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

## 15.函数link、linkat、unlink、unlinkat和remove
函数原型：
```
#inlcude <unistd.h>
int link(const char* existingpath, const char* newpath);

int linkat(const char* efd, const char* existingpath, int nfd, const char* newpath, int flag);
```
以上两个函数创建一个新的目录项newpath，引用现有的文件existingpath。如果newpath已经存在则返回错误，并且只创建newpath路径中的最后一个分量，其前缀应该是已经存在的。

对于linkat函数来说，有几点需要说明一下：
1. 现有文件是通过efd和existingpath两个参数指定的，新的路径名是通过nfd和newpath来指定的。默认情况下，如果两个文件中的任意一个是相对路径，则需要通过相应的文件标识符来计算。如果两个文件描述符中任意一个设置为AT_FDCWD，那么相应的路径（如果是相对路径）就可以通过相对于当前目录进行计算。如果是绝对路径则会忽略相应的文件标识符。
2. 当现有文件是符号链接时（软连接）,如果在flag参数中设置了AT_SYSLINK_NOFOLLOW,则创建指向符号链接最终目标的链接，否则，创建指向符号链接本身的链接

#### 注意
1. 创建新的目录项和增加链接计数应当是一个原子操作
2. 大部分系统不支持创建目录的硬链接，因为那样可能引入循环

函数原型：

```
#inlcude <unistd.h>
int unlink(const char* existingpath, const char* newpath);

int unlinkat(const char* efd, const char* existingpath, int nfd, const char* newpath, int flag);
```
以上两个函数实现删除一个目录项，并将由pathname所引用的文件链接数减1。并且，必须对一个包含该目录项的目录具有读写权限,如果对该目录设置的粘着位，必须对该目录具有写权限，并且具备下面三个条件：
1. 拥有该文件
2. 拥有该目录
3. 具有超级用户

#### 注意
1. 关闭一个文件的时候，内核首先检查打开该文件的进程的数量，如果该计数值为0，然后检查其链接的数量，如果链接的数量为0，则删除该文件
2. 对于unlinkat函数而言，如果fd参数设置为AT_FDCWD，那么通过相对于进程当前工作目录来计算目录名，如果pathname是绝对路径，那么fd参数将会被忽略。
3. flag参数给出了一种方法，使调用进程可以改变unlinkat函数的默认行为。当flag值被设置为AT_ REMOVEDIR标志时，unlinkat函数可以类似于rmdir一样删除目录。如果这个标志被清除，unlinkat与unlink执行同样的操作。
4. **unlink的这种特性经常被程序用来确保即使是在程序崩溃时，它所创建的临时文件也不会遗留下来。进程用open或creat创建一个文件，然后立即调用unlink，因为该文件仍旧是打开的，所以不会将其内容删除。只有当进程关闭该文件或终止时（在这种情况下，内核关闭该进程所打开的全部文件），该文件的内容才被删除。**
5. 如果pathname是符号链接，那么unlink删除该符号链接，而不是删除由该链接所引用的文件。给出符号链接名的情况下，没有一个函数能删除由该链接所引用的文件。

如果文件系统支持的话，超级用户可以调用unlink，其参数pathname指定一个目录，但是通常应当使用rmdir函数，而不使用unlink这种方式。

我们也可以用remove函数解除对一个文件或目录的链接。对于文件，remove的功能与unlink相同。对于目录，remove的功能与rmdir相同。

## 16.函数rename和renameat
函数原型
```
#include <stdio.h>     
int rename(const char *oldname, const char *newname);     
int renameat(int oldfd, const char *oldname, int newfd, const char *newname)
```
对newname已经存在进行说明：
1. 如果oldname指的是一个文件而不是目录，那么为该文件或符号链接重命名。在这种情况下，如果newname已存在，则它不能引用一个目录。如果newname已存在，而且不是一个目录，则先将该目录项删除然后将oldname重命名为newname。对包含oldname的目录以及包含newname的目录，调用进程必须具有写权限，因为将更改这两个目录。

2. 如若oldname指的是一个目录，那么为该目录重命名。如果newname已存在，则它必须引用一个目录，而且该目录应当是空目录（空目录指的是该目录中只有.和..项）。如果newname存在（而且是一个空目录），则先将其删除，然后将oldname重命名为newname。另外，当为一个目录重命名时，newname不能包含oldname作为其路径前缀。例如，不能将/usr/foo重命名为/usr/foo/testdir，因为旧名字（/usr/foo）是新名字的路径前缀，因而不能将其删除。

3. 如若oldname或newname引用符号链接，则处理的是符号链接本身，而不是它所引用的文件。

4. 不能对.和..重命名。更确切地说，.和..都不能出现在oldname和newname的最后部分。

5. 作为一个特例，如果oldname和newname引用同一文件，则函数不做任何更改而成功返回。

#### 注意
oldfd或newfd参数（或两者）都能设置成AT_FDCWD，此时相对于当前目录来计算相应的路径名。

## 17.符号链接
引入符号链接的原因：
1. 硬链接通常要求链接和文件位于同一文件系统中
2. 只有超级用户才能创建指向目录的硬链接（在底层文件系统支持的情况下）

各个函数符号链接的处理如下：
![]()
#### 注意
1. 使用符号链接可能在文件系统中引入循环
2. 符号链接的源文件不一定要求一定存在

## 18.创建和读取符号链接
函数原型：
```
#include<unistd.h>  
int symlink(const char* actualpath,const char* sympath);  
int symlinkat(const char* actualpath, int fd, const char* sympath);
```
在创建符号链接的时候，并不要求actualpath已经存在。

```
#include<unistd.h>
ssize_t readlink(const char* restrict pathname,char * restrict buf ,size_t bufsize);
ssize_t readlinkat(int fd, const char* restrict pathname,char * restrict buf ,size_t bufsize);
```

## 19.文件时间
unix系统中保存以下三种文件的时间：
![与每个文件相关的三个值.png](https://github.com/Stephan14/Advanced_Programming_in_the_UNIX_Envinronment/blob/master/ch4/%E4%B8%8E%E6%AF%8F%E4%B8%AA%E6%96%87%E4%BB%B6%E7%9B%B8%E5%85%B3%E7%9A%84%E4%B8%89%E4%B8%AA%E5%80%BC.png)
#### 注意
系统并不保存对一个i节点 的最后一次访问时间，所以access和stat函数并不更改这三个时间里的任何一个

目录是包含目录项的文件，增加，删除或者修改目录项会影响到与其所在相关的三个时间。例如：创建一个新文件会影响到包含此新文件的目录，也会影响到该新文件的i节点。但是读或者写只对本文件及i节点，而对目录没有影响，具体的情况如下：
![各种函数对访问、修改和状态更改时间的作用](https://github.com/Stephan14/Advanced_Programming_in_the_UNIX_Envinronment/blob/master/ch4/%E5%90%84%E7%A7%8D%E5%87%BD%E6%95%B0%E5%AF%B9%E8%AE%BF%E9%97%AE%E3%80%81%E4%BF%AE%E6%94%B9%E5%92%8C%E7%8A%B6%E6%80%81%E6%9B%B4%E6%94%B9%E6%97%B6%E9%97%B4%E7%9A%84%E4%BD%9C%E7%94%A8.png)

## 20.函数futimens、utimensat和utimens
函数原型
```
#include <sys/stat.h>     
int futimens(int fd, const struct timespec times[2]);    
int utimensat(int fd, const char *path, const struct timespec times[2], int flag);
```
返回值：若成功，返回0；若出错，返回-1

函数介绍：

utimensat的flag参数可用于进一步修改默认行为。如果设置了AT_SYMLINK_NOFOLLOW标志，则符号链接本身的时间就会被修改（如果路径名指向符号链接）。默认的行为是跟随符号链接，并把文件的时间改成符号链接的时间

时间戳可以按下列4种方式之一进行指定：

1. 如果times参数是一个空指针，则访问时间和修改时间两者都设置为当前时间。

2. 如果times参数指向两个timespec结构的数组，任一数组元素的tv_nsec字段的值为UTIME_NOW，相应的时间戳就设置为当前时间，忽略相应的tv_sec字段。

3. 如果times参数指向两个timespec结构的数组，任一数组元素的tv_nsec字段的值为UTIME_OMIT，相应的时间戳保持不变，忽略相应的tv_sec字段。

4. 如果times参数指向两个timespec结构的数组，且tv_nsec字段的值为既不是UTIME_NOW也不是UTIME_OMIT，在这种情况下，相应的时间戳设置为相应的tv_sec和tv_nsec字段的值。

执行这些函数所要求的优先权取决于times参数的值：

1. 如果times是一个空指针，或者任一tv_nsec字段设为UTIME_NOW，则进程的有效用户ID必须等于该文件的所有者ID；进程对该文件必须具有写权限，或者进程是一个超级用户进程。

2. 如果times是非空指针，并且任一tv_nsec字段的值既不是UTIME_NOW也不是UTIME_OMIT，则进程的有效用户ID必须等于该文件的所有者ID，或者进程必须是一个超级用户进程。对文件只具有写权限是不够的。

3. 如果times是非空指针，并且两个tv_nsec字段的值都为UTIME_OMIT，就不执行任何的权限检查。

## 21.函数mkidr、mkdirat和rmdir
函数原型：
```
#include <sys/stat.h>     
int mkdir(const char *pathname, mode_t mode);     
int mkdirat(int fd, const char *pathname, mode_t mode);
```
函数返回值：若成功，返回0；若出错，返回-1

函数原型：
```
#include <unistd.h>     
int rmdir(const char *pathname);
```
函数返回值：若成功，返回0；若出错，返回-1

如果调用此函数使目录的链接计数成为0，并且也没有其他进程打开此目录，则释放由此目录占用的空间。如果在链接计数达到0时，有一个或多个进程打开此目录，则在此函数返回前删除最后一个链接及.和..项。另外，在此目录中不能再创建新文件。但是在最后一个进程关闭它之前并不释放此目录。（即使另一些进程打开该目录，它们在此目录下也不能执行其他操作。这样处理的原因是，为了使rmdir函数成功执行，该目录必须是空的。）

## 22.读目录

函数原型
```
#include <dirent.h>  

DIR* opendir(const char* pathname);//成功返回指针，错误返回null

struct dirent *readdir(DIR *dp);//成功返回其指针，若在目录结尾或出错则返回null

void rewinddir(DIR* dp);  

int closedir(DIR* dp);//成功返回0，出错返回-1

long telldir(DIR* dp);//返回与dp关联的目录中的当前的位置  

void seekdir(DIR * dp,long loc);
```

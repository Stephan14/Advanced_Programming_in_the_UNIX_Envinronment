## 2.2UNIX标准化
##### 2.2.1 ISO C
    增加程序的可移植性
##### 2.2.2 IEEE POSIX
##### 2.2.3 Single UNIX Specification
##### 2.2.4 FIPS

## 2.3UNIX系统实现
##### 2.3.1 SVR4
    符合POSIX 1003.1和X/Open Portability Guide第三版标准
##### 2.3.2 4.4BSD
    符合POSIX标准
##### 2.3.3 FreeBSD
##### 2.3.4 Linux
##### 2.3.5 Mac OS X
##### 2.3.6 Solaris
##### 2.3.7 其他系统

## 2.4 标准和实现的关系

## 2.5 限制
   三种限制：
    1.编译时限制（头文件）
    2.不与文件或者目录相关联的运行时限制（sysconf函数）
    3.与文件或目录相关联的运行时限制（pathconf和fpathconf函数）
##### 2.5.1 ISO C限制
   ISO C定义的限制都是编译时限制
   对于不带符号的数据类型都没有列出其最小值，其最小值应该为0
##### 2.5.2 POSIX限制
   限制和常量的分类：
    1.数值限制
    2.最小值
    3.最大值
    4.运行时可以增加的值
    5.运行时不变的值
    6.其他不变值
    7.路径名可变值
   可以使用sysconf、pathconf和fpathconf来获取当前的限制值
##### 2.5.3 XSI限制
   常量限制的分类：
    最小值
    运行时不变的值
##### 2.5.4 函数sysconf、pathconf和fpathconf
   `
    #include <unistd.h>
    long sysconf( int name  );
    long pathconf( const char *pathname, int name );//pathname为路径名
    long fpathconf( int fd, int name );//fd为文件标识符
   `
   以上三个函数如果成功，返回相应的值，如果成功，返回-1
   关于以上三个函数的返回值，详细说明如下：
    1.若name参数并不存在，则返回-1并且将errno置为EINVAL
    2.有些name会返回一个变量或者提示该值不可以确定，但是，不会改变errno的值
    3._SC_CLK_TCK 的返回值是每秒的时钟滴答数
##### 2.5.5不确定的运行时限制
    如果限制值在头文件<limits.h>中没有定义，在编译时也就不能使用；如果值是不确定的，那么在运行时也可能是未定义的
    1.路径名
    2.最大打开文件数
 某些系统实现时，将LONG_MAX作为OPEN_MAX的限制值，这样导致需要关闭文件描述符的上限为2147483647个文件描述符，会浪费很长时间，实际上并没有有使用那么多的文件描述符

## 2.6选项
   posix.1定义的3种处理选项的方法：
    1.编译时定义在<unistd.h>中
    2.与文件或者目录无关的运行时选项用sysconf函数判断
    3.与文件或者目录有关的运行时选项用pathconf和fpathconf函数来判断

## 2.7功能测试宏
   头文件中定义了很多posix.1和XSI符号，同时还加入了自己的定义。如果希望只使用与posix.1定义的符号，可以定义_POSIX_C_SOURCE来排除各个头文件自己的定义，可以通过以下两种方式来定义：
    1.cc -D_POSIX_C_SOURCE=200809L file.c
    2.在源文件的第一行设置#define _POSIX_C_SOURCE 200809L
    
   同样，为了仅使用XSI的选项，可以通过如下的方式：
    1.c99 -D_XOPEN_SOURCE=700 file.c -o file.c
    2.gcc -D_POSIX_C_SOURCE=700 -std=c99 file.c -o file

## 2.8基本系统数据类型
    在<sys/type.h>等头文件中定义一些基本数据类型

## 2.9标准之间的冲突
   ISO C中的clock函数和POSIX.1中的times函数的返回值单位不同
   

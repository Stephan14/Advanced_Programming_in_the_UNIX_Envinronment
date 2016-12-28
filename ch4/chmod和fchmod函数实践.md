代码：
```
#include <stdio.h>  
#include <sys/stat.h>  
  
int main(void){  
    if(chmod("a",S_IRWXU | S_IRWXO | S_ISUID | S_ISVTX)<0){  
        perror("chmod");  
        return -1;  
    }  
    return 0;  
}
```

运行结果：
yan@yan-vm:~/apue$ ll a
-rw-rw-r-- 1 yan yan 0 Jun 12 13:53 a
yan@yan-vm:~/apue$ ./a.out
yan@yan-vm:~/apue$ ll a
-rws---rwt 1 yan yan 0 Jun 12 13:53 a*

如果要在原来的文件属性上加或者减属性可以先使用stat函数获取文件的mode_t，然后再进行与和或操作
代码：
```
#include <stdio.h>  
#include <sys/stat.h>  
  
  int main(void){  
          struct stat statbuf;  
    
          if(stat("a", &statbuf) < 0){  
                          perror("stat");  
                          return -1;  
                  }  
    
          if(chmod("a",(statbuf.st_mode & ~S_IRUSR)|S_IWGRP)<0){ //去除文件a的用户读，增加组写  
                                                                               perror("chmod");  
                                                                               return -1;  
                                                                       }  
          return 0;  
  } 
```

运行结果：
yan@yan-vm:~/apue$ ll a
-rws---rwt 1 yan yan 0 Jun 12 13:53 a*
yan@yan-vm:~/apue$ ./a.out
yan@yan-vm:~/apue$ ll a
--ws-w-rwt 1 yan yan 0 Jun 12 13:53 a*

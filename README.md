# UNIX-Environment

# 进程
## 1、进程标识符pid
### 类型pid_t
### 命令ps
### 进程号是顺次向下使用
### getpid()
### getppid()
## 2、进程的产生
### （1）fork()
#### fork后父子进程的区别：
##### 1）fork的返回值不一样
##### 2）pid和ppid不同
##### 3）未决信号和文件锁不继承
##### 4）资源利用量清零
#### init进程是所有进程的祖先进程
程序中父进程创建了子进程，并不一定是谁先执行，而是由调度器的调度策略决定哪个进程先执行。用例`/home/yzx/test/fork.cpp`(要用fflush刷新所有打开的流)
### （2）vfork()
## 3、进程的消亡以及释放资源
### wait()
### waitpid()
### waitid()
### wait3()
### wait4()
## 4、exec函数族
### execl()
### execlp()
### execle()
### execv()
### execvp()
## 5、用户权限以及组权限
### u+s
### g+s
#### getuid()
#### geteuid()
#### getpid()
#### getepid()
#### setuid()
#### setgid()
#### seteuid()
#### setegid()
#### setreuid()
#### setregid()
## 6、扩展：解释器文件
## 7、system()
### fork+exec+wait封装
## 8、进程会计（非重点）
### acct
## 9、进程时间
### times
## 10、守护进程
### 会话session，标识sid
### 终端
### setsid()
### getpgrp()
### getpgid()
### setpgid()
### 单实例守护进程：锁文件
### 启动脚本文件
## 11、系统日志
### syslogd服务
### openlog()
### syslog()
### closelog()
# 并发
## 信号
### 1、信号的概念
信号是软件中断
信号的响应依赖于中断
### 2、signal();
```
    #include <signal.h>

    typedef void (*sighandler_t)(int);

    sighandler_t signal(int signum, sighandler_t handler);
```
信号会打断阻塞的系统调用
### 3、信号的不可靠
### 4、可重入函数
### 5、信号的响应过程
### 6、信号相关的常用函数
#### 6.1、kill();
#### 6.2、raise();
#### 6.3、alsrm();
#### 6.4、pause();
#### 6.5、abort();
#### 6.6、system();
#### 6.7、sleep();
### 7、信号集
### 8、信号屏蔽字/pendding集的处理
### 9、扩展
#### 9.1、sigsuspend();
#### 9.2、sigaction();
#### 9.3、setitimer();
### 10、实时信号
## 线程
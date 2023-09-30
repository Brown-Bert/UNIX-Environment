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
```
eg: signal01.cpp
```
### 2、signal();
```
    #include <signal.h>

    typedef void (*sighandler_t)(int);

    sighandler_t signal(int signum, sighandler_t handler);
```
信号会打断阻塞的系统调用
### 3、信号的（行为）不可靠
### 4、可重入函数
所有的系统调用都是可重入的，一部分库函数也是可重入的
### 5、信号的响应过程
信号从响应到收到有一个不可避免的延迟
标准信号的响应没有严格的顺序
不能从信号响应函数随意往外跳（setjmp, longjmp）
### 6、信号相关的常用函数
#### 6.1、kill();
```
    #include <sys/types.h>
    #include <signal.h>

    int kill(pid_t pid, int sig);

DESCRIPTION
    If pid is positive, then signal sig is sent to the process  with  the ID specified by pid.

    If  pid  equals  0,  then sig is sent to every process in the process group of the calling process.

    If pid equals -1, then sig is sent to every process  for  which  the calling  process has permission to send signals, except for process 1(init), but see below.

    If pid is less than -1, then sig is sent  to every  process  in  the process group whose ID is -pid.

    If  sig  is  0,  then no signal is sent, but existence and permission checks are still performed; this can be used to check for  the  exis‐tence  of a process ID or process group ID that the caller is permit‐ted to signal.
```
#### 6.2、raise();
```
    #include <signal.h>

    int raise(int sig);

    给自己发送信号，相当于
    kill(getpid(), sig);
```
#### 6.3、alarm();
```
    #include <unistd.h>

    unsigned int alarm(unsigned int seconds);
```
```
    eg: signaltime.cpp 
        signalalarm.cpp
```
```
    #include <sys/time.h>

    int getitimer(int which, struct itimerval *curr_value);
    int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);

    struct itimerval {
        struct timeval it_interval; /* Interval for periodic timer */
        struct timeval it_value;    /* Time until next expiration */
    };

    struct timeval {
        time_t      tv_sec;         /* seconds */
        suseconds_t tv_usec;        /* microseconds */
    };

```
#### 6.4、pause();
```
    #include <unistd.h>

    int pause(void);
```
#### 6.5、abort();
给当前进程发送一个SIGABRT的信号，终止当前信号
```
    #include <stdlib.h>

    void abort(void);

```
#### 6.6、system();
```
    #include <stdlib.h>

    int system(const char *command);

    During  execution of the command, SIGCHLD will be blocked, and SIGINT and SIGQUIT will be ignored, in  the  process  that  calls  system().
```
#### 6.7、sleep();
不建议使用sleep，因为在有些平台sleep底层使用的是alarm+pause封装，当程序本身有使用信号alarm的时候，如果再使用sleep就有可能造成alarm的覆盖，从而导致程序混乱（有些平台使用nanosleep封装，此时就不会产生混乱）看手册
### 7、信号集
#### 信号集类型：sigset_t
```
    #include <signal.h>

    int sigemptyset(sigset_t *set);

    int sigfillset(sigset_t *set);

    int sigaddset(sigset_t *set, int signum);

    int sigdelset(sigset_t *set, int signum);

    int sigismember(const sigset_t *set, int signum);

```
### 8、信号屏蔽字/pendding集的处理
```
    #include <signal.h>

    /* Prototype for the glibc wrapper function */
    int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);

    eg: signalblock.cpp

    #include <signal.h>

    int sigpending(sigset_t *set);
```
### 9、扩展
#### 9.1、sigsuspend();
```
    #include <signal.h>

    int sigsuspend(const sigset_t *mask);

    eg: sigsuspend.cpp
```
#### 9.2、sigaction();
```
    #include <signal.h>

    int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);

    struct sigaction {
        void     (*sa_handler)(int); // 可防止重入
        void     (*sa_sigaction)(int, siginfo_t *, void *); // 可判断信号来源
        sigset_t   sa_mask;
        int        sa_flags;
        void     (*sa_restorer)(void);
    };
    相比于signal注册信号的行为，sigaction可防止signal的重入现象，以及signal无法判断信号的来源（看手册）
```
### 10、实时信号
实时信号不丢失，要排队有上限（ulimit -a）
## 线程
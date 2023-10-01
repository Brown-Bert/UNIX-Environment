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
#### 常用信号
##### SIGINT
当用户按中断键(一般采用Delete或 Ctrl+C)时，终端驱动程序产生此信号并发送至前台进程组中的每一个进程。当一个进程在运行时失控，特别是它正在屏幕上产生大量不需要的输出时，常用此信号终止它。
##### SIGQUIT
当用户在终端上按退出键（一般采用Ctrl+\）时，中断驱动程序产生此信号,并发送给前台进程组中的所有进程。此信号不仅终止前台进程组(如SIGINT所做的那样)，同时产生一个core文件。
##### SIGTERM
这是由kil1(1)命令发送的系统默认终止信号。由于该信号是由应用程序捕获的，使用SIGTERM也让程序有机会在退出之前做好清理工作，从而优雅地终止(相对于SIGKILL而言。SIGKILL不能被捕捉或者忽略)。
##### SIGCHLD
在一个进程终止或停止时，SIGCHLD信号被送给其父进程。按系统默认,将忽略此信号。如果父进程希望被告知其子进程的这种状态改变，则应捕捉此信号。信号捕捉函数中通常要调用一种wait函数以取得子进程D和其终止状态。System V的早期版本有一个名为SIGCLD（无H）的类似信号。这一信号具有与其他信号不同的语义，SVR2的手册页警告在新的程序中尽量不要使用这种信号。(令人奇怪的是，在SVR3和SVR4版的手册页中，该警告消失了。)应用程序应当使用标准的SIGCHLD信号，但应了解，为了向后兼容，很多系统定义了与SIGCHLD等同的SIGCLD。如果有使用SIGCLD 的软件，需要查阅系统手册，了解它具体的语义。eg: sigcld(存疑p275)
##### SIGSEGV
指示进程进行了一次无效的内存引用(通常说明程序有错，比如访问了一个未经初始化的指针)。名字SEGV代表“段违例”( segmentation violation )。
##### SIGUSR1/SIGUSR2
这两个信息是系统预留给用户的，用户可以自定义的信号
##### SIGKILL/SIGSTOP
这两个信号不可以被捕捉和忽略。SIGKILL向系统管理员提供了一种可以杀死任意进程的可靠方法。SIGSTOP是一个作业控制信号，停止一个进程
```
eg: signal01.cpp
```
### 2、signal();
```
    #include <signal.h>

    typedef void (*sighandler_t)(int);

    sighandler_t signal(int signum, sighandler_t handler);
    handler可以使自定义收到信号signum时要执行的动作，也可以是SIG_IGN或者是SIG_DFL

    void (*signal(int signum, void (*func)(int)))(int)

RETURN VALUE
    1、如果成功注册信号处理函数，则返回先前与该信号相关联的信号处理函数的指针。这个指针的类型是 sig_handler。

    2、如果之前没有与该信号相关联的信号处理函数，则返回 SIG_DFL（表示默认的信号处理行为）。

    3、如果发生错误，则返回 SIG_ERR。
```
```
很多捕捉SIGINT/SIGQUIT这两个信号的交互程序都有如下的代码：
    void sig_int(int), sig_quit(int)
    if(signal(SIGINT, SIG_IGN) != SIG_IGN){
        signal(SIGINT, sig_int);
    }
    if(signal(SIGQUIT, SIG_IGN)!=SIG_IGN){
        signal(SIGQUIT, sig_quit);
    }
    弊端：由signal的返回值造成，必须先对信号进行行为注册之后，才能知道和信号之前关联的行为是什么。
```
信号会打断阻塞的系统调用 eg: breakblocksys.cpp
注：标准信号一般不会进行累加，使用sigprocmask屏蔽信号之后，不会对信号进行累加，解除屏蔽之后，只会执行一次信号处理函数（POSIX标准p276）
### 3、信号的（行为）不可靠
信号可能会丢失
### 4、可重入函数
所有的系统调用都是可重入的，一部分库函数也是可重入的
### 5、信号的响应过程
信号从响应到收到有一个不可避免的延迟
标准信号的响应没有严格的顺序
不能从信号响应函数随意往外跳（setjmp, longjmp）
### 6、信号相关的常用函数
#### 6.1、kill();
将信号发送给进程或进程组
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

    eg: signaltime.cpp 
        signalalarm.cpp  比较time和alarm时间的精确度

    早期的UNIX系统实现曾提出警告，这种信号可能比预定值提前1s发送。POSLX.1则不允许这样做。
    每个进程只能有一个闹钟时间。如果在调用alarm时，之前已为该进程注册的闹钟时间还没有超时，则该闹钟时间的余留值作为本次alarm函数调用的值返回。以前注册的闹钟时间则被新值代替。(signal要在alarm之前)
```
```
    #include <sys/time.h>

    int getitimer(int which, struct itimerval *curr_value);
    int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);

    struct itimerval {
        struct timeval it_interval; /* Interval for periodic timer */ 周期执行
        struct timeval it_value;    /* Time until next expiration */ 初始延迟，只执行一次
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
RETURN VALUE
    pause()  returns  only when a signal was caught and the signal-catching function returned.  In this case, pause() returns -1, and errno is  set to EINTR.p279
```
#### 6.5、abort();
给当前进程发送一个SIGABRT的信号，终止当前进程（是异常终止）
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
不建议使用sleep，因为在有些平台sleep底层使用的是alarm+pause封装，当程序本身有使用信号alarm的时候，如果再使用sleep就有可能造成alarm的覆盖，从而导致程序混乱（有些平台使用nanosleep封装，此时就不会产生混乱）
```
    #include <time.h>

    int nanosleep(const struct timespec *req, struct timespec *rem);
    第一个参数，要休眠的时间长度
    第二个参数，未休眠完的剩余时间
    #include <time.h>
    int clock_nanosleep(clockid_t clockid, int flags, const struct timespec *request, struct timespec *remain);
    Link with -lrt (only for glibc versions before 2.17).
    flags参数用于控制延迟是相对的还是绝对的。 flags为0时表示休眠时间是相对的(例如，希望休眠的时间长度)，如果 flags值设置为TIMER_ABSTIME，表示休眠时间是绝对的(例如，希望休眠到时钟到达某个特定的时间)。
```
### 7、信号集
#### 信号集类型：sigset_t
```
    #include <signal.h>

    int sigemptyset(sigset_t *set);

    int sigfillset(sigset_t *set);

    int sigaddset(sigset_t *set, int signum);

    int sigdelset(sigset_t *set, int signum);

    int sigismember(const sigset_t *set, int signum);

    注：所有应用程序在使用信号集前，要对该信号集调用sigemptyset或sigfillset一次。这是因为C编译程序将不赋初值的外部变量和静态变量都初始化为0，而这是否与给定系统上信号集的实现相对应却并不清楚。
```
### 8、信号屏蔽字/pendding集的处理
```
    #include <signal.h>

    /* Prototype for the glibc wrapper function */
    int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);

    eg: signalblock.cpp

    #include <signal.h>

    int sigpending(sigset_t *set); 信号被放入进set的条件是：1、该信号是阻塞，2、向该进程发送了信号（无论是怎么发送过来的）
    进程可以选用“阻塞信号递送”。如果为进程产生了一个阻塞的信号，而且对该信号的动作是系统默认动作或捕捉该信号，则为该进程将此信号保持为未决状态，直到该进程对此信号解除了阻塞，或者将对此信号的动作更改为忽略。内核在递送一个原来被阻塞的信号给进程时(而不是在产生该信号时)，才决定对它的处理方式。于是进程在信号递送给它之前仍可改变对该信号的动作。进程调用sigpending函数来判定哪些信号是设置为阻塞并处于未决状态的。

```
### 9、扩展
#### 9.1、sigsuspend();
```
    #include <signal.h>

    int sigsuspend(const sigset_t *mask);

    eg: sigsuspend.cpp
```
#### 9.2、sigaction();
sigaction函数的功能是检查或修改（或检查并修改）与指定信号相关联的处理动作。此函数取代了UNIX早期版本使用的signal函数。sigaction 函数实现了signal。
sigaction实在处理响应函数内部才去进行屏蔽结构体中带的信号
```
    #include <signal.h>

    int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);

    struct sigaction {
        void     (*sa_handler)(int); // 可防止重入
        void     (*sa_sigaction)(int, siginfo_t *, void *); // 可判断信号来源 需要sa_flags设置为SA_SIGINFO
        sigset_t   sa_mask;
        int        sa_flags;
        void     (*sa_restorer)(void);
    };
    相比于signal注册信号的行为，sigaction可防止signal的重入现象，以及signal无法判断信号的来源（看手册）
    对除SIGALRM 以外的所有信号，我们都有意尝试设置SA_RESTART 标志，于是被这些信号中断的系统调用都能自动重启动。不希望重启动由SIGALRM信号中断的系统调用的原因是:我们希望对IO操作可以设置时间限制。
```
#### 9.3、sigqueue
```
    #include <signal.h>

    int sigqueue(pid_t pid, int sig, const union sigval value);
    union sigval {
        int   sival_int;
        void *sival_ptr;
    };


```
### 10、实时信号
实时信号不丢失，要排队有上限（ulimit -a）,实时信号的默认行为是终止进程
### 11、作业控制信号
```
SIGCHLD子进程已停止或终止。
SIGCONT如果进程已停止，则使其继续运行。
SIGSTOP停止信号(不能被捕捉或忽略)。
SIGTSTP交互式停止信号。
SIGTTIN后台进程组成员读控制终端。
SIGTTOU后台进程组成员写控制终端。
```
### 12、信号名和信号编号
extern char *sys_siglist [];
数组下标是信号编号，数组中的元素是指向信号名符串的指针。
```
    #include <signal.h>

    void psignal(int sig, const char *s);
    void psiginfo(const siginfo_t *pinfo, const char *s);
    都是往标准错误中输出，s是自定义的信息
```
```
    #include <string.h>

    char *strsignal(int sig);
    给出一个信号编号，strsignal将返回描述该信号的字符串。应用程序可用该字符串打印关于接收到信号的出错消息。
```
```
    #include <signal.h>
    int sig2str (int signo,char *str) ;
    int str2sig (const char *str, int * signop) ;
    Solaris提供一对函数，一个函数将信号编号映射为信号名，另一个则反之。p314
```
## 线程
### 1、线程的概念
一个正在运行的函数
posix线程是一套标准，不是具体实现，eg：pthread_t,具体实现不清楚
```
    #include <pthread.h>

    int pthread_equal(pthread_t t1, pthread_t t2); // 比较两个线程标识是否相等
    pthread_t pthread_self(void); // 获取自身的线程标识
    Compile and link with -pthread.
```
### 2、线程的创建
线程的调度取决于调度器的策略
```
    #include <pthread.h>

    int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

    Compile and link with -pthread.
RETURN VALUE
    On  success,  pthread_create() returns 0; on error, it returns an error number, and the contents of *thread are undefined.
```
#### 2.1、线程的终止（三种方式）
##### 2.1.1、线程从启动例程返回，返回值就是线程的退出码
##### 2.1.2、线程可以被同一进程中的其他线程取消
##### 2.1.3、线程调用pthread_exit()函数
```
    #include <pthread.h>

    void pthread_exit(void *retval); // 类似于exit

    Compile and link with -pthread.
```
```
    #include <pthread.h>

    int pthread_join(pthread_t thread, void **retval); // 类似于wait收尸

    Compile and link with -pthread.

```
#### 2.2、线程栈的清理
```
    #include <pthread.h>

    void pthread_cleanup_push(void (*routine)(void *), void *arg);
    void pthread_cleanup_pop(int execute);

    Compile and link with -pthread.
```
#### 2.3、线程的取消选项
### 3、线程同步
### 4、线程相关的属性
#### 4.1、线程同步的属性
### 5、重入
#### 5.1、线程与信号
#### 5.2、线程与fork
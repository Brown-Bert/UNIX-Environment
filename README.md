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
## 11、系统日志
### syslogd服务
### openlog()
### syslog()
### closelog()
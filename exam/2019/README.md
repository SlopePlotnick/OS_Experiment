t1
利用fork pipe实现 cat /etc/passwd | wc -l 命令

t2
10个子进程计算0~100,第一个进程计算0~10，第二计算11~20，以此类推。 并利用管道把结果返回到主进程

t3
2个线程计算1~100，第一个计算start~middle，第二个计算middle~end。

t4
4个子进程，分别为爹妈儿女。爹往盘子里放苹果，儿子消耗苹果。妈往盘子里放橘子，女儿消耗橘子。盘子里只容许放一个水果。
使程序按如下结果运行循环
put apple
get apple
put orange
get orange

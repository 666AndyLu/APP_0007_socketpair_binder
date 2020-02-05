# APP_0007_socketpair_binder

# socketpair.c
gcc -o socketpair socketpair.c -pthread
./socketpair
ps -A得到 5026 pts/1    00:00:00 socketpair
cd /proc/5026
ls task/可以看到里面两个子进程的代号为5026  5027

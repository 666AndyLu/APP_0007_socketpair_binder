#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

/*
 * 参考frameworks\native\libs\input\inputTransport.cpp
 */
static const size_t SOCKET_BUFFER_SIZE = 32 * 1024;

void *function_thread1 (void *arg)
{
	char buf[500];
	int len;
	int cnt = 0;
	int fd = (int)arg;
	while(1)
	{
		/* 向main线程发出：Hello, main thread */
		len = sprintf(buf, "Hello, main thread, cnt = %d",++cnt);
		write(fd, buf, len);
		/* 读取(main thread发回的数据) */
		len = read(fd, buf, 500);
		buf[len] = '\0';
		printf("%s\n",buf);
		sleep(5);
	}
	return NULL;
}
 
int main(int argc, char **argv)
{
	int sockets[2];
	
	socketpair(AF_UNIX, SOCK_SEQPACKET, 0, sockets);
	
	int bufferSize = SOCKET_BUFFER_SIZE;
    setsockopt(sockets[0], SOL_SOCKET, SO_SNDBUF, &bufferSize, sizeof(bufferSize));
    setsockopt(sockets[0], SOL_SOCKET, SO_RCVBUF, &bufferSize, sizeof(bufferSize));
    setsockopt(sockets[1], SOL_SOCKET, SO_SNDBUF, &bufferSize, sizeof(bufferSize));
    setsockopt(sockets[1], SOL_SOCKET, SO_RCVBUF, &bufferSize, sizeof(bufferSize));
	
	pthread_t threadID;
	/* 创建线程1 */
	pthread_create(&threadID, NULL, function_thread1, (void *)sockets[1]);
	
	char buf[500];
	int len;
	int cnt = 0;
	int fd = sockets[0];
	while(1)
	{
		/* 读数据:线程1发出的数据 */
		len = read(fd, buf, 500);
		buf[len] = '\0';
		printf("%s\n",buf);
		/* main thread向thread1发出：Hello, thread1 */
		len = sprintf(buf, "Hello, thread1, cnt = %d",++cnt);
		write(fd, buf, len);
		sleep(5);
	}
}
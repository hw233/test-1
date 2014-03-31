#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <fcntl.h> // for fcntl

#include "Config.h"
#include "Log/Log.h"
#include "Server/Cfg.h"

Log* g_log = new Log(1, "log/JsonProxy/");

int connASSS(int port)
{
    int clientSock = -1;
    sockaddr_in  sa;
    clientSock = socket( AF_INET, SOCK_STREAM, 0 );

    if( clientSock < 0 )
    {
        g_log->OutError("Client Socket create error!\n");
        return -1;
    }
    memset( &sa, 0, sizeof(sa) );
    sa.sin_family = AF_INET;
    sa.sin_port   = htons( port );
    sa.sin_addr.s_addr = inet_addr( "127.0.0.1" );

    if( connect( clientSock, (sockaddr*)&sa, sizeof(sockaddr) ) < 0 )
    {
        return -1;
    }

#if 0
    struct timeval timeout;
    timeout.tv_sec=1;
    timeout.tv_usec=0;
    int res = setsockopt(clientSock, SOL_SOCKET, SO_RCVTIMEO | SO_SNDTIMEO, (char *)&timeout.tv_sec, sizeof(struct timeval));
    if(res == -1)
        g_log->OutError("asss_conn setsockopt error[%d]!\n", errno);
#endif
    int flag = 1;
    setsockopt(clientSock, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
    int size = 64*1024;
    setsockopt(clientSock, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
    setsockopt(clientSock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

    return clientSock;
}

int jason_listen( int port )
{
    int sockfd = -1;
    struct sockaddr_in my_addr;
    //建立TCP套接口
    if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        g_log->OutError("create socket error\n");
        exit(1);
    }
    //初始化结构体，并绑定2323端口
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(my_addr.sin_zero), 0, 8);
    //绑定套接口
    if(bind(sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))==-1)
    {
        g_log->OutError("bind socket error\n");
        exit(1);
    }
    //创建监听套接口
    if(listen(sockfd,10)==-1)
    {
        g_log->OutError("listen error\n");
        exit(1);
    }

    return sockfd;
}

int wait4read(int fd)
{
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN | POLLERR | POLLPRI;
    pfd.revents = 0;

    int ret = TEMP_FAILURE_RETRY(::poll(&pfd, 1, 1800));
    if (ret > 0 && !(pfd.revents & POLLIN))
        return -1;

    return ret;
}

int wait4send(int fd)
{
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLOUT | POLLERR | POLLPRI;
    pfd.revents = 0;

    int ret = TEMP_FAILURE_RETRY(::poll(&pfd, 1, 1800));
    if (ret > 0 && !(pfd.revents & POLLOUT))
        return -1;

    return ret;
}

int msend(int fd, void* buf, int size, int flag)
{
    int off = 0;
    int ret = 0;
    const unsigned char* buffer = reinterpret_cast<const unsigned char*>(buf);

    do
    {
        ret = TEMP_FAILURE_RETRY(::send(fd, buffer+off, size-off, MSG_NOSIGNAL));
        if (!ret)
            return -1;
        if (ret < 0)
        {
            ret = 0;
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                if (wait4send(fd) < 0)
                    return -1;
            }
            else
            {
                return -1;
            }
        }

        if (ret > 0)
            off += ret;
    } while (off < size);

    return off;
}

int mrecv(int fd, void* buf, int size, int flag)
{
    int off = 0;
    int ret = 0;
    unsigned char* buffer = reinterpret_cast<unsigned char*>(buf);

    do
    {
        ret = TEMP_FAILURE_RETRY(::recv(fd, buffer+off, size-off, MSG_NOSIGNAL));
        if (!ret)
            return -1;
        if (ret < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                if (wait4read(fd) < 0)
                    return -1;
            }
            else
            {
                return -1;
            }
        }

        if (ret > 0)
            off += ret;
    } while (ret < 0);

    return off;
}

int read_jason_req(int fd, char* buf)
{
    int read_len1 = recv( fd, buf, 5, 0);
    if(read_len1 != 5)
    {
        if(read_len1 < 0)
            g_log->OutError("socket error[%d]!\n", errno);
        g_log->OutTrace("len error 1, len is %d\n",read_len1);
        return 0;
    }
    unsigned short len = *(unsigned short*)buf;
    if(len <= 0)
    {
        g_log->OutTrace("len error 2\n");
        return 0;
    }
    if (len >= 16*1024-6)
    {
        g_log->OutTrace("len error 3\n");
        return 0;
    }
    int read_len2 = recv( fd, buf+5, len, 0 );
    if(read_len2 != len)
    {
        g_log->OutTrace("len error 4\n");
        return 0;
    }

    return read_len1 + read_len2;
}

int read_jason_rep(int fd, char* buf)
{
    int read_len1 = recv( fd, buf, 4, 0);
    if(read_len1 != 4)
        return 0;
    unsigned short len = *(unsigned short*)buf;
    if(len <= 0)
        return 0;
    int read_len2 = recv( fd, buf+4, len, 0 );
    if(read_len2 != len)
        return 0;

    return read_len1 + read_len2;

}

void clear_read_buffer(int fd)
{
    for(;;)
    {
        struct timeval stWait;
        fd_set stFdSet;
        int IRet;
        char tmp[2];

        stWait.tv_sec = 0;
        stWait.tv_usec = 1000;
        FD_ZERO(&stFdSet);
        FD_SET(fd, &stFdSet);
        IRet = select(FD_SETSIZE, &stFdSet, NULL, NULL, &stWait);
        if(IRet <= 0)
            break;
        int len = recv(fd, tmp, 1, 0);
        if(len <= 0)
            break;
    }
}

#define CLOSE(fb) do{close(fb);fb=-1;}while(0)

void setNonblock(int s)
{
    int flag = fcntl(s, F_GETFL, 0);
    if (flag >= 0)
    {
        flag |= O_NONBLOCK;
        ::fcntl(s, F_SETFL, flag);
    }
}

int main()
{
    cfg.load();

    int asss_port = cfg.tcpPort;
    int jason_port = cfg.tcpPort - 1000;

    int asss_conn = -1;
    int jason_sock = jason_listen(jason_port);

    socklen_t sin_size = sizeof(struct sockaddr_in);
    struct sockaddr_in their_addr;
    bool flag = false;
    //等待连接
    while(1)
    {
        if(asss_conn == -1)
            asss_conn = connASSS(asss_port);

        if(asss_conn == -1)
        {
            if(flag)
                g_log->OutTrace("failed connect to asss server.\n");
            flag = false;
            sleep(1);
            continue;
        }
        else
        {
            if(!flag)
                g_log->OutTrace("connect to asss server.\n");
            flag = true;
        }

        //如果建立连接，将产生一个全新的套接字
        int new_fd = -1;
        if((new_fd = accept(jason_sock,(struct sockaddr *)&their_addr,&sin_size))==-1)
        {
            g_log->OutError("accept error\n");
            exit(1);
        }
        else
        {
#if 0
            struct timeval timeout;
            timeout.tv_sec=3;
            timeout.tv_usec=0;
            int res = setsockopt(new_fd, SOL_SOCKET, SO_RCVTIMEO | SO_SNDTIMEO, (char *)&timeout.tv_sec, sizeof(struct timeval));
            if(res == -1)
	        g_log->OutError("new_fd setsockopt error[%d]!\n", errno);

#endif
            int flag = 1;
            setsockopt(new_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
            //setNonblock(new_fd);

            g_log->OutTrace("accept new connection %s\n", inet_ntoa(their_addr.sin_addr));
        }

        do {
            // 清空接收缓存
            //clear_read_buffer(asss_conn);
            char buf[16*1024] = {0}; // XXX: 16K
            int len = 0;
            if((len = read_jason_req(new_fd, buf)) == 0)
            {
                g_log->OutTrace("read_jason_req. connection close %s\n", inet_ntoa(their_addr.sin_addr));
                CLOSE(new_fd);
                break;
            }
            g_log->OutTrace("RECV: %s\n", buf);

            int sd = 0;
            if( (sd = send( asss_conn, buf, len, 0 )) < 0)
            {
                CLOSE(new_fd);
                CLOSE(asss_conn);
                g_log->OutError("write_jason_req failed.\n");
                break;
            }

            if((len = read_jason_rep(asss_conn, buf)) == 0)
            {
                CLOSE(new_fd);
                CLOSE(asss_conn);
                g_log->OutError("read_jason_rep failed.\n");
                break;
            }

            int cnt = send( new_fd, buf, len, 0 );
            if( cnt <= 0 )
            {
                CLOSE(new_fd);
                g_log->OutError("write_jason_rep connection close %s\n", inet_ntoa(their_addr.sin_addr));
                break;
            }
            g_log->OutTrace("[len recved: %d][len send: %d] SEND: %s\n", len, cnt, buf);
        }
        while(0);

        if(new_fd > 0)
            CLOSE(new_fd);
    }

    if (asss_conn > 0)
        CLOSE(asss_conn);

    return 0;
}

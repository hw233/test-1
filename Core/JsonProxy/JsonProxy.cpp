#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

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
        g_log->OutError("Client Socket create error!");
        return -1;
    }
    memset( &sa, 0, sizeof(sa) );
    sa.sin_family = AF_INET;
    sa.sin_port   = htons( port );
    sa.sin_addr.s_addr = inet_addr( "127.0.0.1" );

    if( connect( clientSock, (sockaddr*)&sa, sizeof(sockaddr) ) < 0 )
    {
        g_log->OutError("Connect Error!\n");
        return -1;
    }

    return clientSock;
}

int jason_listen( int port )
{
    int sockfd = -1;
    struct sockaddr_in my_addr;
    //建立TCP套接口
    if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        g_log->OutError("create socket error");
        exit(1);
    }
    //初始化结构体，并绑定2323端口
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(my_addr.sin_zero),8);
    //绑定套接口
    if(bind(sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))==-1)
    {
        g_log->OutError("bind socket error");
        exit(1);
    }
    //创建监听套接口
    if(listen(sockfd,10)==-1)
    {
        g_log->OutError("listen error");
        exit(1);
    }

    return sockfd;
}

int read_jason_req(int fd, char* buf)
{
    int read_len1 = read( fd, buf, 5);
    if(read_len1 != 5)
        return 0;
    int len = *(short*)buf;
    int read_len2 = read( fd, buf+5, len );
    if(read_len2 != len)
        return 0;

    return read_len1 + read_len2;
}

int read_jason_rep(int fd, char* buf)
{
    int read_len1 = read( fd, buf, 4);
    if(read_len1 != 4)
        return 0;
    int len = *(short*)buf;
    int read_len2 = read( fd, buf+4, len );
    if(read_len2 != len)
        return 0;

    return read_len1 + read_len2;

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
    //等待连接
    while(1)
    {
        if(asss_conn == -1)
            asss_conn = connASSS(asss_port);

        if(asss_conn == -1)
        {
            sleep(1);
            continue;
        }
        else
            g_log->OutTrace("server is run.\n");

        //如果建立连接，将产生一个全新的套接字
        int new_fd = -1;
        if((new_fd = accept(jason_sock,(struct sockaddr *)&their_addr,&sin_size))==-1)
        {
            g_log->OutError("accept error");
            exit(1);
        }
        else
            g_log->OutTrace("accept new connection %s", inet_ntoa(their_addr.sin_addr));


        while(1)
        {
            char buf[4096] = {0};
            int len = 0;
            if((len = read_jason_req(new_fd, buf)) == 0)
            {
                g_log->OutTrace("read_jason_req. connection close %s", inet_ntoa(their_addr.sin_addr));
                close(new_fd);
                break;
            }

            if( -1 == write( asss_conn, buf, len ))
            {
                close(new_fd);
                close( asss_conn );
                asss_conn = -1;
                g_log->OutError("write_jason_req failed.");
                break;
            }

            if((len = read_jason_rep(asss_conn, buf)) == 0)
            {
                close(new_fd);
                close( asss_conn );
                asss_conn = -1;
                g_log->OutError("read_jason_rep failed.");
                break;
            }

            if( -1 == write( new_fd, buf, len ))
            {
                close(new_fd);
                g_log->OutError("write_jason_rep connection close %s", inet_ntoa(their_addr.sin_addr));
                break;
            }
        }
    }

    close( asss_conn );

    return 0;
}

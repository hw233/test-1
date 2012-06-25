#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


inline long int GetTick()
{
    struct timeval tv;
    if(gettimeofday(&tv, NULL))
        return 0;
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main()
{
    int clientSock, nRecvBytes = -1;
    sockaddr_in  sa;
    char jason[] = "123456789123456789123456789123456789{\"head\": {\"uiPacketLen\":100,\"uiCmdid\":\"3\",\"uiSeqid\":1,\"szServiceName\":\"IDIP\",\"uiSendTime\": 20110820,\"uiVersion\":1001,\"ucAuthenticate\":\"\",\"iResult\":0,\"szRetErrMsg\":\"\"},\"body\":{\"szOpenId\":\"100001\",\"uiAreaId\":1,\"playerId\":1111}}";
    unsigned char buffer[1024] = {0, 0, 0xCC, 1, 0x1D};
    *(short*)(buffer) = strlen(jason) + 2;
    *(short*)(buffer + 5) = strlen(jason);
    memcpy(buffer + 7, jason, strlen(jason));

    int errors = 0;
    long int t1 = GetTick();
    for(int i = 0; i < 10000; ++ i)
    {
        clientSock = socket( AF_INET, SOCK_STREAM, 0 );

        if( clientSock < 0 )
        {
            printf("Client Socket create error!");
            return 0;
        }
        memset( &sa, 0, sizeof(sa) );
        sa.sin_family = AF_INET;
        sa.sin_port   = htons( 7888 );
        sa.sin_addr.s_addr = inet_addr( "192.168.1.102" );

        if( connect( clientSock, (sockaddr*)&sa, sizeof(sockaddr) ) < 0 )
        {
            printf("Connect Error!(%d)(%d)\n", errno, clientSock);
            return 0;
        }

	errno = 0;
        long int t1 = GetTick();
        write( clientSock, buffer, strlen(jason)+7 );
        long int ta1 = GetTick();

        char buffer2[1024] = {0};
        memset( buffer2, 0, sizeof(buffer2) );
        unsigned char buf[4] = {0};
        memset( buf, 0, sizeof(buf) );
        size_t nread1 = read( clientSock, buf, 4);
        short len = *(short *)(buf);
        long int tb2 = GetTick();
        size_t nread2 = read( clientSock, buffer2, len);
        long int t2 = GetTick();
        int t = t2 - t1;
        int ta = ta1 - t1;
        int tb = t2 - tb2;
        int tab = tb2 - ta1;
	if (errno)
		++errors;
        //printf("t1=%lld, t2=%lld, t=%d, ta=%d, tab=%d, tb=%d,  %s\n", t1, t2, t, ta, tab, tb, buffer+4);
	close(clientSock);
    }
    long int t2 = GetTick();

    int times = t2 - t1;

    printf("time1:%lld\n", t1);
    printf("time2:%lld\n", t2);
    printf("time:%lld.%llds\n", times/1000000, (times%1000000)/1000);
    printf("errors: %d\n", errors);

    return 0;
}

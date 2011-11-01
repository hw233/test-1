#ifndef _H_DCAPI_H_
#define _H_DCAPI_H_

#include <string>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/un.h>

using namespace std;

namespace DataCollector
{
	/*
		常用的上报类型定义，需要传递给write_log 和 write_baselog 函数
		这里建议除了模调、基础、监控、自动化测试这些格式固定的数据上报外，
		其余数据上报(CSEC安全分析，CBA经营数据分析)都使用 LT_NORAMAL.
                    
	*/
	typedef char logtype ;
	const logtype LT_NORMAL   = 0;	// 普通数据(CSEC安全分析，CBA经营数据分析)
	const logtype LT_MOD	  = 1;	// 模块间调用数据
	const logtype LT_BASE	  = 2;	// 基础数据
	const logtype LT_MONITOR  = 3;	// 监控数据
	const logtype LT_AUTOTEST = 4;	// 自动化测试数据
	
	class CLogger
	{
		public:

		CLogger();
		~CLogger();


		
		/******************************************
			名称：init
			参数：appname  app名，用于调用名字服务
				
			返回：0 成功 1 失败
			说明：初始化日志上报，只能调用一次
		*******************************************/
		int init(string& appname);

		


		/******************************************
			名称：encode
			参数：src_data 原字符串 
			     dst_data 编码后字符串
			返回：0 成功 !=0 失败
			说明：简单的编码函数，对字符”&”,”=”,”%”进行替换
		*******************************************/
		int encode(string& src_data, string& dst_data);

		
		/*******************************************
			名称：write_log
			返回：0 成功 != 0失败
			参数：seq		数据记录序列号，第一次上报传入0,传出分配的序列号
			     logtype	数据上报的类型
			     data		编码后的数据，业务需要直接传递格式为 "k1=v1&k2=v2"的数据(v2已经做过编码)
			说明：数据上报函数，一般用于落地数据的上报，允许多次添加，最终要调用commit函数完成上报。
                          注意需要先调用get_seq函数得到sequence num，再多次调用write_log函数填入上一步取得的sequence num进行多次上报，最后调用commit函数提交该sequence num完成上报。
                          本接口不适用于上报模调数据。
		********************************************/
		int write_log(unsigned long seq, logtype type, string& data);

		
		/*******************************************
			名称：write_baselog
			返回：0 成功 != 0 失败
			参数：logtype	数据上报的类型
			      data		编码后的数据，业务需要直接传递格式为 "k1=v1&k2=v2"的数据(v2已经做过编码)
			      fallFlag    是否落地，true 落地,false 不落地
			说明：数据上报函数，一般用于非落地数据的上报，不需要调用commit，也不分配序列号
                          注意 Logtype为1，即上报模调数据时，只能使用write_baselog接口一次性上报日志数据，且设置为不需要数据落地。
		********************************************/
		int write_baselog(logtype type, string& data, bool fallFlag);
		
		
		/*******************************************
			名称：commit
			返回：0 成功 != 0 失败
			参数：seq 要提交的序列号
				           fallFlag    是否落地，true 落地,false 不落地
			说明：在write_log多次调用后，一次性提交完成上报
		********************************************/
		int commit(unsigned long seq, bool fallFlag);

		
		/*******************************************
			名称：get_time
			返回：0 成功 != 0 失败
			参数：now 当前时间
			说明：获取时间函数，使用了优化的gettimeofday
		********************************************/
		int get_time(timeval &now);


		/*******************************************
			名称：get_time_consume
			返回：结束时间 - 开始时间 的 毫秒数
			参数：time_begin 起始时间
				  time_end   结束时间
			说明：计算开始和结束时间的耗时(ms)
		********************************************/
		unsigned long get_time_consume(const timeval& time_begin , const timeval& time_end);

	
		/*******************************************
			名称：get_errmsg
			返回：错误信息
			参数：空				  
			说明：返回调用失败时的错误信息
		********************************************/	
		string get_errmsg(void);


		/*******************************************
			名称：get_seq
			返回：0 成功 != 0 失败
			参数：seq
			说明：获取sequence number
		********************************************/	
		int get_seq(unsigned long& seq);

		private:

			pthread_rwlock_t  errMsgrwlock;
			string m_appname ;	//appname
			string m_errmsg ;	//last errmsg
			int rspMsgLen;
			int reqMsgHdrLen;

			struct sockaddr_un serverAddr_un;
			struct sockaddr_in serverAddr_in;

			int addrLen;
			int maxBuffLen;
			
			bool isInit;

			//上报日志的固定前缀(为_appname=xxx)
			string logPreffix;
			int logPreffixLen;

			string logPreffixLite;
			int logPreffixLenLite;

			//是否已经连接
			bool isConnected;
			bool isGetConnected;
			
			int sockfd;
			int get_seq_sockfd;

			//在agent 连接不上时重试连接的计数
			unsigned tryConTimes;
			unsigned slideInterval;
			pthread_rwlock_t  tryConTimesWrlock;

			pthread_rwlock_t  conSeqLock;
			pthread_rwlock_t  conLock;

			


		private:


			//unix socket 连接返回成功后的fd, 0错误
			int sync_unix_conn();

			int sync_get_seq_unix_conn();
			
			int set_non_block(int fd);

			void update_errmsg(const char* cErrmsg, ...);

			int tcp_write (int fd, char *buff, int length, int sockType);
			int tcp_read (int fd, char *buff, int length);

			
			ssize_t readn(int fd, char*  buf, size_t len);
			//同步消息，发送并等待接收回应消息(可选)
			int sync_send(char *buff, int length, char* recvBuff = NULL,bool readflag = false);

			void dis_connect(int fd);
                        void seq_dis_connect(int fd);
			void write_local_file(const char* data, const char* fileName);

			//当连接成功后reset 重试连接的计数
			void reset_try_times();


			int inner_encode(string& src_data, string& dst_data);

			int comm_write(logtype type, string& data, bool fallFlag, bool isBase = true, unsigned long seq = 0);

			string&    replace_all(string&    str,const    string&    old_value,const    string&    new_value);

	};

};

#endif


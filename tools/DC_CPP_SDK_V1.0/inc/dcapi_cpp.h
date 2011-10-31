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
		���õ��ϱ����Ͷ��壬��Ҫ���ݸ�write_log �� write_baselog ����
		���ｨ�����ģ������������ء��Զ���������Щ��ʽ�̶��������ϱ��⣬
		���������ϱ�(CSEC��ȫ������CBA��Ӫ���ݷ���)��ʹ�� LT_NORAMAL.
                    
	*/
	typedef char logtype ;
	const logtype LT_NORMAL   = 0;	// ��ͨ����(CSEC��ȫ������CBA��Ӫ���ݷ���)
	const logtype LT_MOD	  = 1;	// ģ����������
	const logtype LT_BASE	  = 2;	// ��������
	const logtype LT_MONITOR  = 3;	// �������
	const logtype LT_AUTOTEST = 4;	// �Զ�����������
	
	class CLogger
	{
		public:

		CLogger();
		~CLogger();


		
		/******************************************
			���ƣ�init
			������appname  app�������ڵ������ַ���
				
			���أ�0 �ɹ� 1 ʧ��
			˵������ʼ����־�ϱ���ֻ�ܵ���һ��
		*******************************************/
		int init(string& appname);

		


		/******************************************
			���ƣ�encode
			������src_data ԭ�ַ��� 
			     dst_data ������ַ���
			���أ�0 �ɹ� !=0 ʧ��
			˵�����򵥵ı��뺯�������ַ���&��,��=��,��%�������滻
		*******************************************/
		int encode(string& src_data, string& dst_data);

		
		/*******************************************
			���ƣ�write_log
			���أ�0 �ɹ� != 0ʧ��
			������seq		���ݼ�¼���кţ���һ���ϱ�����0,������������к�
			     logtype	�����ϱ�������
			     data		���������ݣ�ҵ����Ҫֱ�Ӵ��ݸ�ʽΪ "k1=v1&k2=v2"������(v2�Ѿ���������)
			˵���������ϱ�������һ������������ݵ��ϱ�����������ӣ�����Ҫ����commit��������ϱ���
                          ע����Ҫ�ȵ���get_seq�����õ�sequence num���ٶ�ε���write_log����������һ��ȡ�õ�sequence num���ж���ϱ���������commit�����ύ��sequence num����ϱ���
                          ���ӿڲ��������ϱ�ģ�����ݡ�
		********************************************/
		int write_log(unsigned long seq, logtype type, string& data);

		
		/*******************************************
			���ƣ�write_baselog
			���أ�0 �ɹ� != 0 ʧ��
			������logtype	�����ϱ�������
			      data		���������ݣ�ҵ����Ҫֱ�Ӵ��ݸ�ʽΪ "k1=v1&k2=v2"������(v2�Ѿ���������)
			      fallFlag    �Ƿ���أ�true ���,false �����
			˵���������ϱ�������һ�����ڷ�������ݵ��ϱ�������Ҫ����commit��Ҳ���������к�
                          ע�� LogtypeΪ1�����ϱ�ģ������ʱ��ֻ��ʹ��write_baselog�ӿ�һ�����ϱ���־���ݣ�������Ϊ����Ҫ������ء�
		********************************************/
		int write_baselog(logtype type, string& data, bool fallFlag);
		
		
		/*******************************************
			���ƣ�commit
			���أ�0 �ɹ� != 0 ʧ��
			������seq Ҫ�ύ�����к�
				           fallFlag    �Ƿ���أ�true ���,false �����
			˵������write_log��ε��ú�һ�����ύ����ϱ�
		********************************************/
		int commit(unsigned long seq, bool fallFlag);

		
		/*******************************************
			���ƣ�get_time
			���أ�0 �ɹ� != 0 ʧ��
			������now ��ǰʱ��
			˵������ȡʱ�亯����ʹ�����Ż���gettimeofday
		********************************************/
		int get_time(timeval &now);


		/*******************************************
			���ƣ�get_time_consume
			���أ�����ʱ�� - ��ʼʱ�� �� ������
			������time_begin ��ʼʱ��
				  time_end   ����ʱ��
			˵�������㿪ʼ�ͽ���ʱ��ĺ�ʱ(ms)
		********************************************/
		unsigned long get_time_consume(const timeval& time_begin , const timeval& time_end);

	
		/*******************************************
			���ƣ�get_errmsg
			���أ�������Ϣ
			��������				  
			˵�������ص���ʧ��ʱ�Ĵ�����Ϣ
		********************************************/	
		string get_errmsg(void);


		/*******************************************
			���ƣ�get_seq
			���أ�0 �ɹ� != 0 ʧ��
			������seq
			˵������ȡsequence number
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

			//�ϱ���־�Ĺ̶�ǰ׺(Ϊ_appname=xxx)
			string logPreffix;
			int logPreffixLen;

			string logPreffixLite;
			int logPreffixLenLite;

			//�Ƿ��Ѿ�����
			bool isConnected;
			bool isGetConnected;
			
			int sockfd;
			int get_seq_sockfd;

			//��agent ���Ӳ���ʱ�������ӵļ���
			unsigned tryConTimes;
			unsigned slideInterval;
			pthread_rwlock_t  tryConTimesWrlock;

			pthread_rwlock_t  conSeqLock;
			pthread_rwlock_t  conLock;

			


		private:


			//unix socket ���ӷ��سɹ����fd, 0����
			int sync_unix_conn();

			int sync_get_seq_unix_conn();
			
			int set_non_block(int fd);

			void update_errmsg(const char* cErrmsg, ...);

			int tcp_write (int fd, char *buff, int length, int sockType);
			int tcp_read (int fd, char *buff, int length);

			
			ssize_t readn(int fd, char*  buf, size_t len);
			//ͬ����Ϣ�����Ͳ��ȴ����ջ�Ӧ��Ϣ(��ѡ)
			int sync_send(char *buff, int length, char* recvBuff = NULL,bool readflag = false);

			void dis_connect(int fd);
                        void seq_dis_connect(int fd);
			void write_local_file(const char* data, const char* fileName);

			//�����ӳɹ���reset �������ӵļ���
			void reset_try_times();


			int inner_encode(string& src_data, string& dst_data);

			int comm_write(logtype type, string& data, bool fallFlag, bool isBase = true, unsigned long seq = 0);

			string&    replace_all(string&    str,const    string&    old_value,const    string&    new_value);

	};

};

#endif


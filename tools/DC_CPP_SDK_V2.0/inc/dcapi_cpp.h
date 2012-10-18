#ifndef _H_DCAPI_H_
#define _H_DCAPI_H_

#include <string>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/un.h>

using namespace std;

class Sampling;
namespace DataCollector
{
	class SocketClient;

	typedef char logtype ;
	const logtype LT_NORMAL   = 0;   // ���������ϱ�
	const logtype LT_MOD      = 1;   // ģ�������ϱ�
	const logtype LT_BASE	  = 2;	 // ��������
	const logtype LT_MONITOR  = 3;	 // �������
	const logtype LT_SECDATA  = 4;   // ��ȫ�����ϱ�

	class CLogger
	{
		public:
		enum ProType
		{
			PRO_STRING = 0,
			PRO_BINARY = 1,
		};


		/******************************************
			���ƣ�constructor
			������sockettype, 0: unix domain socket, other :tcp protocol
					    needrsp, �����ӽ�����true�������ӽ�����false
			���أ���
			˵����ͨ�����ֱ��ʹ��Ĭ�ϲ���
		*******************************************/
		CLogger(char socketType = 0, bool needRsp = false);
		~CLogger();


		
		/******************************************
			���ƣ�init
			������logname  dc��������ҵ��ı�ʶ
							ҵ�����ݱ���ȥ���ҳ������
					   isModule  �Ƿ�Ϊģ������
					               ��false
			���أ�0 �ɹ� !0 ʧ��
			˵������ʼ��ҵ�������ϱ���ֻ�ܵ���һ��
		*******************************************/
		int init(string& logName, bool isModule = false);


                /******************************************
                        ���ƣ�init
                        ������mid
                                          ��������ID(235000036)
                        ���أ�0 �ɹ� !0 ʧ��
                        ˵������ʼ��ģ�������ϱ���ֻ�ܵ���һ��
                *******************************************/
                int init(int mid);

		


		/******************************************
			���ƣ�encode
			������src_data ԭ�ַ��� 
				  dst_data ������ַ���
			���أ�0 �ɹ� !=0 ʧ��
			˵�����򵥵ı��뺯��
		*******************************************/
		int encode(string& src_data, string& dst_data);




		/*******************************************
			���ƣ�write_baselog
			���أ�0 �ɹ� != 0 ʧ��
			������logtype	�����ϱ�������
				          data		���������ݣ�ҵ����Ҫֱ�Ӵ��ݸ�ʽΪ "k1=v1&k2=v2"������(v2�Ѿ���������)
				          len             ���ݵĳ���
				          proType     0:�ַ���
				                            1:������
			      timestamp   unixʱ�����Ĭ��ֵΪϵͳ��ǰʱ��
			˵���������ϱ�������һ�����ڷ�������ݵ��ϱ�������Ҫ����commit��Ҳ���������к�
		********************************************/
		int write_baselog(const char* data, unsigned int len, char logType, char proType, unsigned int timestamp = time(NULL));



		/*******************************************
			  ���ƣ�write_baselog   ����1.0
			  ���أ�0 �ɹ� != 0 ʧ��
			  ������logtype	�����ϱ�������
			  data		���������ݣ�ҵ����Ҫֱ�Ӵ��ݸ�ʽΪ "k1=v1&k2=v2"������(v2�Ѿ���������)
			  fallFlag    �ò����Ѳ��ܿ����Ƿ���أ��ɺ�����������
			  timestamp   unixʱ�����Ĭ��ֵΪϵͳ��ǰʱ��
			  ˵���������ϱ�������һ�����ڷ�������ݵ��ϱ�
			 ********************************************/
		int write_baselog(logtype type, string& data, bool fallFlag = true, unsigned int timestamp = time(NULL));
		
		/*******************************************
			���ƣ�write_modulelog
			���أ�0 �ɹ� != 0 ʧ��
			������		  sid               ����ID
				          ifid              �ӿ�ID
					  mip		    ����IP
				          sip               ����IP
				          retval            ����ֵ
					  result	    ���ý�����
				          reservedInt0      �����ֶΣ�����
					  reservedInt1	    �����ֶΣ�����
				          reservedStr0      �����ֶΣ��ַ���
				          reservedStr1      �����ֶΣ��ַ���
			˵����ģ�������ϱ�����
		********************************************/
		int write_modulelog(int sid, int ifid, string& mip, string& sip, int retval, int result, int delay, int reservedInt0, int reservedInt1, string& reservedStr0, string& reservedStr1);


		
	
		/*******************************************
			���ƣ�get_errmsg
			���أ�������Ϣ
			��������				  
			˵�������ص���ʧ��ʱ�Ĵ�����Ϣ
		********************************************/	
		string get_errmsg(void);


		private:

			SocketClient *pSocketClient;
			Sampling *pSampling;
			
			pthread_rwlock_t  errMsgrwlock;
			
			string appname;	
			string errmsg;	//last errmsg
			int rspMsgLen;
			int reqMsgHdrLen;


			int addrLen;
			bool needRsp;
			int socketType;

			int m_iMid;
			
			bool isInit;

			unsigned short appnameLen;
			unsigned short appnamePartLen;

		private:

			void update_errmsg(const char* cErrmsg, ...);

			void write_bin_file(const char* data, unsigned int len, const char* fileName);



			int inner_encode(string& src_data, string& dst_data);

			int comm_write(const char* data, unsigned short len, char logType = -1, char proType = -1, unsigned int tm = time(NULL));
			
			
			void encode_str(char* &p, string& str);


			string&    replace_all(string&    str,const    string&    old_value,const    string&    new_value);
	};

};

#endif


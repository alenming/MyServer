#ifndef __LOGIN_SERVICE_H__
#define __LOGIN_SERVICE_H__

#include "KxCommInterfaces.h"

class LoginService
{
public:

	static void processServiceC2S(int subcmd, int uid, char *buffer, int len, IKxComm *commun);
	static void processServiceS2S(int subcmd, int uid, char *buffer, int len, IKxComm *commun);

	static void CMD_C2S_LOGIN(int uid, char *buffer, int len, IKxComm *commun);
	static void CMD_S2C_LOGIN(int uid);

	static void CMD_C2S_REGISTER(int uid, char *buffer, int len, IKxComm *commun);
	static void CMD_S2C_REGISTER(int uid);
	
	static void SERVER_SUB_OFFLINE(int uid, char *buffer, int len, IKxComm *commun);

	//ǰ��ս��������С������
	static void processUserReconect(int uid, char *buffer, int len, IKxComm *commun);
	//�����û���¼���ύ��־��Ϣ
	static void processUserLogService(int uid, char *buffer, int len, IKxComm *commun);
	//�����û���¼���ύ������־��Ϣ
	static void processGuideLogService(int uid, char *buffer, int len, IKxComm *commun);
	//�����û�����������ʱ���ˢ����������
	static void processLoginFreshService(int uid, char *buffer, int len, IKxComm *commun);
};

#endif //__LOGIN_SERVICE_H__
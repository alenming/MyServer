#include "SessionClient.h"
#include "NetworkManager.h"
#include "SessionServer.h"
#include "NetworkManager.h"

using namespace std;

#define HEARTBEATTIME                   (24*60*60)       //心跳时间 s

SessionClient::SessionClient()
: m_Permission(0)
, m_GuestId(0)
, m_UserId(0)
{
    // 设置客户端心跳超时
    m_TimerCallBack = new KxTimerCallback<SessionClient>();
    setTimer();
}

SessionClient::~SessionClient(void)
{
    m_TimerCallBack->stop();
    KXSAFE_RELEASE(m_TimerCallBack);
}

bool SessionClient::setConServer(int nKey, int nValue)
{
	if (nKey == 0)
	{
		return false;
	}
	m_MapConKeyValue[nKey] = nValue;
	return true;
}

bool SessionClient::sendDataToServer(int mainCmd, int subCmd, char *pszContext, int nLen)
{
    KxTCPConnector* pTcpConnector = NULL;
	pTcpConnector = static_cast<KxTCPConnector*>(NetWorkManager::getInstance()->getServer(mainCmd));
    if (pTcpConnector == NULL)
    {
        return false;
    }
    return pTcpConnector->sendData(pszContext, nLen) >= 0;
}

bool SessionClient::sendDataToGroupServer(int nGroupID, char *pszContext, int nLen)
{
    vector<IKxComm*>* pVectConnector = NetWorkManager::getInstance()->getGroupServer(nGroupID);
	if (pVectConnector == NULL)
	{
		return false;
	}

    for (vector<IKxComm*>::iterator ator = pVectConnector->begin();
        ator != pVectConnector->end(); ++ator)
    {
        (*ator)->sendData(pszContext, nLen);
    }
    return true;
}

bool SessionClient::sendDataToAllServer(char *pszContext, int nLen)
{
    map<int, IKxComm*>& allServer = NetWorkManager::getInstance()->getAllServer();
    for (map<int,IKxComm* >::iterator ator = allServer.begin(); ator != allServer.end(); ++ator)
    {
        // ps. 如果send失败触发onError，在onError中从NetWorkManager中移除，会导致崩溃
		ator->second->sendData(pszContext, nLen);
    }
    return true;
}

int SessionClient::getRouteValue(int nKey)
{
	map<int, int>::iterator ator = m_MapConKeyValue.find(nKey);
	if (ator == m_MapConKeyValue.end())
	{
		return 0;
	}
	return ator->second;
}

int SessionClient::onRecv()
{
    m_TimerCallBack->stop(); 
    SessionServer::getInstance()->getTimerManager()->addTimer( m_TimerCallBack, HEARTBEATTIME, 0);
    return KxTCPClienter::onRecv();
}

void SessionClient::setTimer()
{
	m_TimerCallBack->setCallback(this, &SessionClient::onTimer);
	SessionServer::getInstance()->getTimerManager()->addTimer(m_TimerCallBack, HEARTBEATTIME, 0);
	KX_LOGDEBUG("CSessionClient::setTimer()");
}

void SessionClient::onTimer()
{
    // 时间到了
    clean();
	KX_LOGDEBUG("CSessionClient::onTimer()");
}

void SessionClient::clean()
{
	if (m_Permission == 0)
	{
		NetWorkManager::getInstance()->removeGuest(m_GuestId);
	}
	else
	{
		NetWorkManager::getInstance()->removeUser(m_UserId);
	}

    m_TimerCallBack->stop();
    m_TimerCallBack->clean();
    if (m_Poller != NULL)
    {
        m_Poller->removeCommObject(this);
    }
}

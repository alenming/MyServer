#include <iostream>
#include <string>
#include <ctime> 
#include "TestClient.h"
#include "GameNetworkNode.h"
#include "KXServer.h"


using namespace std;


int main(int argc, char ** argv) 
{
	
	TestClient::getInstance()->onServerInit();
	auto poll = CGameNetworkNode::getInstance()->getPoller();
	poll->poll();

	TestClient::getInstance()->login();

	while (true)
	{
		poll->poll();
	}

	char temp = ' ';
	cout << "please loginout" << endl;
	scanf("%c", &temp);
    TestClient::destroy();

	LogManager::destroy();


	while (true)
	{

	}
    return 0;
}

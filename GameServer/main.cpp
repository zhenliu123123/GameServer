#define _CRT_SECURE_NO_WARNINGS 1
#include"GameChannel.h"
#include"GameMsg.h"
#include"msg.pb.h"
#include<cstdio>

int main()
{
	ZinxKernel::ZinxKernelInit();
	
	//testMsg
	pb::SyncPid* pTestMsg = new pb::SyncPid();
	pTestMsg->set_pid(1);
	pTestMsg->set_username("testName");
	GameMsg gmTest(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pTestMsg);
	auto testOutput = gmTest.serialize();
	for (auto byte : testOutput)
	{
		printf("%02X", byte);
	}

	//添加监听通道类
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8899,new GameConnFact())));
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}
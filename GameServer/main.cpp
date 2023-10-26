#define _CRT_SECURE_NO_WARNINGS 1
#include"GameChannel.h"
#include"GameMsg.h"
#include"msg.pb.h"
#include<cstdio>

int main()
{
	ZinxKernel::ZinxKernelInit();
	//添加监听通道类
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8899,new GameConnFact())));
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	////调试信息-testMsg未解析的二进制信息
	//pb::SyncPid* pTestMsg = new pb::SyncPid();
	//pTestMsg->set_pid(1);
	//pTestMsg->set_username("test");
	//GameMsg gmTest(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pTestMsg);
	//auto testOutput = gmTest.serialize();
	//for (auto byte : testOutput)
	//{
	//	printf("%02X ", byte);
	//}
	//puts(" ");
	////调试信息-testMsg解析之后的信息
	//char binaryInfo[] = { 0x08,0x01,0x12,0x08,0x74,0x65,0x73,0x74,0x4E,0x61,0x6D,0x65 };
	//std::string strInfo(binaryInfo, sizeof(binaryInfo));
	//GameMsg gmTestInfo(GameMsg::MSG_TYPE_LOGIN_ID_NAME, strInfo);
	//std::cout << dynamic_cast<pb::SyncPid*>(gmTestInfo.pMsg)->pid() << std::endl;
	//std::cout << dynamic_cast<pb::SyncPid*>(gmTestInfo.pMsg)->username() << std::endl;
	return 0;
}
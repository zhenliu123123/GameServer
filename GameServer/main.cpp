#define _CRT_SECURE_NO_WARNINGS 1
#include"GameChannel.h"
#include"GameMsg.h"
#include"msg.pb.h"
#include<cstdio>
#include"AOIWorld.h"
#include"ZinxTimer.h"
#include"RandomName.h"
////测试玩家类，继承玩家类
//class testPlayer
//	:public Player
//{
//public:
//	testPlayer(int _x, int _y) :x(_x), y(_y) {}
//
//	int x=0;
//	int y=0;
//
//	// 通过 Player 继承
//	virtual int GetX() override
//	{
//		return x;
//	}
//	virtual int GetY() override
//	{
//		return y;
//	}
//};
extern RandomName randName;

//创建守护进程
void Daemonize() {
	//1、fork创建子进程
	int pid = fork();
	if (pid < 0) {exit(-1);}
	if (pid > 0) {
		//2、父进程退出
		exit(0);
	}
	//3、子进程开启会话，设置会话id
	pid_t sid = setsid();
	//4、子进程 设置执行路径(不必要)
	//5、子进程 重定向3个文件描述到/dev/null
	int nullFd = open("/dev/null", O_RDWR);
	if (nullFd >= 0) {
		dup2(nullFd, 0);
		dup2(nullFd, 1);
		dup2(nullFd, 2);
		close(nullFd);
	}
	//进程监控
	while (1) {
		pid = fork();
		if (pid < 0) {
			exit(-1);
		}
		//父进程等子进程退出
		if (pid>0) {
			int status = 0;
			wait(&status);
			if (0 == status) {
				exit(0);
			}
		}
		else {break;}//子进程跳出循环执行游戏业务
	}
}

int main()
{
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
	////调试信息
	//testPlayer p1(60, 107);
	//testPlayer p2(91, 118);
	//testPlayer p3(147, 133);
	//AOIWorld map(20, 200, 20, 230, 6, 6);
	//map.AddPlayer(&p1);
	//map.AddPlayer(&p2);
	//map.AddPlayer(&p3);
	//auto list = map.GetSurroundPlayers(&p1);
	//for (auto elem : list) {
	//	std::cout << "(" << dynamic_cast<testPlayer*>(elem)->GetX() << ","
	//		<< dynamic_cast<testPlayer*>(elem)->GetY() << ")" << std::endl;
	//}//
	
	Daemonize();//守护进程化
	randName.LoadFile();
	ZinxKernel::ZinxKernelInit();
	//添加监听通道类
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8899,new GameConnFact())));
	ZinxKernel::Zinx_Add_Channel(*new ZinxTimer());
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}
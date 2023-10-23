#include "GameChannel.h"

//继承父类构造函数
GameChannel::GameChannel(int _fd):ZinxTcpData(_fd)
{

}

GameChannel::~GameChannel()
{
}

//返回协议对象
AZinxHandler* GameChannel::GetInputNextStage(BytesMsg& _oInput)
{
    return mProtocol;
}

//tcp连接工厂
ZinxTcpData* GameConnFact::CreateTcpDataChannel(int _fd)
{
    //通道协议角色三方绑定：
    //创建tcp通道对象
    auto pGameChannel = new GameChannel(_fd);
    //创建协议对象
    auto pProtocol = new GameProtocol();
    //绑定协议对象
    pGameChannel->mProtocol = pProtocol;
    //将协议对象添加到Kernel
    ZinxKernel::Zinx_Add_Proto(*pProtocol);
    return pGameChannel;
}

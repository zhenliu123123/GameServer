#include "GameChannel.h"
#include"GameRole.h"

//继承父类构造函数
GameChannel::GameChannel(int _fd):ZinxTcpData(_fd)
{

}

GameChannel::~GameChannel()
{
    //回收资源
    if (NULL != mProtocol) {
        ZinxKernel::Zinx_Del_Proto(*mProtocol);
        delete mProtocol;
    }
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
    auto pChannel = new GameChannel(_fd);
    //创建协议对象
    auto pProtocol = new GameProtocol();
    //创建玩家对象
    auto pRole = new GameRole();
    //绑定协议对象与通道对象
    pChannel->mProtocol = pProtocol;
    pProtocol->mChannel = pChannel;
    //绑定协议对象与玩家对象
    pProtocol->mRole = pRole;
    pRole->mProtocol = pProtocol;
    //将协议对象添加到Kernel
    ZinxKernel::Zinx_Add_Proto(*pProtocol);
    //将玩家对象添加到Kernel
    ZinxKernel::Zinx_Add_Role(*pRole);
    return pChannel;
}

#pragma once
#include<ZinxTCP.h>
#include"GameProtocol.h"
class GameChannel :
    public ZinxTcpData
{
public:
    GameChannel(int _fd);
    virtual ~GameChannel();
    //通道协议角色三方绑定，因为要给每个玩家的信息处理方式不同
    //绑定协议
    GameProtocol *mProtocol = NULL;
    // 通过 ZinxTcpData 继承
    virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

//tcp连接工厂
class GameConnFact :
    public IZinxTcpConnFact
{
    // 通过 IZinxTcpConnFact 继承
    virtual ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};
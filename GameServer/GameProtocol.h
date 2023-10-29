#pragma once
#include <zinx.h>
//不可以在这里再次包含通道层！
//在头文件包含头文件要注意，避免循环包含。cpp文件无所谓。
//#include"GameChannel.h" 可以通过#pragma once解决 但是尽量源头避免
class GameChannel;
class GameRole;
class GameProtocol :
    public Iprotocol
{
public:
    std::string szLast;
public:
    GameProtocol();
    virtual~GameProtocol();
    //绑定
    GameChannel* mChannel = NULL;
    GameRole* mRole = NULL;
    // 通过 Iprotocol 继承
    virtual UserData* raw2request(std::string _szInput) override;
    virtual std::string* response2raw(UserData& _oUserData) override;
    virtual Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) override;
    virtual Ichannel* GetMsgSender(BytesMsg& _oBytes) override;
};


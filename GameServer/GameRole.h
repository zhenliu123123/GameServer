#pragma once
#include <zinx.h>
class GameProtocol;//不用到类里成员(属性、函数...)时后可以只声明一下
class GameRole :
    public Irole
{
public:
    // 通过 Irole 继承
    virtual bool Init() override;
    virtual UserData* ProcMsg(UserData& _poUserData) override;
    virtual void Fini() override;
    //绑定
    GameProtocol* mProtocol = nullptr;
};


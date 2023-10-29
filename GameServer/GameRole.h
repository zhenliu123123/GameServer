#pragma once
#include <zinx.h>
#include"AOIWorld.h"
#include"GameMsg.h"
#include"msg.pb.h"
class GameProtocol;//不用到类里成员(属性、函数...)时后可以只声明一下
class GameRole :
    public Irole,public Player//多继承
{
    float x = 0;
    float y = 0;//客户端定义的y为高
    float z = 0;
    float v = 0;
    int playerPid = 0;
    std::string playerName;
    //同步周围玩家信息的函数
    void ProcTalkMsg(std::string _content);
    void ProcMoveMsg(float _x,float _y,float _z,float _v);

    void ViewAppear(GameRole* _pRole);
    void ViewLost(GameRole* _pRole);
    //创建信息
    GameMsg* CreateIDNameLogin();
    GameMsg* CreateSurPlayers();
    GameMsg* CreateSelfPosition();
    GameMsg* CreateIDNameLogoff();
    GameMsg* CreateTalkBroadCast(std::string _content);
public:
    GameRole();
    virtual ~GameRole();
    // 通过 Irole 继承
    virtual bool Init() override;
    virtual UserData* ProcMsg(UserData& _poUserData) override;
    virtual void Fini() override;
    //绑定
    GameProtocol* mProtocol = NULL;

    // 通过 Player 继承
    virtual int GetX() override;
    virtual int GetY() override;
};


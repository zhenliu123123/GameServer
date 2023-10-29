#include "GameRole.h"
#include"GameMsg.h"
#include"AOIWorld.h"
#include"msg.pb.h"
#include"GameProtocol.h"
#include"GameChannel.h"
#include<algorithm>
#include<random>
#include"ZinxTimer.h"
//创建全局随机数引擎
static std::default_random_engine GameRandomEngine(time(NULL));
//创建AOIWorld全局对象
static AOIWorld gameWorld(0,400,0,400,20,20);

void GameRole::ProcTalkMsg(std::string _content)
{
    std::list<Irole*> allIroleList = ZinxKernel::Zinx_GetAllRole();
    for (Irole* pSingleIrole : allIroleList) {
        GameRole* pGameRole = dynamic_cast<GameRole*>(pSingleIrole);
        GameMsg* pMsg = CreateTalkBroadCast(_content);
        ZinxKernel::Zinx_SendOut(*pMsg, *(pGameRole->mProtocol));
    }
}

void GameRole::ProcMoveMsg(float _x, float _y, float _z, float _v)
{
    //1、跨网格处理
    //获取原来的 周围玩家s1
    std::list<Player*> s1 = gameWorld.GetSurroundPlayers(this);
    //更新坐标，摘除旧格子，添加新格子，获取 新周围玩家s2
    gameWorld.DelPlayer(this);
    x = _x;
    y = _y;
    z = _z;
    v = _v;
    gameWorld.AddPlayer(this);
    std::list<Player*> s2 = gameWorld.GetSurroundPlayers(this);
    //遍历s2，若元素不属于s1，视野出现
    for (Player* singlePlayer : s2) {
        if (s1.end() == std::find(s1.begin(), s1.end(), singlePlayer)) {
            ViewAppear(dynamic_cast<GameRole*>(singlePlayer));
        }
    }
    //遍历s1，若元素不属于s2，视野消失
    for (Player* singlePlayer : s1) {
        if (s2.end()==std::find(s2.begin(),s2.end(),singlePlayer)) {
            ViewLost(dynamic_cast<GameRole*>(singlePlayer));
        }
    }
    //2、广播新位置给周围玩家
    std::list <Player*> surPlayersList = gameWorld.GetSurroundPlayers(this);
    for (Player* pSinglePlayer : surPlayersList) {
        //组成待发送报文
        pb::BroadCast* pBroadCastMsg = new pb::BroadCast();
        pb::Position* pPosition = pBroadCastMsg->mutable_p();
        pPosition->set_x(_x);
        pPosition->set_y(_y);
        pPosition->set_z(_z);
        pPosition->set_v(_v);
        pBroadCastMsg->set_pid(playerPid);
        pBroadCastMsg->set_tp(4);
        pBroadCastMsg->set_username(playerName);
        //遍历周围玩家发送
        GameRole* pGameRole = dynamic_cast<GameRole*>(pSinglePlayer);
        GameMsg* pMsg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pBroadCastMsg);
        ZinxKernel::Zinx_SendOut(*pMsg, *(pGameRole->mProtocol));
    }
}

void GameRole::ViewAppear(GameRole* _pRole)
{
    //向自己发送参数玩家的200消息(玩家ID，tp1聊天内容/tp2初始位置/tp3动作（预留）/tp4新位置)
    ZinxKernel::Zinx_SendOut(*(_pRole->CreateSelfPosition()), *(this->mProtocol));
    //向参数里的玩家发自己的200消息
    ZinxKernel::Zinx_SendOut(*(this->CreateSelfPosition()), *(_pRole->mProtocol));
}

void GameRole::ViewLost(GameRole* _pRole)
{
    //向自己发送参数玩家的201消息(玩家ID和玩家姓名)
    ZinxKernel::Zinx_SendOut(*(_pRole->CreateIDNameLogoff()), *(this->mProtocol));
    //向参数里的玩家发自己的201消息
    ZinxKernel::Zinx_SendOut(*(this->CreateIDNameLogoff()), *(_pRole->mProtocol));
}

GameMsg* GameRole::CreateIDNameLogin()
{
    pb::SyncPid* pMsg = new pb::SyncPid();
    pMsg->set_pid(playerPid);
    pMsg->set_username(playerName);
    GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME,pMsg);
    return pRet;
}

GameMsg* GameRole::CreateSurPlayers()
{
    pb::SyncPlayers* pMsg = new pb::SyncPlayers();

    std::list<Player*> surPlayersList = gameWorld.GetSurroundPlayers(this);//auto
    //auto
    for (Player* singlePlayer : surPlayersList) {
        //子类(子消息)挂到父类(父消息)，返回父类的指针即pb::Player
        pb::Player* pPlayer = pMsg->add_ps();//auto
        GameRole* pRole = dynamic_cast<GameRole*>(singlePlayer);//auto
        pPlayer->set_pid(pRole->playerPid);
        pPlayer->set_username(pRole->playerName);
        //子类(子消息)挂到父类(父消息)，返回子类的指针pb::Position
        pb::Position* pPosition=pPlayer->mutable_p();//auto
        pPosition->set_x(pRole->x);
        pPosition->set_y(pRole->y);
        pPosition->set_z(pRole->z);
        pPosition->set_v(pRole->v); 
    }
    GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_SRD_POSITION, pMsg);
    return pRet;
}

GameMsg* GameRole::CreateSelfPosition()
{
    //定义protobuf数据
    pb::BroadCast* pMsg = new pb::BroadCast();
    //写protobuf数据
    pMsg->set_pid(playerPid);
    pMsg->set_username(playerName);
    pMsg->set_tp(2);//tp=1:聊天内容/tp=2:初始位置/tp=3:动作（预留）/tp=4:新位置
    
    //子类挂到父类，返回子类指针
    pb::Position* pPositon = pMsg->mutable_p();//auto
    pPositon->set_x(x);
    pPositon->set_y(y);
    pPositon->set_z(z);
    pPositon->set_v(v);
    //发送protobuf数据
    GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_BROADCAST,pMsg);
    return pRet;
}

GameMsg* GameRole::CreateIDNameLogoff()
{
    //定义protobuf消息
    pb::SyncPid* pMsg = new pb::SyncPid();
    //写protobuf消息
    pMsg->set_pid(playerPid);
    pMsg->set_username(playerName);
    //发送protobuf消息
    GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_LOGOFF_ID_NAME,pMsg);
    return pRet;
}

GameMsg* GameRole::CreateTalkBroadCast(std::string _content)
{
    pb::BroadCast* pMsg = new pb::BroadCast();
    pMsg->set_pid(playerPid);
    pMsg->set_username(playerName);
    pMsg->set_tp(1);
    pMsg->set_content(_content);
    GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_BROADCAST,pMsg);
    return pRet;
}

GameRole::GameRole()
{
    x = 100 + GameRandomEngine() % 30;
    z = 100 + GameRandomEngine() % 30;
    playerName = "zhenliu";
}

GameRole::~GameRole()
{
}

//创建定时器类继承定时器处理类
class ExitTimer
    :public TimeOutProc {
    // 通过 TimeOutProc 继承
    virtual void Proc() override
    {
        ZinxKernel::Zinx_Exit();
    }
    virtual int getTimeSec() override
    {
        //玩家全部下线后10s，关闭服务器
        return 10;
    }
};

//实例化一个定时器全局对象
static ExitTimer exitTimer;

bool GameRole::Init()
{
    //如果线上没玩家，本玩家作为第一个玩家上线Init，删除退出定时器
    if (ZinxKernel::Zinx_GetAllRole().size()<=0) {
        TimerOutManager::getInstance().delTask(&exitTimer);
    }

    //添加自己到游戏世界
    bool bRet = false;
    playerPid = mProtocol->mChannel->GetFd();//设置玩家ID为当前连接的fd
    bRet = gameWorld.AddPlayer(this);

    if (true == bRet) {
        //向 自己 发送 ID和名称
        GameMsg* pMsg = CreateIDNameLogin();//auto
        ZinxKernel::Zinx_SendOut(*pMsg,*mProtocol);
        //向 自己 发送 周围玩家 坐标
        pMsg = CreateSurPlayers();
        ZinxKernel::Zinx_SendOut(*pMsg,*mProtocol);
        //向 周围玩家 发送 其位置
        std::list<Player*> surPlayers = gameWorld.GetSurroundPlayers(this);//auto
        //Player*
        for (auto singlePlayer : surPlayers) {
            pMsg = CreateSelfPosition();
            GameRole* pRole = dynamic_cast<GameRole*>(singlePlayer);//auto
            ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->mProtocol));
        }
    }
    return bRet;
}

//处理游戏相关的用户请求
UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    //从协议层获得转换成16进制后的信息，是MultiMsg类型
    GET_REF2DATA(MultiMsg, mulMsg, _poUserData);
    for (GameMsg* singleGameMsg : mulMsg.mMsgs) {
        ////调试信息
        //这里pMsg为Protobuf类型
        std::cout << "type is" << singleGameMsg->enMsgType << std::endl;
        std::cout << singleGameMsg->pMsg->Utf8DebugString() << std::endl;

        //定义Position指针来获取坐标(不能在Switch内部定义)
        pb::Position* pNewPosition = dynamic_cast<pb::Position*>(singleGameMsg->pMsg);
        switch (singleGameMsg->enMsgType) {
        //把聊天消息发给所有玩家
        case GameMsg::MSG_TYPE_CHAT_CONTENT:
            //这里pMsg为Protobuf类型
            ProcTalkMsg(dynamic_cast<pb::Talk*>(singleGameMsg->pMsg)->content());
            break;
        //把新位置信息发给周围玩家
        case GameMsg::MSG_TYPE_NEW_POSITION:
            ProcMoveMsg(pNewPosition->x(), pNewPosition->y(), pNewPosition->z(), pNewPosition->z());
            break;
        default:
            break;
        }
    }
    return nullptr;
}



void GameRole::Fini()
{
    //向 周围玩家 发送 其下线的消息
    std::list<Player*> surPlayerList = gameWorld.GetSurroundPlayers(this);//auto
    //Player*
    for (auto singlePlayer : surPlayerList) {
        GameMsg* pMsg = CreateIDNameLogoff();//auto
        GameRole* pRole = dynamic_cast<GameRole*>(singlePlayer);//auto
        ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->mProtocol));
    }
    gameWorld.DelPlayer(this);

    //最后一个玩家下线后，开始计时，20s没有新玩家上线就退出服务器
    if (ZinxKernel::Zinx_GetAllRole().size()<=0) {
        TimerOutManager::getInstance().addTask(&exitTimer);
    }
}

//实现Player类的纯虚函数
int GameRole::GetX()
{
    return (int)x;
}

int GameRole::GetY()
{
    return (int)z;//客户端定义y为高，z为纵坐标
}

#include "GameRole.h"
#include"GameMsg.h"
bool GameRole::Init()
{
    return true;
}

//处理游戏相关的用户请求
UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    //从协议层获得转换成16进制后的信息，是MultiMsg类型
    GET_REF2DATA(MultiMsg, mulMsg, _poUserData);
    //调试信息
    for (auto elem : mulMsg.mMsgs) {
        std::cout<<elem->pMsg->Utf8DebugString()<<std::endl;
    }
    return nullptr;
}

void GameRole::Fini()
{
}

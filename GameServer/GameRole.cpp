#include "GameRole.h"
#include"GameMsg.h"
bool GameRole::Init()
{
    return true;
}

//������Ϸ��ص��û�����
UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    //��Э�����ת����16���ƺ����Ϣ����MultiMsg����
    GET_REF2DATA(MultiMsg, mulMsg, _poUserData);
    //������Ϣ
    for (auto elem : mulMsg.mMsgs) {
        std::cout<<elem->pMsg->Utf8DebugString()<<std::endl;
    }
    return nullptr;
}

void GameRole::Fini()
{
}

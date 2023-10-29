#include "GameRole.h"
#include"GameMsg.h"
#include"AOIWorld.h"
#include"msg.pb.h"
#include"GameProtocol.h"
#include"GameChannel.h"
#include<algorithm>
#include<random>
#include"ZinxTimer.h"
//����ȫ�����������
static std::default_random_engine GameRandomEngine(time(NULL));
//����AOIWorldȫ�ֶ���
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
    //1����������
    //��ȡԭ���� ��Χ���s1
    std::list<Player*> s1 = gameWorld.GetSurroundPlayers(this);
    //�������꣬ժ���ɸ��ӣ�����¸��ӣ���ȡ ����Χ���s2
    gameWorld.DelPlayer(this);
    x = _x;
    y = _y;
    z = _z;
    v = _v;
    gameWorld.AddPlayer(this);
    std::list<Player*> s2 = gameWorld.GetSurroundPlayers(this);
    //����s2����Ԫ�ز�����s1����Ұ����
    for (Player* singlePlayer : s2) {
        if (s1.end() == std::find(s1.begin(), s1.end(), singlePlayer)) {
            ViewAppear(dynamic_cast<GameRole*>(singlePlayer));
        }
    }
    //����s1����Ԫ�ز�����s2����Ұ��ʧ
    for (Player* singlePlayer : s1) {
        if (s2.end()==std::find(s2.begin(),s2.end(),singlePlayer)) {
            ViewLost(dynamic_cast<GameRole*>(singlePlayer));
        }
    }
    //2���㲥��λ�ø���Χ���
    std::list <Player*> surPlayersList = gameWorld.GetSurroundPlayers(this);
    for (Player* pSinglePlayer : surPlayersList) {
        //��ɴ����ͱ���
        pb::BroadCast* pBroadCastMsg = new pb::BroadCast();
        pb::Position* pPosition = pBroadCastMsg->mutable_p();
        pPosition->set_x(_x);
        pPosition->set_y(_y);
        pPosition->set_z(_z);
        pPosition->set_v(_v);
        pBroadCastMsg->set_pid(playerPid);
        pBroadCastMsg->set_tp(4);
        pBroadCastMsg->set_username(playerName);
        //������Χ��ҷ���
        GameRole* pGameRole = dynamic_cast<GameRole*>(pSinglePlayer);
        GameMsg* pMsg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pBroadCastMsg);
        ZinxKernel::Zinx_SendOut(*pMsg, *(pGameRole->mProtocol));
    }
}

void GameRole::ViewAppear(GameRole* _pRole)
{
    //���Լ����Ͳ�����ҵ�200��Ϣ(���ID��tp1��������/tp2��ʼλ��/tp3������Ԥ����/tp4��λ��)
    ZinxKernel::Zinx_SendOut(*(_pRole->CreateSelfPosition()), *(this->mProtocol));
    //����������ҷ��Լ���200��Ϣ
    ZinxKernel::Zinx_SendOut(*(this->CreateSelfPosition()), *(_pRole->mProtocol));
}

void GameRole::ViewLost(GameRole* _pRole)
{
    //���Լ����Ͳ�����ҵ�201��Ϣ(���ID���������)
    ZinxKernel::Zinx_SendOut(*(_pRole->CreateIDNameLogoff()), *(this->mProtocol));
    //����������ҷ��Լ���201��Ϣ
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
        //����(����Ϣ)�ҵ�����(����Ϣ)�����ظ����ָ�뼴pb::Player
        pb::Player* pPlayer = pMsg->add_ps();//auto
        GameRole* pRole = dynamic_cast<GameRole*>(singlePlayer);//auto
        pPlayer->set_pid(pRole->playerPid);
        pPlayer->set_username(pRole->playerName);
        //����(����Ϣ)�ҵ�����(����Ϣ)�����������ָ��pb::Position
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
    //����protobuf����
    pb::BroadCast* pMsg = new pb::BroadCast();
    //дprotobuf����
    pMsg->set_pid(playerPid);
    pMsg->set_username(playerName);
    pMsg->set_tp(2);//tp=1:��������/tp=2:��ʼλ��/tp=3:������Ԥ����/tp=4:��λ��
    
    //����ҵ����࣬��������ָ��
    pb::Position* pPositon = pMsg->mutable_p();//auto
    pPositon->set_x(x);
    pPositon->set_y(y);
    pPositon->set_z(z);
    pPositon->set_v(v);
    //����protobuf����
    GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_BROADCAST,pMsg);
    return pRet;
}

GameMsg* GameRole::CreateIDNameLogoff()
{
    //����protobuf��Ϣ
    pb::SyncPid* pMsg = new pb::SyncPid();
    //дprotobuf��Ϣ
    pMsg->set_pid(playerPid);
    pMsg->set_username(playerName);
    //����protobuf��Ϣ
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

//������ʱ����̳ж�ʱ��������
class ExitTimer
    :public TimeOutProc {
    // ͨ�� TimeOutProc �̳�
    virtual void Proc() override
    {
        ZinxKernel::Zinx_Exit();
    }
    virtual int getTimeSec() override
    {
        //���ȫ�����ߺ�10s���رշ�����
        return 10;
    }
};

//ʵ����һ����ʱ��ȫ�ֶ���
static ExitTimer exitTimer;

bool GameRole::Init()
{
    //�������û��ң��������Ϊ��һ���������Init��ɾ���˳���ʱ��
    if (ZinxKernel::Zinx_GetAllRole().size()<=0) {
        TimerOutManager::getInstance().delTask(&exitTimer);
    }

    //����Լ�����Ϸ����
    bool bRet = false;
    playerPid = mProtocol->mChannel->GetFd();//�������IDΪ��ǰ���ӵ�fd
    bRet = gameWorld.AddPlayer(this);

    if (true == bRet) {
        //�� �Լ� ���� ID������
        GameMsg* pMsg = CreateIDNameLogin();//auto
        ZinxKernel::Zinx_SendOut(*pMsg,*mProtocol);
        //�� �Լ� ���� ��Χ��� ����
        pMsg = CreateSurPlayers();
        ZinxKernel::Zinx_SendOut(*pMsg,*mProtocol);
        //�� ��Χ��� ���� ��λ��
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

//������Ϸ��ص��û�����
UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    //��Э�����ת����16���ƺ����Ϣ����MultiMsg����
    GET_REF2DATA(MultiMsg, mulMsg, _poUserData);
    for (GameMsg* singleGameMsg : mulMsg.mMsgs) {
        ////������Ϣ
        //����pMsgΪProtobuf����
        std::cout << "type is" << singleGameMsg->enMsgType << std::endl;
        std::cout << singleGameMsg->pMsg->Utf8DebugString() << std::endl;

        //����Positionָ������ȡ����(������Switch�ڲ�����)
        pb::Position* pNewPosition = dynamic_cast<pb::Position*>(singleGameMsg->pMsg);
        switch (singleGameMsg->enMsgType) {
        //��������Ϣ�����������
        case GameMsg::MSG_TYPE_CHAT_CONTENT:
            //����pMsgΪProtobuf����
            ProcTalkMsg(dynamic_cast<pb::Talk*>(singleGameMsg->pMsg)->content());
            break;
        //����λ����Ϣ������Χ���
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
    //�� ��Χ��� ���� �����ߵ���Ϣ
    std::list<Player*> surPlayerList = gameWorld.GetSurroundPlayers(this);//auto
    //Player*
    for (auto singlePlayer : surPlayerList) {
        GameMsg* pMsg = CreateIDNameLogoff();//auto
        GameRole* pRole = dynamic_cast<GameRole*>(singlePlayer);//auto
        ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->mProtocol));
    }
    gameWorld.DelPlayer(this);

    //���һ��������ߺ󣬿�ʼ��ʱ��20sû����������߾��˳�������
    if (ZinxKernel::Zinx_GetAllRole().size()<=0) {
        TimerOutManager::getInstance().addTask(&exitTimer);
    }
}

//ʵ��Player��Ĵ��麯��
int GameRole::GetX()
{
    return (int)x;
}

int GameRole::GetY()
{
    return (int)z;//�ͻ��˶���yΪ�ߣ�zΪ������
}

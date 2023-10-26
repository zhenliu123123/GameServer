#include "GameProtocol.h"
#include"GameMsg.h"
#include"GameChannel.h"
#include"msg.pb.h"
#include"GameRole.h"
//��������Ǵ�ͨ��������ԭʼ���� �������TCP��(������0.5������Ҳ������1.8������)
//����ֵ��ת�������Ϣ����
UserData* GameProtocol::raw2request(std::string _szInput)
{
    MultiMsg* pRet = new MultiMsg();
    szLast.append(_szInput);
    while (1) {
        if (szLast.length() < 8) {
            break;
        }
        //��ʮ��������ת����������������ֱ�Ӹ��ƻ���������ֽ���(���С��)
        //ǰ�ĸ��ֽڶ�ȡ��Ϣ����
        int msgLength = 0;
        msgLength |= szLast[0] << 0;
        msgLength |= szLast[1] << 8;
        msgLength |= szLast[2] << 16;
        msgLength |= szLast[3] << 24;
        //�м��ĸ��ֽڶ�ȡ��ϢID
        int msgID = 0;
        msgID |= szLast[4] << 0;
        msgID |= szLast[5] << 8;
        msgID |= szLast[6] << 16;
        msgID |= szLast[7] << 24;
        //����Ƿ�һ��������
        if (szLast.length() < msgLength + 8) {
            break;
        }
        GameMsg* pMsg = new GameMsg((GameMsg::MSG_TYPE)msgID, szLast.substr(8, msgLength));
        szLast.erase(0, 8 + msgLength);//�ӻ����е�������
        pRet->mMsgs.push_back(pMsg);//����GameMsg��������б�
    }
    ////������Ϣ
    //for (auto elem : pRet->mMsgs) {
    //    std::cout<<elem->pMsg->Utf8DebugString()<<std::endl;
    //}
    ////������Ϣ
    //pb::Talk* pMsg = new pb::Talk();
    //pMsg->set_content("hello");
    //GameMsg* pGameMsg = new GameMsg(GameMsg::MSG_TYPE_CHAT_CONTENT, pMsg);
    //ZinxKernel::Zinx_SendOut((*pGameMsg), *this);
    return pRet;
}   

//��������ҵ��㣬�����û�����
//����ʮ�������ֽ���
std::string* GameProtocol::response2raw(UserData& _oUserData)
{
    int msgLength = 0;
    int msgID = 0;
    std::string msgContent;
    GET_REF2DATA(GameMsg, usrRequest, _oUserData);
    std::string* pRet = new std::string();
    msgID = usrRequest.enMsgType;
    msgContent = usrRequest.serialize();//ֱ���÷�װ�õ����л���ת�����ƣ�����
    msgLength = msgContent.size();
    pRet->push_back((msgLength >> 0) & 0xff);
    pRet->push_back((msgLength >> 8) & 0xff);
    pRet->push_back((msgLength >> 16) & 0xff);
    pRet->push_back((msgLength >> 24) & 0xff);

    pRet->push_back((msgID >> 0) & 0xff);
    pRet->push_back((msgID >> 8) & 0xff);
    pRet->push_back((msgID >> 16) & 0xff);
    pRet->push_back((msgID >> 24) & 0xff);

    pRet->append(msgContent);
    return pRet;
}

//���ش�����Ϣ����Ҷ���
Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
    return mRole;
}

//���ط��͵�ͨ��
Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes)
{
    return mChannel;
}

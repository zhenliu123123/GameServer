#include "GameProtocol.h"
#include"GameMsg.h"
#include"GameChannel.h"
#include"msg.pb.h"
#include"GameRole.h"
//输入参数是从通道传来的原始报文 输入的是TCP流(可能是0.5条报文也可能是1.8条报文)
//返回值是转换后的消息对象
UserData* GameProtocol::raw2request(std::string _szInput)
{
    MultiMsg* pRet = new MultiMsg();
    szLast.append(_szInput);
    while (1) {
        if (szLast.length() < 8) {
            break;
        }
        //将十六进制数转换成整型数，不能直接复制会忽略网络字节序(大端小端)
        //前四个字节读取消息长度
        int msgLength = 0;
        msgLength |= szLast[0] << 0;
        msgLength |= szLast[1] << 8;
        msgLength |= szLast[2] << 16;
        msgLength |= szLast[3] << 24;
        //中间四个字节读取消息ID
        int msgID = 0;
        msgID |= szLast[4] << 0;
        msgID |= szLast[5] << 8;
        msgID |= szLast[6] << 16;
        msgID |= szLast[7] << 24;
        //检测是否够一整条报文
        if (szLast.length() < msgLength + 8) {
            break;
        }
        GameMsg* pMsg = new GameMsg((GameMsg::MSG_TYPE)msgID, szLast.substr(8, msgLength));
        szLast.erase(0, 8 + msgLength);//从缓存中弹出报文
        pRet->mMsgs.push_back(pMsg);//插入GameMsg玩家请求列表
    }
    ////调试信息
    //for (auto elem : pRet->mMsgs) {
    //    std::cout<<elem->pMsg->Utf8DebugString()<<std::endl;
    //}
    ////调试信息
    //pb::Talk* pMsg = new pb::Talk();
    //pMsg->set_content("hello");
    //GameMsg* pGameMsg = new GameMsg(GameMsg::MSG_TYPE_CHAT_CONTENT, pMsg);
    //ZinxKernel::Zinx_SendOut((*pGameMsg), *this);
    return pRet;
}   

//参数来自业务层，接收用户请求
//返回十六进制字节流
std::string* GameProtocol::response2raw(UserData& _oUserData)
{
    int msgLength = 0;
    int msgID = 0;
    std::string msgContent;
    GET_REF2DATA(GameMsg, usrRequest, _oUserData);
    std::string* pRet = new std::string();
    msgID = usrRequest.enMsgType;
    msgContent = usrRequest.serialize();//直接用封装好的序列化（转二进制）函数
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

//返回处理信息的玩家对象
Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
    return mRole;
}

//返回发送的通道
Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes)
{
    return mChannel;
}

#include "GameMsg.h"
#include"msg.pb.h"

//用初始化列表初始化
GameMsg::GameMsg(MSG_TYPE _type, google::protobuf::Message* _pMsg):enMsgType(_type),pMsg(_pMsg)
{
}

GameMsg::GameMsg(MSG_TYPE _type, std::string _stream):enMsgType(_type)
{
	//通过简单工厂构造具体的消息对象
	switch (_type)
	{
	case GameMsg::MSG_TYPE_LOGIN_ID_NAME:
		//pb和std一样是个命名空间
		pMsg = new pb::SyncPid();
		break;
	case GameMsg::MSG_TYPE_CHAT_CONTENT:
		pMsg = new pb::Talk();
		break;
	case GameMsg::MSG_TYPE_NEW_POSITION:
		pMsg = new pb::Position();
		break;
	case GameMsg::MSG_TYPE_BROADCAST:
		pMsg = new pb::BroadCast();
		break;
	case GameMsg::MSG_TYPE_LOGOFF_ID_NAME:
		pMsg = new pb::SyncPid();
		break;
	case GameMsg::MSG_TYPE_SRD_POSITION:
		pMsg = new pb::SyncPlayers();
		break;
	default:
		break;
	}
	//解析_stream中的内容并填充到pMsg指向的对象
	pMsg->ParseFromString(_stream);
}

std::string GameMsg::serialize()
{
	std::string pRet;
	//序列化：转为二进制字节流
	pMsg->SerializeToString(&pRet);
	return pRet;
}
GameMsg::~GameMsg()
{

}
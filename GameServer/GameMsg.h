#pragma once
#include <zinx.h>
#include<google/protobuf/message.h>
/**************************************************************************************************/
/*消息ID		消息内容	   发送方向			客户端处理				服务器处理				  */
/*	1		玩家ID和玩家姓名	 S->C		记录自己ID和姓名				无					  */
/*	2			聊天内容	     C->S				无					广播给所有玩家			  */
/*	3     		新位置			 C->S				无			  处理玩家位置更新后的信息同步    */
/*	200			注释①			 S->C		根据子类型不通而不同			无					  */
/*	201		玩家ID和玩家姓名	 S->C	   把该ID的玩家从画面中拿掉			无					  */
/*	202		周围玩家们的位置	 S->C		在画面中显示周围的玩家			无					  */
/*表注释①：玩家ID，聊天内容 / 初始位置 / 动作（预留） / 新位置									  */
/**************************************************************************************************/

class GameMsg :
    public UserData
{
public:
	//用户的请求信息内容
	google::protobuf::Message* pMsg = NULL;
	//用枚举定义ID和消息关系
	enum MSG_TYPE {
		MSG_TYPE_LOGIN_ID_NAME = 1,
		MSG_TYPE_CHAT_CONTENT=2,
		MSG_TYPE_NEW_POSITION=3,
		MSG_TYPE_BROADCAST=200,
		MSG_TYPE_LOGOFF_ID_NAME=201,
		MSG_TYPE_SRD_POSITION=202
	}enMsgType;//定义一个变量来存储
	//已知消息内容时，构造消息对象
	GameMsg(MSG_TYPE _type,google::protobuf::Message *_pMsg);
	//将字节流内容转换成消息结构(已知字节流(字符串形式)可以通过构造函数转化成protobuf形式)
	//解析：从二进制到结构化数据
	GameMsg(MSG_TYPE _type,std::string _stream);
	//序列化：从结构化转换回二进制
	std::string serialize();
	virtual ~GameMsg();
};


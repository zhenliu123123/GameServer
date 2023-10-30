#pragma once
#include<vector>
#include<string>
//随机姓名池
//线性表存姓和 (名组成的线性表) 
//取名字:随机取姓，随机取名
//还名字:尾部追加姓或名
//读姓文件的同时读名文件，边追加节点

//姓和 名组成的线性表
//即姓名池节点
class NamePoolNode {
public:
	//姓
	std::string mLastName;
	//名组成的线性表
	std::vector<std::string> mFirstNameVector;
};
class RandomName
{

	std::vector<NamePoolNode*> mNamePool;
public:
	RandomName();
	//取名字
	std::string GetName();
	//还名字
	void ReleaseName(std::string _name);
	//加载文件
	void LoadFile();
	virtual ~RandomName();
};


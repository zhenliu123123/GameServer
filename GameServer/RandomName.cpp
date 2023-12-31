#include "RandomName.h"
#include<fstream>
#include<iostream>
#include<errno.h>
#include<random>

//bug:出现赵赵、王王这种名字

static std::default_random_engine PoolRandomEngine(time(NULL));
RandomName::RandomName()
{
}

void RandomName::LoadFile()
{
	std::vector<std::string> tmpFirNameVector;
	std::string tmpName;
	std::fstream LastName("randomLastname.txt", std::ios::in);//姓
	if (!LastName.is_open()) { perror("LastName error"); }
	std::fstream FirstName("randomFirstname.txt", std::ios::in);//名
	if (!FirstName.is_open()) { perror("FiestName error"); }

	//截取的时候根据文件内不同可能会截取到\n或者空格
	//   geline默认以\n分割，并且会把\n读入
	//   >>读的时候默认以空格分割，并且不读入空格

	//读取所有名字组成一个线性表
	while (FirstName>>tmpName) {
		tmpFirNameVector.push_back(tmpName);
	}
	//读取所有姓，创建姓名池节点，拷贝名字组成的线性表
	while (LastName>>tmpName) {
		NamePoolNode* namePoolNode = new NamePoolNode();
		namePoolNode->mLastName = tmpName;
		namePoolNode->mFirstNameVector = tmpFirNameVector;
		mNamePool.push_back(namePoolNode);
	}
}

std::string RandomName::GetName()
{
	std::string pRet;
	//取姓
	auto num = PoolRandomEngine() % mNamePool.size();
	std::string lastName = mNamePool[num]->mLastName;
	//取名
	auto randomIndex = PoolRandomEngine() % mNamePool[num]->mFirstNameVector.size();
	std::string FirstName = mNamePool[num]->mFirstNameVector[randomIndex];
	//若本姓的所有名都取完了，把姓删掉
	if (mNamePool[num]->mFirstNameVector.size()<=0) {
		delete mNamePool[num];
		mNamePool.erase(mNamePool.begin() + num);
	}
	

	pRet = lastName +" "+ FirstName;
	return pRet;
}

void RandomName::ReleaseName(std::string _name)
{
	//分割名字，得到姓和名
	auto space_pos = _name.find(" ", 0);
	auto last = _name.substr(0, space_pos);
	auto first = _name.substr(space_pos + 1, _name.size() - space_pos - 1);

	bool flag = 0;

	//节点没被删除情况下
	for (NamePoolNode* singleNode : mNamePool) {
		if (last == singleNode->mLastName) {
			flag = 1;
			singleNode->mFirstNameVector.push_back(last);
		}
	}
	//节点已经被删除，创建节点
	if (flag == 0) {
		NamePoolNode* newNode = new NamePoolNode();
		newNode->mLastName = last; //为什么没写？初始化节点各个数据为什么没有初始化 姓？？
		newNode->mFirstNameVector.push_back(first);
		mNamePool.push_back(newNode);
	}
}


RandomName::~RandomName()
{
}

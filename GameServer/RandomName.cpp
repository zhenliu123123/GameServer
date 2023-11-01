#include "RandomName.h"
#include<fstream>
#include<iostream>
#include<errno.h>
#include<random>

//bug:�������ԡ�������������

static std::default_random_engine PoolRandomEngine(time(NULL));
RandomName::RandomName()
{
}

void RandomName::LoadFile()
{
	std::vector<std::string> tmpFirNameVector;
	std::string tmpName;
	std::fstream LastName("randomLastname.txt", std::ios::in);//��
	if (!LastName.is_open()) { perror("LastName error"); }
	std::fstream FirstName("randomFirstname.txt", std::ios::in);//��
	if (!FirstName.is_open()) { perror("FiestName error"); }

	//��ȡ��ʱ������ļ��ڲ�ͬ���ܻ��ȡ��\n���߿ո�
	//   gelineĬ����\n�ָ���һ��\n����
	//   >>����ʱ��Ĭ���Կո�ָ���Ҳ�����ո�

	//��ȡ�����������һ�����Ա�
	while (FirstName>>tmpName) {
		tmpFirNameVector.push_back(tmpName);
	}
	//��ȡ�����գ����������ؽڵ㣬����������ɵ����Ա�
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
	//ȡ��
	auto num = PoolRandomEngine() % mNamePool.size();
	std::string lastName = mNamePool[num]->mLastName;
	//ȡ��
	auto randomIndex = PoolRandomEngine() % mNamePool[num]->mFirstNameVector.size();
	std::string FirstName = mNamePool[num]->mFirstNameVector[randomIndex];
	//�����յ���������ȡ���ˣ�����ɾ��
	if (mNamePool[num]->mFirstNameVector.size()<=0) {
		delete mNamePool[num];
		mNamePool.erase(mNamePool.begin() + num);
	}
	

	pRet = lastName +" "+ FirstName;
	return pRet;
}

void RandomName::ReleaseName(std::string _name)
{
	//�ָ����֣��õ��պ���
	auto space_pos = _name.find(" ", 0);
	auto last = _name.substr(0, space_pos);
	auto first = _name.substr(space_pos + 1, _name.size() - space_pos - 1);

	bool flag = 0;

	//�ڵ�û��ɾ�������
	for (NamePoolNode* singleNode : mNamePool) {
		if (last == singleNode->mLastName) {
			flag = 1;
			singleNode->mFirstNameVector.push_back(last);
		}
	}
	//�ڵ��Ѿ���ɾ���������ڵ�
	if (flag == 0) {
		NamePoolNode* newNode = new NamePoolNode();
		newNode->mLastName = last; //Ϊʲôûд����ʼ���ڵ��������Ϊʲôû�г�ʼ�� �գ���
		newNode->mFirstNameVector.push_back(first);
		mNamePool.push_back(newNode);
	}
}


RandomName::~RandomName()
{
}

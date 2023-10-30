#pragma once
#include<vector>
#include<string>
//���������
//���Ա���պ� (����ɵ����Ա�) 
//ȡ����:���ȡ�գ����ȡ��
//������:β��׷���ջ���
//�����ļ���ͬʱ�����ļ�����׷�ӽڵ�

//�պ� ����ɵ����Ա�
//�������ؽڵ�
class NamePoolNode {
public:
	//��
	std::string mLastName;
	//����ɵ����Ա�
	std::vector<std::string> mFirstNameVector;
};
class RandomName
{

	std::vector<NamePoolNode*> mNamePool;
public:
	RandomName();
	//ȡ����
	std::string GetName();
	//������
	void ReleaseName(std::string _name);
	//�����ļ�
	void LoadFile();
	virtual ~RandomName();
};


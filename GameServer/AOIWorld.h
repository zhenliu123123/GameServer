#pragma once
#include<list>
#include<vector>
//ӵ�к�������Ķ���
class Player
{
public:
	virtual int GetX()=0;
	virtual int GetY()=0;
private:
	int x=0;
	int y=0;
};
class Grid 
{
public:
	std::list<Player*> mPlayersList;
};
class AOIWorld
{
public:
	AOIWorld(int _xBegin,int _xEnd,int _yBegin,int _yEnd,int _xCount,int _yCount);
	virtual ~AOIWorld();
	std::vector<Grid> mGridsVector;
	//��ȡ��Χ���
	std::list<Player*> GetSurroundPlayers(Player* _player);
	//�����ҵ�AOI����
	bool AddPlayer(Player* _player);
	//ժ�����
	void DelPlayer(Player* _player);
private:
	int xBegin = 0;
	int xEnd = 0;
	int yBegin = 0;
	int yEnd = 0;
	int xCount = 0;//x��ֳɼ���
	int yCount = 0;//y��ֳɼ���
	int xWidth = 0;
	int yWidth = 0;
};


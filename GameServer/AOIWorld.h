#pragma once
#include<list>
#include<vector>
//拥有横纵坐标的对象
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
	//获取周围玩家
	std::list<Player*> GetSurroundPlayers(Player* _player);
	//添加玩家到AOI网格
	bool AddPlayer(Player* _player);
	//摘除玩家
	void DelPlayer(Player* _player);
private:
	int xBegin = 0;
	int xEnd = 0;
	int yBegin = 0;
	int yEnd = 0;
	int xCount = 0;//x轴分成几份
	int yCount = 0;//y轴分成几份
	int xWidth = 0;
	int yWidth = 0;
};


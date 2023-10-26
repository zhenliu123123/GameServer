#include "AOIWorld.h"

AOIWorld::AOIWorld(int _xBegin, int _xEnd, int _yBegin, int _yEnd, int _xCount, int _yCount)
	:xBegin(_xBegin),xEnd(_xEnd),yBegin(_yBegin),yEnd(_yEnd),xCount(_xCount),yCount(_yCount)
{
	//x轴网格宽度=(x轴结束坐标-x轴起始坐标)/x轴网格数量
	xWidth = (xEnd - xBegin) / xCount;
	yWidth = (yEnd = yBegin) / yCount;
	//创建格子容器
	for (int i = 0; i < xCount * yCount; i++) 
	{
		Grid tmp;
		mGridsVector.push_back(tmp);
	}
}

AOIWorld::~AOIWorld()
{
}

std::list<Player*> AOIWorld::GetSurroundPlayers(Player* _player)
{
	std::list<Player*> listRet;
	//计算网格编号
	int gridID = (_player->GetX() - xBegin) / xWidth + (_player->GetY() - yBegin) / yWidth * xCount;
	//x的位序：从左往右数的第几个
	int xIndex = 0;
	//y的位序：从上往下数的第几个
	int yIndex = 0;
	//左上角格子
	if (xIndex > 0 && yIndex > 0) {
		//把左上角格子里的玩家列表插入到listRet并返回
		std::list<Player*> curPlayerList=mGridsVector[gridID-1-xCount].mPlayersList;
		listRet.insert(listRet.begin(), curPlayerList.begin(), curPlayerList.end());
	}
	//正上方格子
	if (yIndex > 0) {
		//把正上方格子里的玩家列表插入到listRet并返回
		std::list<Player*> curPlayerList = mGridsVector[gridID - xCount].mPlayersList;
		listRet.insert(listRet.begin(), curPlayerList.begin(), curPlayerList.end());
	}
	//右上方格子
	if (xIndex <= xCount && yIndex > 0) {
		std::list<Player*> curPlayerList = mGridsVector[gridID + 1 - xCount].mPlayersList;
		listRet.insert(listRet.begin(), curPlayerList.begin(), curPlayerList.end());
	}
	//正左边格子
	if (xIndex > 0) {
		std::list<Player*> curPlayerList = mGridsVector[gridID - 1].mPlayersList;
		listRet.insert(listRet.begin(), curPlayerList.begin(), curPlayerList.end());
	}
	//正右边格子
	return listRet;
}

bool AOIWorld::AddPlayer(Player* _player)
{
	//网格编号=(x-x轴起始坐标)/x轴网格宽度 + (y-y轴起始坐标)/y轴宽度*x轴网格数量 
	//计算网格编号
	int gridID = (_player->GetX() - xBegin)/xWidth+(_player->GetY()-yBegin)/yWidth*xCount;
	//添加到网格中
	mGridsVector[gridID].mPlayersList.push_back(_player);
	return true;
}

void AOIWorld::DelPlayer(Player* _player)
{
	//计算网格编号
	int gridID = (_player->GetX() - xBegin) / xWidth + (_player->GetY() - yBegin) / yWidth * xCount;
	//摘除玩家
	mGridsVector[gridID].mPlayersList.remove(_player);

}

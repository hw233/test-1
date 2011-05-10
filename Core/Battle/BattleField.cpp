#include "Config.h"
#include "GObject/Country.h"
#include "BattleField.h"

namespace Battle
{

BattleField::BattleField()
{
	memset(_objs, 0, sizeof(_objs));
	memset(_isBody, 0, sizeof(_isBody));
	memset(_formation, 0, sizeof(_formation));
}

BattleField::~BattleField()
{
	clear();
}

void BattleField::setObject( int side, int idx, BattleObject * obj, UInt8 isBody )
{
	if(_objs[side][idx] != NULL)
	{
		if(!_isBody[side][idx])
			delete _objs[side][idx];
		_objs[side][idx] = NULL;
	}
	if(_formation[side] != NULL && obj->getClass() == BattleObject::Char)
	{
		int c = _formation[side]->getGridCount();
		for(int i = 0; i < c; ++ i)
		{
			if(idx == (*_formation[side])[i].pos)
			{
				BattleFighter * fgt = static_cast<BattleFighter *>(obj);
				fgt->setFormationEffect(&(*_formation[side])[i]);
			}
		}
	}
	_objs[side][idx] = obj;
	_isBody[side][idx] = isBody;
	if(!isBody && obj->getClass() == BattleObject::Char)
	{
		BattleFighter * bfgt = static_cast<BattleFighter *>(obj);
		GObject::Fighter * fgt_ = bfgt->getFighter();
		if(!fgt_->isNpc())
			return;
		GObject::Fighter * fgt = static_cast<GObject::Fighter *>(fgt_);
		if(!fgt->extraPos.empty())
		{
			Int8 x = idx % 5;
			Int8 y = idx / 5;
			for(std::vector<GObject::Fighter::Offset>::iterator iter = fgt->extraPos.begin(); iter != fgt->extraPos.end(); ++ iter)
			{
				int nx = x + iter->x;
				int ny = y + iter->y;
				setObjectXY(1, nx, ny, bfgt, true, x, y);
			}
		}
	}
}

void BattleField::setObjectXY( int side, int x, int y, BattleObject * obj, bool isBody, UInt8 isBodyX, UInt8 isBodyY )
{
	if(isBodyX > 4 || isBodyY > 4)
		isBodyX = isBodyY = 0;
	if(isBody)
		setObject(side, x + y * 5, obj, isBodyX + isBodyY * 5 + 1);
	else
		setObject(side, x + y * 5, obj, 0);
}

BattleObject * BattleField::operator()( int side, int idx )
{
	return _objs[side][idx];
}

BattleObject * BattleField::getObjectXY( int side, int x, int y )
{
	return (*this)(side, x + y * 5);
}

int BattleField::getPossibleTarget( int side, int idx )
{
	static int select_table[5][5] = {
		{0 , 1 , 2 , 3 , 4},
		{1 , 0 , 2 , 3 , 4},
		{2 , 1 , 3 , 0 , 4},
		{3 , 2 , 4 , 1 , 0},
		{4 , 3 , 2 , 1 , 0},
	};
	int targetidx = 4 - (idx % 5);
	int tside = 1 - side;
	int * tbl = select_table[targetidx];
	for(int i = 0; i < 5; ++ i)
	{
		for(int j = 0; j < 5; ++ j)
		{
			int tidx = tbl[j] + i * 5;
			if(_objs[tside][tidx] != NULL && _objs[tside][tidx]->getHP() > 0)
			{
				return tidx;
			}
		}
	}
	return -1;
}

bool BattleField::anyObjectInRow( int side, int row )
{
	int start = row * 5, end = start + 5;
	for(int p = start; p < end; ++ p)
	{
		if(_objs[side][p] != NULL && _objs[side][p]->getHP() > 0)
			return true;
	}
	return false;
}

void BattleField::setFormation( int idx, UInt32 id )
{
	_formation[idx] = GData::formationManager[id];
}

void BattleField::getFormationPositions( int idx, std::vector<UInt8>& poslist )
{
	if(_formation[idx] == NULL)
		return;
	int gridCount = _formation[idx]->getGridCount();
	for(int i = 0; i < gridCount; ++ i)
	{
		poslist.push_back((*_formation[idx])[i].pos);
	}
}

UInt32 BattleField::getAliveCount( int side )
{
	UInt32 r = 0;
	for(int i = 0; i < 25; ++ i)
	{
		BattleObject * obj = _objs[side][i];
		if(obj != NULL && obj->getClass() == BattleObject::Char &&
			obj->getHP() > 0)
		{
			++ r;
		}
	}
	return r;
}

UInt32 BattleField::getObjHp( int side )
{
	UInt32 r = 0;
	for (int i = 0; i < 25; ++ i)
	{
		BattleObject * obj = _objs[side][i];
		if (obj != NULL)
			r += obj->getHP();
	}
	return r;
}

UInt32 BattleField::getMaxObjHp( int side )
{
	UInt32 r = 0;
	for (int i = 0; i < 25; ++ i)
	{
		BattleFighter * obj = static_cast<BattleFighter *>(_objs[side][i]);
		if (obj != NULL)
			r += obj->getMaxHP();
	}
	return r;
}

void BattleField::clear()
{
	for(int i = 0; i < 2; ++ i)
		for(int j = 0; j < 25; ++ j)
			if(_objs[i][j] && !_isBody[i][j])
				delete _objs[i][j];
	memset(_objs, 0, sizeof(_objs));
	memset(_formation, 0, sizeof(_formation));
}

void BattleField::reset()
{
	for(int i = 0; i < 2; ++ i)
		for(int j = 0; j < 25; ++ j)
			if(_objs[i][j] && _objs[i][j]->getClass() == BattleObject::Char)
			{
				BattleFighter * bf = static_cast<BattleFighter *>(_objs[i][j]);
				bf->setHP(bf->getMaxHP());
			}
}

void BattleField::updateStats( int side )
{
	for(int i = 0; i < 2; ++ i)
		for(int j = 0; j < 25; ++ j)
			if(_objs[i][j] && _objs[i][j]->getClass() == BattleObject::Char)
			{
				BattleFighter * bf = static_cast<BattleFighter *>(_objs[i][j]);
				bf->setFormationEffect(NULL);
				bf->updateAllAttr();
			}
}

void BattleField::updateStats( int side, int pos )
{
	if(_objs[side][pos] && _objs[side][pos]->getClass() == BattleObject::Char)
	{
		BattleFighter * bf = static_cast<BattleFighter *>(_objs[side][pos]);
		bf->updateAllAttr();
	}
}

}

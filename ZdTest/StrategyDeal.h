/*
������Ե���
*/
#pragma once
#include "SpreadStgy.h"
#include "ZdSpi.h"


class StrategyDeal
{
public:
	StrategyDeal();
	~StrategyDeal();



	//���ز���
	void LoadStrategy(char* StgyPath);
	//��ȡ���Ե�������Ϣ
	void GetStrategyConfig(char* StgyPath);
	//�޸Ĳ�������
	void SetStrategyConfig(char* Key, char* value);

	

private:
	SpreadStgy* SpStgy;

};




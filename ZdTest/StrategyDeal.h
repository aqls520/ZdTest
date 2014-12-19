/*
处理策略的类
*/
#pragma once
#include "SpreadStgy.h"
#include "ZdSpi.h"


class StrategyDeal
{
public:
	StrategyDeal();
	~StrategyDeal();



	//加载策略
	void LoadStrategy(char* StgyPath);
	//获取策略的配置信息
	void GetStrategyConfig(char* StgyPath);
	//修改策略配置
	void SetStrategyConfig(char* Key, char* value);

	

private:
	SpreadStgy* SpStgy;

};




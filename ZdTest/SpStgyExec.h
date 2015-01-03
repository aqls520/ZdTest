#pragma once

#include "ZdStruct.h"
#include <map>
#include <vector>
#include <windows.h>
using namespace std;

typedef void* LPVOID;

class SpreadStgy;

class SpStgyExec
{
public:
	SpStgyExec();
	~SpStgyExec();

	
public:
	//给策略调用的接口
	void SubMarketData(StgyConfig);
	void UpLoadStgyCfg(StgyConfig);
	void SendSpOrder(SpOrder spod);

private:
	//接口封装初始化
	void InitSpi();

	//行情推给策略
	void OnTick(SpTick t);
	//报单变化了就推给策略
	void OnOrder(SpOrder);

	void ExecAOrder(SpOrder);

	//预留接口
	//策略配置更新
	void UpdateStgyCfg(StgyConfig* aStygCfg);

		
private:
	StgyConfig m_MyStgyCfg;
	LPVOID pTradeSpiAct;
	LPVOID pTradeSpiPas;
	map<string,Tick> m_mInstTick;
	SpreadStgy* m_SpStgy;
	vector<SpOrder> m_vSpOdList;
	SpOrder m_CurSpOrder;
	CRITICAL_SECTION m_cs;
	HANDLE m_Event;
	SpTick m_curSpTick;
	vector<SpTick> m_vSpTickL;

};


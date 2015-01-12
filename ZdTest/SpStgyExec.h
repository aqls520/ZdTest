#pragma once

#include "ZdStruct.h"
#include <map>
#include <vector>
#include <windows.h>
#include "Lib_dll/ThostFtdcUserApiStruct.h"
#include "CtpQtSpi.h"
#include "CtpTdSpi.h"
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
	void ReqConnect();
	void RegisterExec(SpreadStgy* pSpStgy);
	void SubMarketData(StgyConfig);
	void UpLoadStgyCfg(StgyConfig);
	void SendSpOrder(SpOrder spod);

	//接收行情，推给策略
	void OnCtpRtnTick(CThostFtdcDepthMarketDataField *pDepthMarketData);
	//接收成交回报，推给策略
	void OnCtpRtnTrade(CThostFtdcTradeField* pTrade);
	void OnCtpRtnOrder(CThostFtdcOrderField* pOrder);
private:
	//接口封装初始化
	void InitSpi();

	
	//报单变化了就推给策略
	void OnOrder(SpOrder);
	void ExecAOrder(SpOrder);

	//报单适配器
	void OrderAdapter(SpOrder);
	void OrderAdapter(CtpSpOrder spod);

	//预留接口
	//策略配置更新
	void UpdateStgyCfg(StgyConfig* aStygCfg);

	void SetInstCode(StgyConfig* aStygCfg);

		
private:
	StgyConfig m_MyStgyCfg;
	LPVOID pTradeSpiAct;
	LPVOID pTradeSpiPas;
	CtpQtSpi* pQtSpi;
	CtpTdSpi* pTdSpi;
	map<string,CThostFtdcDepthMarketDataField> m_mInstTick;
	SpreadStgy* m_SpStgy;
	vector<SpOrder> m_vSpOdList;
	vector<CtpSpOrder> m_vCtpSpOdList;
	SpOrder m_CurSpOrder;
	CRITICAL_SECTION m_cs;
	HANDLE m_Event;
	SpTick m_curSpTick;
	vector<SpTick> m_vSpTickL;
	string ActiveInstCode;
	string PassiveInstCode;
	

};


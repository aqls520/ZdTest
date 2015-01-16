#pragma once

#include "ZdStruct.h"
#include <map>
#include <vector>
#include <queue>
#include <windows.h>
using namespace std;

typedef void* LPVOID;

class SpreadStgy;
class CtpQtSpi;
class CtpTdSpi;


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
	void SendSpOrder(CtpSpOrder spod);
	bool CheckOrder(CtpSpOrder spod);
	void ExecAOrder(CtpSpOrder);

	//接收行情，推给策略
	void OnCtpRtnTick(CThostFtdcDepthMarketDataField *pDepthMarketData);
	//接收成交回报，推给策略
	void OnCtpRtnTrade(CThostFtdcTradeField* pTrade);
	void OnCtpRtnOrder(CThostFtdcOrderField* pOrder);
private:
	//接口封装初始化
	void InitSpi();

	
	//报单变化了就推给策略
	void OnOrder(CtpSpOrder);
	

	//报单适配器
	void OrderAdapter(CtpSpOrder spod);

	//价差拆单
	vector<ComOrder> SplitSpOrder(CtpSpOrder spod);
	

	//预留接口
	//策略配置更新
	void UpdateStgyCfg(StgyConfig* aStygCfg);

	void SetInstCode(StgyConfig* aStygCfg);

		

public:
	
	LPVOID pTradeSpiAct;
	LPVOID pTradeSpiPas;
	CtpQtSpi* pQtSpi;
	CtpTdSpi* pTdSpi;
	map<string,CThostFtdcDepthMarketDataField> m_mInstTick;
	SpreadStgy* m_SpStgy;
	
	CtpSpOrder m_CurSpOrder;
	CRITICAL_SECTION m_cs;
	
	SpTick m_curSpTick;
	vector<SpTick> m_vSpTickL;
	string ActiveInstCode;
	string PassiveInstCode;
	


	vector<CtpSpOrder> m_vSpOdList;
	vector<CtpSpOrder> m_vNotExecSpOd;
	vector<CtpSpOrder> m_vAllSpOd;
	vector<CtpSpOrder> m_vExecSpOd;
	HANDLE m_Event, m_MDEvent;
	StgyConfig m_MyStgyCfg;

};


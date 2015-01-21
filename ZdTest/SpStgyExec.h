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
	void ExecAOrder(CtpSpOrder* pSpod);
	//查询慎用，ctp有流控，1s只能查1次，最好初始化的时候进行查询
	vector<CThostFtdcInvestorPositionField> GetCTPCurPosition();
	CThostFtdcTradingAccountField GetCTPCurAccoutMoney();


	void OnCtpRtnTick(CThostFtdcDepthMarketDataField *pDepthMarketData);
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
	int GetPosition(string Inst);

	//预留接口
	//策略配置更新
	void UpdateStgyCfg(StgyConfig* aStygCfg);

	void SetInstCode(StgyConfig* aStygCfg);
	ComOrder GetActOrder(CtpSpOrder spod);
	ComOrder GetPasOrder(CtpSpOrder spod);
	char CheckOrderOffset(ComOrder od);
public:
	
	LPVOID pTradeSpiAct;
	LPVOID pTradeSpiPas;
	CtpQtSpi* pQtSpi;
	CtpTdSpi* pTdSpi;
	map<string,CThostFtdcDepthMarketDataField> m_mInstTick;
	SpreadStgy* m_SpStgy;
	
	CtpSpOrder m_CurSpOrder;
	CRITICAL_SECTION m_cs,m_qSpOdCS;
	
	SpTick m_curSpTick;
	vector<SpTick> m_vSpTickL;
	string ActiveInstCode;
	string PassiveInstCode;
	
	map<string, int> m_InstPos;

	vector<CtpSpOrder> m_vAllSpOd;
	HANDLE m_Event, m_MDEvent,m_NewOrderEvent;
	StgyConfig m_MyStgyCfg;
	queue<CtpSpOrder> m_qSpOrder;

};


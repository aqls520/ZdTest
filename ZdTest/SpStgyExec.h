#pragma once

#include "ZdStruct.h"
#include <map>
#include <vector>
#include <queue>
#include <windows.h>
#include "Log.h"
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
	Formula ParseMathFormula(string strMathF);
	void SendSpOrder(CtpSpOrder spod);
	ErrInfo CancelSpOrder(int SpOrderRef);//主动撤单
	bool CheckOrderTouch(CtpSpOrder spod);
	void ExecAOrder(CtpSpOrder* pSpod);
	void CancelAOrder(CtpSpOrder* pSpod);
	//查询慎用，ctp有流控，1s只能查1次，最好初始化的时候进行查询
	vector<CThostFtdcInvestorPositionField> GetCTPCurPosition();
	CThostFtdcTradingAccountField GetCTPCurAccoutMoney();


	void OnCtpRtnTick(CThostFtdcDepthMarketDataField *pDepthMarketData);
	void OnCtpRtnTrade(CThostFtdcTradeField* pTrade);
	void OnCtpRtnOrder(CThostFtdcOrderField* pOrder);
	
	void Init();//模块初始化
private:
	
	void InitSpi();//接口封装初始化
	void OrderAdapter(CtpSpOrder spod);//报单适配器
	SpTick CalCurSpTick();//根据公式，计算当前tick
	//价差拆单
	vector<ComOrder> SplitSpOrder(CtpSpOrder spod);
	int GetPosition(string Inst);
	void UpdateStgyCfg(StgyConfig* aStygCfg);
	ComOrder GetActOrder(CtpSpOrder spod);
	ComOrder GetPasOrder(CtpSpOrder spod);
	char CheckOrderOffset(ComOrder od);
public:
	
	CtpQtSpi* pQtSpi;
	CtpTdSpi* pTdSpi;
	map<string,CThostFtdcDepthMarketDataField> m_mInstTick;
	SpreadStgy* m_SpStgy;
	
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
	Log* m_log;
};


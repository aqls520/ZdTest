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
	SpStgyExec(SpreadStgy* pSpStgy);
	~SpStgyExec();

public:
	//给策略调用的接口
	void SendSpOrder(CtpSpOrder spod);
	ErrInfo CancelSpOrder(int SpOrderRef);//主动撤单
	void SubMarketData(StgyConfig);
	void UpLoadStgyCfg(StgyConfig);
	Formula ParseMathFormula(string strMathF);
	
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
	int UpdateInstPos(CThostFtdcTradeField* pTrade);
private:
	
	void InitSpi();//接口封装初始化
	void OrderAdapter(CtpSpOrder spod);//报单适配器
	SpTick CalCurSpTick();//根据公式，计算当前tick

	ComOrder GetActOrder(CtpSpOrder spod);
	ComOrder GetPasOrder(CtpSpOrder spod);
	char CheckOrderOffset(ComOrder od);
	int GetInstPos(string Inst, char Dir);

private:
	

public:
	CtpQtSpi* pQtSpi;
	CtpTdSpi* pTdSpi;

	SpreadStgy* m_SpStgy;
	StgyConfig m_StgyCfg;

	string ActInstCode;
	string PasInstCode;

	SpTick m_curSpTick;
	vector<SpTick> m_vSpTickL;
	vector<CtpSpOrder> m_vAllSpOd;
	map<string, CThostFtdcDepthMarketDataField> m_mInstTick;

	vector<CThostFtdcInvestorPositionField> m_CtpPosition;
	CThostFtdcTradingAccountField m_CtpActInfo;
	vector<InstPos> m_InstPos;
	queue<CtpSpOrder> m_qSpOrder;

	Log* m_log;

	CRITICAL_SECTION m_cs, m_qSpOdCS;
	HANDLE m_Event, m_MDEvent, m_NewOrderEvent;
};


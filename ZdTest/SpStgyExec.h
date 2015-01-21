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
	//�����Ե��õĽӿ�
	void ReqConnect();
	void RegisterExec(SpreadStgy* pSpStgy);
	void SubMarketData(StgyConfig);
	void UpLoadStgyCfg(StgyConfig);
	void SendSpOrder(CtpSpOrder spod);
	bool CheckOrder(CtpSpOrder spod);
	void ExecAOrder(CtpSpOrder* pSpod);
	//��ѯ���ã�ctp�����أ�1sֻ�ܲ�1�Σ���ó�ʼ����ʱ����в�ѯ
	vector<CThostFtdcInvestorPositionField> GetCTPCurPosition();
	CThostFtdcTradingAccountField GetCTPCurAccoutMoney();


	void OnCtpRtnTick(CThostFtdcDepthMarketDataField *pDepthMarketData);
	void OnCtpRtnTrade(CThostFtdcTradeField* pTrade);
	void OnCtpRtnOrder(CThostFtdcOrderField* pOrder);
	
private:
	//�ӿڷ�װ��ʼ��
	void InitSpi();

	
	//�����仯�˾��Ƹ�����
	void OnOrder(CtpSpOrder);
	

	//����������
	void OrderAdapter(CtpSpOrder spod);

	//�۲��
	vector<ComOrder> SplitSpOrder(CtpSpOrder spod);
	int GetPosition(string Inst);

	//Ԥ���ӿ�
	//�������ø���
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


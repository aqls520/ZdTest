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
	void ExecAOrder(CtpSpOrder);

	//�������飬�Ƹ�����
	void OnCtpRtnTick(CThostFtdcDepthMarketDataField *pDepthMarketData);
	//���ճɽ��ر����Ƹ�����
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
	

	//Ԥ���ӿ�
	//�������ø���
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


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
	//�����Ե��õĽӿ�
	void ReqConnect();
	void RegisterExec(SpreadStgy* pSpStgy);
	void SubMarketData(StgyConfig);
	void UpLoadStgyCfg(StgyConfig);
	void SendSpOrder(SpOrder spod);

	//�������飬�Ƹ�����
	void OnCtpRtnTick(CThostFtdcDepthMarketDataField *pDepthMarketData);
	//���ճɽ��ر����Ƹ�����
	void OnCtpRtnTrade(CThostFtdcTradeField* pTrade);
	void OnCtpRtnOrder(CThostFtdcOrderField* pOrder);
private:
	//�ӿڷ�װ��ʼ��
	void InitSpi();

	
	//�����仯�˾��Ƹ�����
	void OnOrder(SpOrder);
	void ExecAOrder(SpOrder);

	//����������
	void OrderAdapter(SpOrder);
	void OrderAdapter(CtpSpOrder spod);

	//Ԥ���ӿ�
	//�������ø���
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


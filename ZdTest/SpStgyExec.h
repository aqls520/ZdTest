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
	//�����Ե��õĽӿ�
	void ReqConnect();
	void RegisterExec(SpreadStgy* pSpStgy);
	void SubMarketData(StgyConfig);
	void UpLoadStgyCfg(StgyConfig);
	Formula ParseMathFormula(string strMathF);
	void SendSpOrder(CtpSpOrder spod);
	ErrInfo CancelSpOrder(int SpOrderRef);//��������
	bool CheckOrderTouch(CtpSpOrder spod);
	void ExecAOrder(CtpSpOrder* pSpod);
	void CancelAOrder(CtpSpOrder* pSpod);
	//��ѯ���ã�ctp�����أ�1sֻ�ܲ�1�Σ���ó�ʼ����ʱ����в�ѯ
	vector<CThostFtdcInvestorPositionField> GetCTPCurPosition();
	CThostFtdcTradingAccountField GetCTPCurAccoutMoney();


	void OnCtpRtnTick(CThostFtdcDepthMarketDataField *pDepthMarketData);
	void OnCtpRtnTrade(CThostFtdcTradeField* pTrade);
	void OnCtpRtnOrder(CThostFtdcOrderField* pOrder);
	
	void Init();//ģ���ʼ��
private:
	
	void InitSpi();//�ӿڷ�װ��ʼ��
	void OrderAdapter(CtpSpOrder spod);//����������
	SpTick CalCurSpTick();//���ݹ�ʽ�����㵱ǰtick
	//�۲��
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


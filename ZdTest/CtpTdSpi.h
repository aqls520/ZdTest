#pragma once
#include "Lib_dll/ThostFtdcTraderApi.h"
#include <map>
#include <vector>
#include <string>
#include <windows.h>
#include "ZdDefine.h"
using namespace std;

typedef CThostFtdcTraderApi CtpTdApi;

class SpStgyExec;

class CtpTdSpi : public CThostFtdcTraderSpi
{
public:
	CtpTdSpi();
	~CtpTdSpi();

#pragma region Api�ص�

public:
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///�ͻ�����֤��Ӧ
	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����¼��������Ӧ
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Ԥ��¼��������Ӧ
	virtual void OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Ԥ�񳷵�¼��������Ӧ
	virtual void OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������������Ӧ
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Ͷ���߽�����ȷ����Ӧ
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	
	///�����ѯ������Ӧ
	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ�ɽ���Ӧ
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯͶ���ֲ߳���Ӧ
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ�ʽ��˻���Ӧ
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ��Լ��֤������Ӧ
	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ��Լ����������Ӧ
	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ��Լ��Ӧ
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯͶ���ֲ߳���ϸ��Ӧ
	virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����֪ͨ
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///�ɽ�֪ͨ
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

	///����¼�����ر�
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);

	///������������ر�
	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);

	///��Լ����״̬֪ͨ
	virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus);

	///����֪ͨ
	virtual void OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo);


#pragma endregion

public:
	void ReqUserLogin();
	void ReqSettleInfoConfirm();
	void ReqQryTradingAccount();
	void ReqQryTradingCode();
	void ReqQryOrder();
	void ReqQryTrade();
	void ReqQryIvstPosition();
	void ReqQryIvstPositionDetail();
	void ReqQryInstMarginRate(TThostFtdcInstrumentIDType InstrumentID);
	void ReqQryInstrument(TThostFtdcInstrumentIDType InstrumentID);
	void ReqQryBrokerTradingParas();
	void ReqQrySettlementInfo(TThostFtdcDateType TradingDay);
	void ReqQryInstCommissionRate(TThostFtdcInstrumentIDType InstrumentID);
	
	//����
	void ReqOrdLimit(TThostFtdcInstrumentIDType	Inst, TThostFtdcPosiDirectionType dir, TThostFtdcOffsetFlagType offset,
		TThostFtdcVolumeType vol, TThostFtdcPriceType price);
	void ReqOrdAny(TThostFtdcInstrumentIDType	Inst, TThostFtdcPosiDirectionType dir, TThostFtdcOffsetFlagType offset,
		TThostFtdcVolumeType vol);
	void ReqOrdFOK(TThostFtdcInstrumentIDType	Inst, TThostFtdcPosiDirectionType dir, TThostFtdcOffsetFlagType offset,
		TThostFtdcVolumeType vol, TThostFtdcPriceType price);
	void ReqOrdFAK(TThostFtdcInstrumentIDType	Inst, TThostFtdcPosiDirectionType dir, TThostFtdcOffsetFlagType offset,
		TThostFtdcVolumeType vol, TThostFtdcPriceType price,TThostFtdcVolumeType minVol=1);
	void ReqOrdCondition(TThostFtdcInstrumentIDType	Inst, TThostFtdcPosiDirectionType dir, TThostFtdcOffsetFlagType offset,
		TThostFtdcVolumeType vol, TThostFtdcPriceType price);

	//����
	void ReqOrderCancelBySysID(TThostFtdcExchangeIDType ExchangeID, TThostFtdcOrderSysIDType OrdSysID);
	void ReqOrderCancelByOrdRef(TThostFtdcInstrumentIDType instId, TThostFtdcOrderRefType OrdRef);

	//��־��¼
	void ShowMessage(string msg);
	bool IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo);
public:
	//����ӿڣ���ʼ�����ͷ�
	void Init();
	void LoadTdCfg(char* cfgpath);
	void SetTdInfo(string brokerid, string investorid, string pwd, string sFrontAddr);
	void Release();
	void RegisterStgyExec(SpStgyExec* stgyexec);

public:
	ConnectStatus m_connStatus;
	LoginStatus m_loginStatus;

private:
	CtpTdApi* m_pTdApi;

	//-----------���׻���������Ϣ-----------------
	vector<string> m_vsFrtAddr;
	TThostFtdcBrokerIDType m_sBrkID;
	TThostFtdcInvestorIDType m_sIvstID;
	TThostFtdcPasswordType m_sPwd;
	TThostFtdcFrontIDType m_iFrontID;
	TThostFtdcSessionIDType m_iSessionID;
	int m_iOrderRef;
	int m_iReqNo;

	//-----------------------------
	vector<CThostFtdcOrderField> m_vOrdList;//ί���б�
	vector<CThostFtdcTradeField> m_vTrdList;//�ɽ��б�

	SpStgyExec* m_StgyExec;
	HANDLE m_Event;
	



};


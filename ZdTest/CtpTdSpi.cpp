#include "CtpTdSpi.h"
#include <fstream>
#include <windows.h>
#include "SpStgyExec.h"
using namespace std;

CtpTdSpi::CtpTdSpi()
{
	m_Event = CreateEvent(NULL,false,false,NULL);
	strcpy(m_sBrkID,"66666");
	strcpy(m_sIvstID,"901719");
	strcpy(m_sPwd,"018977");
}

CtpTdSpi::~CtpTdSpi()
{}

void CtpTdSpi::RegisterStgyExec(SpStgyExec* stgyexec)
{
	this->m_StgyExec = stgyexec;
}

#pragma region Api�ص�

///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CtpTdSpi::OnFrontConnected()
{
	m_connStatus = CTP_CONNECTED;
	ShowMessage("OnFrontConnected\n");
	ReqUserLogin();
}

///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
void CtpTdSpi::OnFrontDisconnected(int nReason)
{
	m_connStatus = CTP_DISCONNECTED;
	m_loginStatus = CTP_LOGOUT;
}

///��¼������Ӧ
void CtpTdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		m_loginStatus = CTP_LOGIN;
		m_iFrontID = pRspUserLogin->FrontID;
		m_iSessionID = pRspUserLogin->SessionID;
		m_iOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		ReqSettleInfoConfirm();
	}
}

///����¼��������Ӧ
void CtpTdSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	printf("OnRspOrderInsert-errormsg:%s\n",pRspInfo->ErrorMsg);
}

///Ԥ��¼��������Ӧ
void CtpTdSpi::OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///Ԥ�񳷵�¼��������Ӧ
void CtpTdSpi::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	printf("OnRspParkedOrderAction-errormsg:%s\n", pRspInfo->ErrorMsg);
}

///��������������Ӧ
void CtpTdSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	printf("OnRspOrderAction-errormsg:%s\n", pRspInfo->ErrorMsg);
}

///Ͷ���߽�����ȷ����Ӧ
void CtpTdSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//ReqQryTradingAccount();
}

///�����ѯ������Ӧ
void CtpTdSpi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (pOrder)
	{
		printf("InstID:%s\n",pOrder->InstrumentID);
	}
}

///�����ѯ�ɽ���Ӧ
void CtpTdSpi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pTrade)
	{
		printf("TradeID:%s,BID:%c\n", pTrade->TradeID, pTrade->OffsetFlag);
	}
}

///�����ѯͶ���ֲ߳���Ӧ
void CtpTdSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!pInvestorPosition) 
		return;
	m_vPosition.push_back(*pInvestorPosition);
	if (bIsLast)
		SetEvent(m_Event);
}

///�����ѯ�ʽ��˻���Ӧ
void CtpTdSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast)
	{
		m_TradeAccount = *pTradingAccount;
		SetEvent(m_Event);
	}

}

///�����ѯ��Լ��֤������Ӧ
void CtpTdSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pInstrumentMarginRate && !IsErrorRspInfo(pRspInfo))
	{
		printf("instid=%s,mrlm=%f,mrlv=%f,mrsm=%f,mrsv=%f\n", pInstrumentMarginRate->InstrumentID,
			pInstrumentMarginRate->LongMarginRatioByMoney,
			pInstrumentMarginRate->LongMarginRatioByVolume,
			pInstrumentMarginRate->ShortMarginRatioByMoney,
			pInstrumentMarginRate->ShortMarginRatioByVolume);
	}
}

///�����ѯ��Լ����������Ӧ
void CtpTdSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (pInstrumentCommissionRate)
	{
		printf("OpenRatio:", pInstrumentCommissionRate->OpenRatioByMoney);
	}
}
///�����ѯ��Լ��Ӧ
void CtpTdSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!bIsLast)
	{
		printf("1");
	}
	if (bIsLast)
	{
		printf(pInstrument->InstrumentID);
	}
	/*if (pInstrument && !IsErrorRspInfo(pRspInfo))
	{
		printf("mlr=%f,msr=%f\n", pInstrument->LongMarginRatio, pInstrument->ShortMarginRatio);
	}
	SetEvent(event);*/

}

///����֪ͨ
void CtpTdSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	m_StgyExec->OnCtpRtnOrder(pOrder);
}

///�ɽ�֪ͨ
void CtpTdSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	m_StgyExec->OnCtpRtnTrade(pTrade);
}

///����¼�����ر�
void CtpTdSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	printf("OnErrRtnOrderInsert-errormsg:%s\n", pRspInfo->ErrorMsg);
}

///������������ر�
void CtpTdSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	printf("OnErrRtnOrderAction-errormsg:%s\n", pRspInfo->ErrorMsg);
}

///��Լ����״̬֪ͨ
void CtpTdSpi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{

}

///�����ѯͶ���ֲ߳���ϸ��Ӧ
void CtpTdSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	printf("%s,%s,%f;%f,%f\n", pInvestorPositionDetail->InstrumentID,
		pInvestorPositionDetail->OpenDate,
		pInvestorPositionDetail->OpenPrice,
		pInvestorPositionDetail->CloseProfitByTrade,
		pInvestorPositionDetail->CloseProfitByDate);
}

///����Ӧ��
void CtpTdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	printf("OnRspError-errormsg:%s\n",pRspInfo->ErrorMsg);
}
///����֪ͨ
void CtpTdSpi::OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) {}


#pragma endregion

void CtpTdSpi::ShowMessage(string msg)
{
	printf(msg.c_str());
}

bool CtpTdSpi::IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo)
{
	return (pRspInfo) && (pRspInfo->ErrorID);
}

void CtpTdSpi::Init()
{
	m_iReqNo = 0;
	m_pTdApi = CtpTdApi::CreateFtdcTraderApi("log//");
	m_pTdApi->RegisterSpi(this);
	m_pTdApi->SubscribePrivateTopic(THOST_TERT_QUICK);
	m_pTdApi->SubscribePublicTopic(THOST_TERT_QUICK);
	m_pTdApi->RegisterFront("tcp://ctpfz1-front1.citicsf.com:51205");
	m_pTdApi->Init();
	while (m_loginStatus != CTP_LOGIN)
	{}
}
void CtpTdSpi::LoadTdCfg(char* cfgpath)
{
	fstream fin;
	fin.open(cfgpath, ios::in);
	while (!fin.eof())
	{
		string cfgdata;
		int idx_begin = 0;
		int idx_split = 0;
		getline(fin, cfgdata);
		if (cfgdata.length() < 1)
			continue;
		idx_split = cfgdata.find('=', idx_begin);
		string key = cfgdata.substr(idx_begin, idx_split);
		string keyvalue = cfgdata.substr(idx_split + 1, cfgdata.length());
		if (key == "FrontAddr")
			m_vsFrtAddr.push_back(keyvalue);
		if (key == "BrokerID")
			strcpy(m_sBrkID, keyvalue.c_str());
		if (key == "InvestorID")
			strcpy(m_sIvstID, keyvalue.c_str());
		if (key == "PassWord")
			strcpy(m_sPwd, keyvalue.c_str());
	}
	fin.close();
}

void CtpTdSpi::SetTdInfo(string brokerid, string investorid, string pwd, string sFrontAddr)
{
	strcpy(m_sBrkID, brokerid.c_str());
	strcpy(m_sIvstID, investorid.c_str());
	strcpy(m_sPwd, pwd.c_str());
	m_vsFrtAddr.push_back(sFrontAddr);
}

void CtpTdSpi::Release()
{
	m_pTdApi->RegisterSpi(NULL);
	m_pTdApi->Release();
	m_pTdApi = NULL;
}

void CtpTdSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sBrkID);
	strcpy(req.UserID, m_sIvstID);
	strcpy(req.Password, m_sPwd);

	int rlt = m_pTdApi->ReqUserLogin(&req, ++m_iReqNo);
}
void CtpTdSpi::ReqSettleInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sBrkID);
	strcpy(req.InvestorID, m_sIvstID);
	int iResult = m_pTdApi->ReqSettlementInfoConfirm(&req, ++m_iReqNo);
}

void CtpTdSpi::ReqQrySettlementInfo(TThostFtdcDateType TradingDay)
{
	CThostFtdcQrySettlementInfoField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sBrkID);
	strcpy(req.InvestorID, m_sIvstID);
	strcpy(req.TradingDay, TradingDay);

	int rlt = m_pTdApi->ReqQrySettlementInfo(&req, ++m_iReqNo);
}

CThostFtdcTradingAccountField CtpTdSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sBrkID);
	strcpy(req.InvestorID, m_sIvstID);
	int rlt = m_pTdApi->ReqQryTradingAccount(&req, ++m_iReqNo);

	WaitForSingleObject(m_Event, INFINITE);
	return m_TradeAccount;
}

void CtpTdSpi::ReqQryTradingCode()
{
	CThostFtdcQryTradingCodeField req;
	memset(&req, 0, sizeof(req));
	int rlt = m_pTdApi->ReqQryTradingCode(&req, ++m_iReqNo);
}

void CtpTdSpi::ReqQryOrder()
{
	CThostFtdcQryOrderField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sBrkID);
	strcpy(req.InvestorID, m_sIvstID);

	int rlt = m_pTdApi->ReqQryOrder(&req, ++m_iReqNo);
}
void CtpTdSpi::ReqQryTrade()
{
	CThostFtdcQryTradeField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sBrkID);
	strcpy(req.InvestorID, m_sIvstID);
	strcpy(req.TradeTimeStart, "20140701");

	int rlt = m_pTdApi->ReqQryTrade(&req, ++m_iReqNo);
}
vector<CThostFtdcInvestorPositionField> CtpTdSpi::ReqQryIvstPosition()
{
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sBrkID);
	strcpy(req.InvestorID, m_sIvstID);

	int rlt = m_pTdApi->ReqQryInvestorPosition(&req, ++m_iReqNo);
	m_vPosition.clear();

	WaitForSingleObject(m_Event, INFINITE);

	return m_vPosition;
}

void CtpTdSpi::ReqQryIvstPositionDetail()
{
	CThostFtdcQryInvestorPositionDetailField req;
	memset(&req, 0, sizeof(req));

	int rlt = m_pTdApi->ReqQryInvestorPositionDetail(&req, ++m_iReqNo);
}

void CtpTdSpi::ReqQryInstMarginRate(TThostFtdcInstrumentIDType InstrumentID)
{
	CThostFtdcQryInstrumentMarginRateField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sBrkID);
	strcpy(req.InvestorID, m_sIvstID);
	strcpy(req.InstrumentID, InstrumentID);
	req.HedgeFlag = THOST_FTDC_HF_Speculation;

	int rlt = m_pTdApi->ReqQryInstrumentMarginRate(&req, ++m_iReqNo);
}

void CtpTdSpi::ReqQryBrokerTradingParas()
{
	CThostFtdcQryBrokerTradingParamsField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sBrkID);
	strcpy(req.InvestorID, m_sIvstID);

	int rlt = m_pTdApi->ReqQryBrokerTradingParams(&req,++m_iReqNo);
}

void CtpTdSpi::ReqQryInstrument(TThostFtdcInstrumentIDType	InstrumentID)
{
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.InstrumentID, InstrumentID);

	int rlt = m_pTdApi->ReqQryInstrument(&req, ++m_iReqNo);
}

void CtpTdSpi::ReqQryInstCommissionRate(TThostFtdcInstrumentIDType InstrumentID)
{
	CThostFtdcQryInstrumentCommissionRateField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sBrkID);
	strcpy(req.InvestorID, m_sIvstID);
	strcpy(req.InstrumentID, InstrumentID);

	int rlt = m_pTdApi->ReqQryInstrumentCommissionRate(&req, ++m_iReqNo);
}

//�޼۵�
/*��Ҫ�ֶΣ�
BrokerID��InvestorID�����ޣ����޴�Ȩ�ޣ�
InstrumentID���Ҳ�����Լ��
LimitPrice���򱻳�ʼ��Ϊ0�����Բ���Ļ������۸���Ƶ�ͣ�壩
OrderPriceType��Direction��CombOffsetFlag��CombHedgeFlag��VolumeTotalOriginal��
TimeCondition��VolumeCondition��ContingentCondition��ForceCloseReason�������ֶ�����
*/
/*�Ǳ�Ҫ�ֶ�
OrderRef��ϵͳ�Զ���1��ʼ������API�˿��Բ���Ҫ�Լ�ά������Ҫ�������
�����Ǹ�ֵҲ���ԣ������������
���OrderRef��ͬ��һ������������һ���ͱ��ܾ���OnRtnOrder��
���OrderRef�ݼ�������ʾ�������ظ���������OnRspOI��OnErrOI����
*/

int CtpTdSpi::ReqOrdLimit(TThostFtdcInstrumentIDType	Inst, TThostFtdcPosiDirectionType dir, TThostFtdcOffsetFlagType offset,
	TThostFtdcVolumeType vol, TThostFtdcPriceType price)
{
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sBrkID);
	strcpy(req.InvestorID, m_sIvstID);
	strcpy(req.InstrumentID, Inst);
	TThostFtdcOrderRefType thisOrdRef;
	sprintf(thisOrdRef, "%d", ++m_iOrderRef);
	strcpy(req.OrderRef, thisOrdRef);
	//LimitPrice + price
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//�۸�����=�޼�	
	req.Direction = dir;  //��������	
	req.CombOffsetFlag[0] = offset; //��Ͽ�ƽ��־:����
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	  //���Ͷ���ױ���־
	req.LimitPrice = price;	//�۸�
	req.VolumeTotalOriginal = vol;	///����	
	req.TimeCondition = THOST_FTDC_TC_GFD;  //��Ч������:������Ч
	req.VolumeCondition = THOST_FTDC_VC_AV; //�ɽ�������:�κ�����
	req.ContingentCondition = THOST_FTDC_CC_Immediately;  //��������:����
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;	//ǿƽԭ��:��ǿƽ	

	//��2���ֶ���memset��ʱ���Զ���Ϊ0�ˣ������������ָ��Ҳ���Բ�ָ��
	req.IsAutoSuspend = 0;  //�Զ������־:��	
	req.UserForceClose = 0;   //�û�ǿ����־:��

	m_pTdApi->ReqOrderInsert(&req, ++m_iReqNo);

	return m_iOrderRef;

}
//�м۵�
void CtpTdSpi::ReqOrdAny(TThostFtdcInstrumentIDType	Inst, TThostFtdcPosiDirectionType dir, TThostFtdcOffsetFlagType offset,
	TThostFtdcVolumeType vol)
{
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sBrkID);
	strcpy(req.InvestorID, m_sIvstID);
	strcpy(req.InstrumentID, Inst);
	TThostFtdcOrderRefType thisOrdRef;
	sprintf(thisOrdRef, "%d", ++m_iOrderRef);
	strcpy(req.OrderRef, thisOrdRef);
	// AnyPrice + Price=0(���
	req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;//�۸�����=�޼�	
	req.Direction = dir;  //��������	
	req.CombOffsetFlag[0] = offset; //��Ͽ�ƽ��־:����
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	  //���Ͷ���ױ���־
	req.VolumeTotalOriginal = vol;	///����	
	req.TimeCondition = THOST_FTDC_TC_IOC;  //��Ч������:������Ч
	req.VolumeCondition = THOST_FTDC_VC_AV; //�ɽ�������:�κ�����
	//req.MinVolume = 1;	//��С�ɽ���:1	
	req.ContingentCondition = THOST_FTDC_CC_Immediately;  //��������:����
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;	//ǿƽԭ��:��ǿƽ	
	req.IsAutoSuspend = 0;  //�Զ������־:��	
	req.UserForceClose = 0;   //�û�ǿ����־:��

	m_pTdApi->ReqOrderInsert(&req, ++m_iReqNo);
}
/*
FOK(Fill Or Kill)ָ���ǽ���������Ч����������ΪTHOST_FTDC_TC_IOC,ͬʱ���ɽ�����������ΪTHOST_FTDC_VC_CV,��ȫ������.
FAK(Fill And Kill)ָ����ǽ���������Ч����ΪTHOST_FTDC_TC_IOC,ͬʱ,�ɽ���������ΪTHOST_FTDC_VC_AV,����������;

����,��FAKָ����,����ָ����С�ɽ���,����ָ����λ��������С�ɽ��������ϳɽ�,ʣ�ඩ����ϵͳ����,����ϵͳȫ������.����״����,
��Ч����������ΪTHOST_FTDC_TC_IOC,����������ΪTHOST_FTDC_VC_MV,ͬʱ�趨MinVolume�ֶ�.

Fill + Kill = IOC
Or = THOST_FTDC_VC_CV��ȫ��������
And = THOST_FTDC_VC_AV������������ ���� THOST_FTDC_VC_MV + MinVolume=N
*/
void CtpTdSpi::ReqOrdFOK(TThostFtdcInstrumentIDType	Inst, TThostFtdcPosiDirectionType dir, TThostFtdcOffsetFlagType offset,
	TThostFtdcVolumeType vol, TThostFtdcPriceType price)
{
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sBrkID);
	strcpy(req.InvestorID, m_sIvstID);
	strcpy(req.InstrumentID, Inst);
	TThostFtdcOrderRefType thisOrdRef;
	sprintf(thisOrdRef, "%d", ++m_iOrderRef);
	strcpy(req.OrderRef, thisOrdRef);

	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//�۸�����=�޼�	
	req.Direction = dir;  //��������	
	req.CombOffsetFlag[0] = offset; //��Ͽ�ƽ��־:����
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	  //���Ͷ���ױ���־
	req.LimitPrice = price;	//�۸�
	req.VolumeTotalOriginal = vol;	///����	
	req.TimeCondition = THOST_FTDC_TC_IOC;  //��Ч������:�����ɽ���������
	req.VolumeCondition = THOST_FTDC_VC_CV; //�ɽ�������:ȫ������
	req.ContingentCondition = THOST_FTDC_CC_Immediately;  //��������:����
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;	//ǿƽԭ��:��ǿƽ	
	req.IsAutoSuspend = 0;  //�Զ������־:��	
	req.UserForceClose = 0;   //�û�ǿ����־:��

	m_pTdApi->ReqOrderInsert(&req, ++m_iReqNo);
}
void CtpTdSpi::ReqOrdFAK(TThostFtdcInstrumentIDType	Inst, TThostFtdcPosiDirectionType dir, TThostFtdcOffsetFlagType offset,
	TThostFtdcVolumeType vol, TThostFtdcPriceType price,TThostFtdcVolumeType minVol)
{
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sBrkID);
	strcpy(req.InvestorID, m_sIvstID);
	strcpy(req.InstrumentID, Inst);
	TThostFtdcOrderRefType thisOrdRef;
	sprintf(thisOrdRef, "%d", ++m_iOrderRef);
	strcpy(req.OrderRef, thisOrdRef);
	
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//�۸�����=�޼�	
	req.Direction = dir;  //��������	
	req.CombOffsetFlag[0] = offset; //��Ͽ�ƽ��־:����
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	  //���Ͷ���ױ���־
	req.LimitPrice = price;	//�۸�
	req.VolumeTotalOriginal = vol;	///����	
	req.TimeCondition = THOST_FTDC_TC_IOC;  //��Ч������:�����ɽ���������
	req.VolumeCondition = THOST_FTDC_VC_AV;
	req.MinVolume = minVol;	//��С�ɽ���:1	
	req.ContingentCondition = THOST_FTDC_CC_Immediately;  //��������:����
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;	//ǿƽԭ��:��ǿƽ	
	req.IsAutoSuspend = 0;  //�Զ������־:��	
	req.UserForceClose = 0;   //�û�ǿ����־:��

	m_pTdApi->ReqOrderInsert(&req, ++m_iReqNo);
}

//������
void CtpTdSpi::ReqOrdCondition(TThostFtdcInstrumentIDType	Inst, TThostFtdcPosiDirectionType dir, TThostFtdcOffsetFlagType offset,
	TThostFtdcVolumeType vol, TThostFtdcPriceType price)
{

}

//����
void CtpTdSpi::ReqOrderCancelBySysID(TThostFtdcExchangeIDType ExchangeID, TThostFtdcOrderSysIDType OrdSysID)
{
	//BrokerID + InvestorID + ExchangeID + OrderSysID
	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sBrkID);   //���͹�˾����	
	strcpy(req.InvestorID, m_sIvstID); //Ͷ���ߴ���
	strcpy(req.ExchangeID, ExchangeID);
	strcpy(req.OrderSysID, OrdSysID);
	req.ActionFlag = THOST_FTDC_AF_Delete;  //������־ 

	m_pTdApi->ReqOrderAction(&req, ++m_iReqNo);
}

void CtpTdSpi::ReqOrderCancelByOrdRef(TThostFtdcInstrumentIDType instId, TThostFtdcOrderRefType OrdRef)
{
	// FrontID + SessionID + OrdRef + InstrumentID
	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.InstrumentID, instId); //��Լ����
	strcpy(req.OrderRef, OrdRef); //��������	
	req.FrontID = m_iFrontID;           //ǰ�ñ��	
	req.SessionID = m_iSessionID;       //�Ự���
	req.ActionFlag = THOST_FTDC_AF_Delete;  //������־ 

	m_pTdApi->ReqOrderAction(&req, ++m_iReqNo);
}

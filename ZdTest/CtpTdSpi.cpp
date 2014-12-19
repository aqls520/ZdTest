#include "CtpTdSpi.h"
#include <fstream>
#include <windows.h>
using namespace std;

extern HANDLE event;

CtpTdSpi::CtpTdSpi()
{}

CtpTdSpi::~CtpTdSpi()
{}

#pragma region Api�ص�

///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CtpTdSpi::OnFrontConnected()
{
	ShowMessage("OnFrontConnected\n");
	ReqUserLogin();
}

///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void CtpTdSpi::OnFrontDisconnected(int nReason){}

///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
void CtpTdSpi::OnHeartBeatWarning(int nTimeLapse){}

///�ͻ�����֤��Ӧ
void CtpTdSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}


///��¼������Ӧ
void CtpTdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("user:%s\n", pRspUserLogin->UserID);
	/*if (IsErrorRspInfo(pRspInfo))
	{
		printf(pRspInfo->ErrorMsg);
	}
	else
		printf("������ȷ");*/

	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		m_iOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		ReqSettleInfoConfirm();
	}
}

///�ǳ�������Ӧ
void CtpTdSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�û��������������Ӧ
void CtpTdSpi::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�ʽ��˻��������������Ӧ
void CtpTdSpi::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

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

///��ѯ��󱨵�������Ӧ
void CtpTdSpi::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///Ͷ���߽�����ȷ����Ӧ
void CtpTdSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
}

///ɾ��Ԥ����Ӧ
void CtpTdSpi::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///ɾ��Ԥ�񳷵���Ӧ
void CtpTdSpi::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///ִ������¼��������Ӧ
void CtpTdSpi::OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///ִ���������������Ӧ
void CtpTdSpi::OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///ѯ��¼��������Ӧ
void CtpTdSpi::OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///����¼��������Ӧ
void CtpTdSpi::OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///���۲���������Ӧ
void CtpTdSpi::OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

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
	printf("��Լ:%s\n", pInvestorPosition->InstrumentID);
	printf("�ֲֶ��:%c\n", pInvestorPosition->PosiDirection);
	printf("�ֲ�����:%c\n", pInvestorPosition->PositionDate);
	printf("���ճֲ�:%d\n", pInvestorPosition->YdPosition);
	printf("���ճֲ�:%d\n", pInvestorPosition->Position);
	printf("���ճֲ�Td:%d\n", pInvestorPosition->TodayPosition);
	printf("\n");
	if (bIsLast)
	{
		int a = 1;
	}
}

///�����ѯ�ʽ��˻���Ӧ
void CtpTdSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf(pTradingAccount->AccountID);
	if (bIsLast)
	{
		printf(pTradingAccount->AccountID);
	}

}

///�����ѯͶ������Ӧ
void CtpTdSpi::OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯ���ױ�����Ӧ
void CtpTdSpi::OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!pTradingCode) 
		return;
	printf("Eid:%s,Cid:%s,CidType:%c\n", pTradingCode->ExchangeID, pTradingCode->ClientID,pTradingCode->ClientIDType);
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

///�����ѯ��������Ӧ
void CtpTdSpi::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯ��Ʒ��Ӧ
void CtpTdSpi::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

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

///�����ѯ������Ӧ
void CtpTdSpi::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯͶ���߽�������Ӧ
void CtpTdSpi::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	
	if (bIsLast)
	{
		printf("2");
	}
}


///����֪ͨ
void CtpTdSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	printf("OnRtnOrder������״̬:%c�������ύ״̬��%c\n", pOrder->OrderStatus, pOrder->OrderSubmitStatus);
}

///�ɽ�֪ͨ
void CtpTdSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	printf("TradeID:%s,OrdRef:%s\n", pTrade->TradeID, pTrade->OrderRef);
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
	
///�����ѯ�ͻ�֪ͨ��Ӧ
void CtpTdSpi::OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯ������Ϣȷ����Ӧ
void CtpTdSpi::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯͶ���ֲ߳���ϸ��Ӧ
void CtpTdSpi::OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///��ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ��Ӧ
void CtpTdSpi::OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯ�ֵ��۵���Ϣ��Ӧ
void CtpTdSpi::OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *pEWarrantOffset, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯͶ����Ʒ��/��Ʒ�ֱ�֤����Ӧ
void CtpTdSpi::OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯ��������֤������Ӧ
void CtpTdSpi::OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯ������������֤������Ӧ
void CtpTdSpi::OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯ������Ӧ
void CtpTdSpi::OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯ�����������Ա����Ȩ����Ӧ
void CtpTdSpi::OnRspQrySecAgentACIDMap(CThostFtdcSecAgentACIDMapField *pSecAgentACIDMap, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯ��Ȩ���׳ɱ���Ӧ
void CtpTdSpi::OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯ��Ȩ��Լ��������Ӧ
void CtpTdSpi::OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯִ��������Ӧ
void CtpTdSpi::OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯѯ����Ӧ
void CtpTdSpi::OnRspQryForQuote(CThostFtdcForQuoteField *pForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯ������Ӧ
void CtpTdSpi::OnRspQryQuote(CThostFtdcQuoteField *pQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯת����ˮ��Ӧ
void CtpTdSpi::OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯ����ǩԼ��ϵ��Ӧ
void CtpTdSpi::OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///����Ӧ��
void CtpTdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	printf("OnRspError-errormsg:%s\n",pRspInfo->ErrorMsg);
}


///����֪ͨ
void CtpTdSpi::OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) {}

///��ʾ������У�����
void CtpTdSpi::OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder) {}

///ִ������֪ͨ
void CtpTdSpi::OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder) {}

///ִ������¼�����ر�
void CtpTdSpi::OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo) {}

///ִ�������������ر�
void CtpTdSpi::OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo) {}

///ѯ��¼�����ر�
void CtpTdSpi::OnErrRtnForQuoteInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo) {}

///����֪ͨ
void CtpTdSpi::OnRtnQuote(CThostFtdcQuoteField *pQuote) {}

///����¼�����ر�
void CtpTdSpi::OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo) {}

///���۲�������ر�
void CtpTdSpi::OnErrRtnQuoteAction(CThostFtdcQuoteActionField *pQuoteAction, CThostFtdcRspInfoField *pRspInfo) {}

///ѯ��֪ͨ
void CtpTdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {}

///�����ѯǩԼ������Ӧ
void CtpTdSpi::OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯԤ����Ӧ
void CtpTdSpi::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯԤ�񳷵���Ӧ
void CtpTdSpi::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯ����֪ͨ��Ӧ
void CtpTdSpi::OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///�����ѯ���͹�˾���ײ�����Ӧ
void CtpTdSpi::OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	printf("1");
}



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
	if (m_vsFrtAddr.size() == 0)
	{
		ShowMessage("Init the frontaddr first��\n");
		return;
	}
	m_iReqNo = 0;
	m_pTdApi = CtpTdApi::CreateFtdcTraderApi("log//");
	m_pTdApi->RegisterSpi(this);
	m_pTdApi->SubscribePrivateTopic(THOST_TERT_QUICK);
	m_pTdApi->SubscribePublicTopic(THOST_TERT_QUICK);
	for (UINT i = 0; i < m_vsFrtAddr.size(); i++)
	{
		m_pTdApi->RegisterFront((char*)m_vsFrtAddr[i].c_str());
	}
	m_pTdApi->Init();
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

void CtpTdSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sBrkID);
	strcpy(req.InvestorID, m_sIvstID);
	int rlt = m_pTdApi->ReqQryTradingAccount(&req, ++m_iReqNo);
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
void CtpTdSpi::ReqQryIvstPosition()
{
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sBrkID);
	strcpy(req.InvestorID, m_sIvstID);

	int rlt = m_pTdApi->ReqQryInvestorPosition(&req, ++m_iReqNo);

}

void CtpTdSpi::ReqQryIvstPositionDetail()
{
	CThostFtdcQryInvestorPositionDetailField req;
	memset(&req, 0, sizeof(req));
// 	strcpy(req.BrokerID, m_sBrkID);
// 	strcpy(req.InvestorID, m_sIvstID);

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

void CtpTdSpi::ReqOrdLimit(TThostFtdcInstrumentIDType	Inst, TThostFtdcPosiDirectionType dir, TThostFtdcOffsetFlagType offset,
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

void CtpTdSpi::OnTick(CThostFtdcDepthMarketDataField* pDepthMarketData)
{

}
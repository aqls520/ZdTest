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

#pragma region Api回调

///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void CtpTdSpi::OnFrontConnected()
{
	m_connStatus = CTP_CONNECTED;
	ShowMessage("OnFrontConnected\n");
	ReqUserLogin();
}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
void CtpTdSpi::OnFrontDisconnected(int nReason)
{
	m_connStatus = CTP_DISCONNECTED;
	m_loginStatus = CTP_LOGOUT;
}

///登录请求响应
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

///报单录入请求响应
void CtpTdSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	printf("OnRspOrderInsert-errormsg:%s\n",pRspInfo->ErrorMsg);
}

///预埋单录入请求响应
void CtpTdSpi::OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///预埋撤单录入请求响应
void CtpTdSpi::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	printf("OnRspParkedOrderAction-errormsg:%s\n", pRspInfo->ErrorMsg);
}

///报单操作请求响应
void CtpTdSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	printf("OnRspOrderAction-errormsg:%s\n", pRspInfo->ErrorMsg);
}

///投资者结算结果确认响应
void CtpTdSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//ReqQryTradingAccount();
}

///请求查询报单响应
void CtpTdSpi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (pOrder)
	{
		printf("InstID:%s\n",pOrder->InstrumentID);
	}
}

///请求查询成交响应
void CtpTdSpi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pTrade)
	{
		printf("TradeID:%s,BID:%c\n", pTrade->TradeID, pTrade->OffsetFlag);
	}
}

///请求查询投资者持仓响应
void CtpTdSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!pInvestorPosition) 
		return;
	m_vPosition.push_back(*pInvestorPosition);
	if (bIsLast)
		SetEvent(m_Event);
}

///请求查询资金账户响应
void CtpTdSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast)
	{
		m_TradeAccount = *pTradingAccount;
		SetEvent(m_Event);
	}

}

///请求查询合约保证金率响应
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

///请求查询合约手续费率响应
void CtpTdSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (pInstrumentCommissionRate)
	{
		printf("OpenRatio:", pInstrumentCommissionRate->OpenRatioByMoney);
	}
}
///请求查询合约响应
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

///报单通知
void CtpTdSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	m_StgyExec->OnCtpRtnOrder(pOrder);
}

///成交通知
void CtpTdSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	m_StgyExec->OnCtpRtnTrade(pTrade);
}

///报单录入错误回报
void CtpTdSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	printf("OnErrRtnOrderInsert-errormsg:%s\n", pRspInfo->ErrorMsg);
}

///报单操作错误回报
void CtpTdSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	printf("OnErrRtnOrderAction-errormsg:%s\n", pRspInfo->ErrorMsg);
}

///合约交易状态通知
void CtpTdSpi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{

}

///请求查询投资者持仓明细响应
void CtpTdSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	printf("%s,%s,%f;%f,%f\n", pInvestorPositionDetail->InstrumentID,
		pInvestorPositionDetail->OpenDate,
		pInvestorPositionDetail->OpenPrice,
		pInvestorPositionDetail->CloseProfitByTrade,
		pInvestorPositionDetail->CloseProfitByDate);
}

///错误应答
void CtpTdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	printf("OnRspError-errormsg:%s\n",pRspInfo->ErrorMsg);
}
///交易通知
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

//限价单
/*必要字段：
BrokerID、InvestorID（若无，报无此权限）
InstrumentID（找不到合约）
LimitPrice（因被初始化为0，所以不填的话，报价格跌破跌停板）
OrderPriceType、Direction、CombOffsetFlag、CombHedgeFlag、VolumeTotalOriginal、
TimeCondition、VolumeCondition、ContingentCondition、ForceCloseReason（报单字段有误）
*/
/*非必要字段
OrderRef：系统自动从1开始自增，API端可以不需要自己维护，但要求递增，
就算是负值也可以，但必须递增。
如果OrderRef相同，一个正常报单，一个就被拒绝（OnRtnOrder）
如果OrderRef递减，则提示不允许重复报单，从OnRspOI和OnErrOI返回
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
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//价格类型=限价	
	req.Direction = dir;  //买卖方向	
	req.CombOffsetFlag[0] = offset; //组合开平标志:开仓
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	  //组合投机套保标志
	req.LimitPrice = price;	//价格
	req.VolumeTotalOriginal = vol;	///数量	
	req.TimeCondition = THOST_FTDC_TC_GFD;  //有效期类型:当日有效
	req.VolumeCondition = THOST_FTDC_VC_AV; //成交量类型:任何数量
	req.ContingentCondition = THOST_FTDC_CC_Immediately;  //触发条件:立即
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;	//强平原因:非强平	

	//这2个字段在memset的时候自动置为0了，所以在这里可指定也可以不指定
	req.IsAutoSuspend = 0;  //自动挂起标志:否	
	req.UserForceClose = 0;   //用户强评标志:否

	m_pTdApi->ReqOrderInsert(&req, ++m_iReqNo);

	return m_iOrderRef;

}
//市价单
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
	// AnyPrice + Price=0(不填）
	req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;//价格类型=限价	
	req.Direction = dir;  //买卖方向	
	req.CombOffsetFlag[0] = offset; //组合开平标志:开仓
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	  //组合投机套保标志
	req.VolumeTotalOriginal = vol;	///数量	
	req.TimeCondition = THOST_FTDC_TC_IOC;  //有效期类型:当日有效
	req.VolumeCondition = THOST_FTDC_VC_AV; //成交量类型:任何数量
	//req.MinVolume = 1;	//最小成交量:1	
	req.ContingentCondition = THOST_FTDC_CC_Immediately;  //触发条件:立即
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;	//强平原因:非强平	
	req.IsAutoSuspend = 0;  //自动挂起标志:否	
	req.UserForceClose = 0;   //用户强评标志:否

	m_pTdApi->ReqOrderInsert(&req, ++m_iReqNo);
}
/*
FOK(Fill Or Kill)指令是将报单的有效期类型设置为THOST_FTDC_TC_IOC,同时将成交量类型设置为THOST_FTDC_VC_CV,即全部数量.
FAK(Fill And Kill)指令就是将报单的有效期设为THOST_FTDC_TC_IOC,同时,成交量类型设为THOST_FTDC_VC_AV,即任意数量;

此外,在FAK指令下,还可指定最小成交量,即在指定价位、满足最小成交数量以上成交,剩余订单被系统撤销,否则被系统全部撤销.此种状况下,
有效期类型设置为THOST_FTDC_TC_IOC,数量条件设为THOST_FTDC_VC_MV,同时设定MinVolume字段.

Fill + Kill = IOC
Or = THOST_FTDC_VC_CV（全部数量）
And = THOST_FTDC_VC_AV（任意数量） 或者 THOST_FTDC_VC_MV + MinVolume=N
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

	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//价格类型=限价	
	req.Direction = dir;  //买卖方向	
	req.CombOffsetFlag[0] = offset; //组合开平标志:开仓
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	  //组合投机套保标志
	req.LimitPrice = price;	//价格
	req.VolumeTotalOriginal = vol;	///数量	
	req.TimeCondition = THOST_FTDC_TC_IOC;  //有效期类型:立即成交，否则撤销
	req.VolumeCondition = THOST_FTDC_VC_CV; //成交量类型:全部数量
	req.ContingentCondition = THOST_FTDC_CC_Immediately;  //触发条件:立即
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;	//强平原因:非强平	
	req.IsAutoSuspend = 0;  //自动挂起标志:否	
	req.UserForceClose = 0;   //用户强评标志:否

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
	
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//价格类型=限价	
	req.Direction = dir;  //买卖方向	
	req.CombOffsetFlag[0] = offset; //组合开平标志:开仓
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;	  //组合投机套保标志
	req.LimitPrice = price;	//价格
	req.VolumeTotalOriginal = vol;	///数量	
	req.TimeCondition = THOST_FTDC_TC_IOC;  //有效期类型:立即成交，否则撤销
	req.VolumeCondition = THOST_FTDC_VC_AV;
	req.MinVolume = minVol;	//最小成交量:1	
	req.ContingentCondition = THOST_FTDC_CC_Immediately;  //触发条件:立即
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;	//强平原因:非强平	
	req.IsAutoSuspend = 0;  //自动挂起标志:否	
	req.UserForceClose = 0;   //用户强评标志:否

	m_pTdApi->ReqOrderInsert(&req, ++m_iReqNo);
}

//条件单
void CtpTdSpi::ReqOrdCondition(TThostFtdcInstrumentIDType	Inst, TThostFtdcPosiDirectionType dir, TThostFtdcOffsetFlagType offset,
	TThostFtdcVolumeType vol, TThostFtdcPriceType price)
{

}

//撤单
void CtpTdSpi::ReqOrderCancelBySysID(TThostFtdcExchangeIDType ExchangeID, TThostFtdcOrderSysIDType OrdSysID)
{
	//BrokerID + InvestorID + ExchangeID + OrderSysID
	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sBrkID);   //经纪公司代码	
	strcpy(req.InvestorID, m_sIvstID); //投资者代码
	strcpy(req.ExchangeID, ExchangeID);
	strcpy(req.OrderSysID, OrdSysID);
	req.ActionFlag = THOST_FTDC_AF_Delete;  //操作标志 

	m_pTdApi->ReqOrderAction(&req, ++m_iReqNo);
}

void CtpTdSpi::ReqOrderCancelByOrdRef(TThostFtdcInstrumentIDType instId, TThostFtdcOrderRefType OrdRef)
{
	// FrontID + SessionID + OrdRef + InstrumentID
	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.InstrumentID, instId); //合约代码
	strcpy(req.OrderRef, OrdRef); //报单引用	
	req.FrontID = m_iFrontID;           //前置编号	
	req.SessionID = m_iSessionID;       //会话编号
	req.ActionFlag = THOST_FTDC_AF_Delete;  //操作标志 

	m_pTdApi->ReqOrderAction(&req, ++m_iReqNo);
}

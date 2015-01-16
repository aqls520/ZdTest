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

#pragma region Api回调

public:
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason);

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///客户端认证响应
	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单录入请求响应
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///预埋单录入请求响应
	virtual void OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///预埋撤单录入请求响应
	virtual void OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单操作请求响应
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///投资者结算结果确认响应
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	
	///请求查询报单响应
	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询成交响应
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者持仓响应
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询资金账户响应
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询合约保证金率响应
	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询合约手续费率响应
	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询合约响应
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者持仓明细响应
	virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单通知
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///成交通知
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

	///报单录入错误回报
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);

	///报单操作错误回报
	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);

	///合约交易状态通知
	virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus);

	///交易通知
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
	
	//报单
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

	//撤单
	void ReqOrderCancelBySysID(TThostFtdcExchangeIDType ExchangeID, TThostFtdcOrderSysIDType OrdSysID);
	void ReqOrderCancelByOrdRef(TThostFtdcInstrumentIDType instId, TThostFtdcOrderRefType OrdRef);

	//日志记录
	void ShowMessage(string msg);
	bool IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo);
public:
	//对外接口：初始化和释放
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

	//-----------交易环境基础信息-----------------
	vector<string> m_vsFrtAddr;
	TThostFtdcBrokerIDType m_sBrkID;
	TThostFtdcInvestorIDType m_sIvstID;
	TThostFtdcPasswordType m_sPwd;
	TThostFtdcFrontIDType m_iFrontID;
	TThostFtdcSessionIDType m_iSessionID;
	int m_iOrderRef;
	int m_iReqNo;

	//-----------------------------
	vector<CThostFtdcOrderField> m_vOrdList;//委托列表
	vector<CThostFtdcTradeField> m_vTrdList;//成交列表

	SpStgyExec* m_StgyExec;
	HANDLE m_Event;
	



};


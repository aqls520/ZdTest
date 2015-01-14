#pragma once

#include "Lib_dll/ThostFtdcMdApi.h"
#include <vector>
#include <windows.h>
using namespace std;


class SpStgyExec;

class CtpQtSpi:public CThostFtdcMdSpi
{
public:
	CtpQtSpi();
	~CtpQtSpi();

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

	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///订阅行情应答
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///订阅询价应答
	virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///取消订阅询价应答
	virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///深度行情通知
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) ;

	///询价通知
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) ;

private:
	void ReqUserLogin();
	void ShowMessage(string msg);
	bool IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo);
public:
	void Init();
	void LoadQtCfg(char* cfgpath);
	void LoadInstCfg(char* instpath);
	void SubMarketData(char* pInstruments[], int tcount);
	void UnSubMarketData(char* pInstruments[], int tcount);
	void Release();
	void RegisterStgyExec(SpStgyExec* stgyexec);

public:
	ConnectStatus m_connStatus;
	LoginStatus m_loginStatus;

private:
	CThostFtdcMdApi* m_pQtApi;
	vector<string> m_vsFrtAddr;
	vector<string> m_vsInst;
	int m_iReqNo;
	SpStgyExec* m_StgyExec;
	HANDLE m_Event;


};


#include "CtpQtSpi.h"
#include <fstream>
#include "SpStgyExec.h"

CtpQtSpi::CtpQtSpi()
{
	m_Event = CreateEvent(NULL,false,false,NULL);
}

CtpQtSpi::~CtpQtSpi()
{}

void CtpQtSpi::RegisterStgyExec(SpStgyExec* stgyexec)
{
	this->m_StgyExec = stgyexec;
}

#pragma region Api回调

///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void CtpQtSpi::OnFrontConnected()
{
	m_connStatus = CTP_CONNECTED;
	ReqUserLogin();
}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void CtpQtSpi::OnFrontDisconnected(int nReason)
{
	m_connStatus = CTP_DISCONNECTED;
	m_loginStatus = CTP_LOGOUT;
}

///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
void CtpQtSpi::OnHeartBeatWarning(int nTimeLapse){}

///登录请求响应
void CtpQtSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		m_loginStatus = CTP_LOGIN;
	}
}

///错误应答
void CtpQtSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	ShowMessage("OnRspError\n");
}
///订阅行情应答
void CtpQtSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	ShowMessage("OnRspSubMarketData\n");
}
///取消订阅行情应答
void CtpQtSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	ShowMessage("OnRspUnSubMarketData\n");
}
///订阅询价应答
void CtpQtSpi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	ShowMessage("OnRspSubForQuoteRsp\n");
}
///取消订阅询价应答
void CtpQtSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	ShowMessage("OnRspUnSubForQuoteRsp\n");
}

///深度行情通知
void CtpQtSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) 
{
	m_StgyExec->OnCtpRtnTick(pDepthMarketData);
}

///询价通知
void CtpQtSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) 
{
	ShowMessage("OnRspUnSubForQuoteRsp\n");
}

#pragma endregion

void CtpQtSpi::Init()
{
	m_pQtApi = CThostFtdcMdApi::CreateFtdcMdApi();
	m_pQtApi->RegisterSpi(this);
	m_pQtApi->RegisterFront("tcp://27.115.78.193:41213");
	m_pQtApi->Init();
	while (m_loginStatus != CTP_LOGIN)
	{}
}

void CtpQtSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	int r = m_pQtApi->ReqUserLogin(&req, 1);
}
void CtpQtSpi::LoadQtCfg(char* cfgpath)
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
	}
	fin.close();
}
void CtpQtSpi::SubMarketData(char* pInstruments[], int tcount)
{
	m_pQtApi->SubscribeMarketData(pInstruments, tcount);
}
void CtpQtSpi::UnSubMarketData(char* pInstruments[], int tcount)
{
	m_pQtApi->UnSubscribeMarketData(pInstruments, tcount);
}
void CtpQtSpi::Release()
{
	if (m_pQtApi)
	{
		m_pQtApi->RegisterSpi(NULL);
		m_pQtApi->Release();
		m_pQtApi = NULL;
	}
}
void CtpQtSpi::ShowMessage(string msg)
{
	printf(msg.c_str());
}
bool CtpQtSpi::IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo)
{
	return (pRspInfo) && (pRspInfo->ErrorID);
}
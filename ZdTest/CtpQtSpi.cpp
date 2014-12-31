#include "CtpQtSpi.h"
#include <fstream>
#include <windows.h>
#include <string>

CtpQtSpi::CtpQtSpi()
{}

CtpQtSpi::~CtpQtSpi()
{}

#pragma region Api�ص�

///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CtpQtSpi::OnFrontConnected()
{
	ReqUserLogin();
}

///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void CtpQtSpi::OnFrontDisconnected(int nReason){}

///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
void CtpQtSpi::OnHeartBeatWarning(int nTimeLapse){}

///��¼������Ӧ
void CtpQtSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{

	}
}

///����Ӧ��
void CtpQtSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	ShowMessage("OnRspError\n");
}
///��������Ӧ��
void CtpQtSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	ShowMessage("OnRspSubMarketData\n");
}
///ȡ����������Ӧ��
void CtpQtSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	ShowMessage("OnRspUnSubMarketData\n");
}
///����ѯ��Ӧ��
void CtpQtSpi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	ShowMessage("OnRspSubForQuoteRsp\n");
}
///ȡ������ѯ��Ӧ��
void CtpQtSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	ShowMessage("OnRspUnSubForQuoteRsp\n");
}

///�������֪ͨ
void CtpQtSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) 
{
	printf("ID:%s,Lp:%g,Time:%s,MSec:%d\n", pDepthMarketData->InstrumentID,pDepthMarketData->LastPrice, pDepthMarketData->UpdateTime,pDepthMarketData->UpdateMillisec);
}

///ѯ��֪ͨ
void CtpQtSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) 
{
	ShowMessage("OnRspUnSubForQuoteRsp\n");
}

#pragma endregion

void CtpQtSpi::Init()
{
	m_pQtApi = CThostFtdcMdApi::CreateFtdcMdApi("log//");
	m_pQtApi->RegisterSpi(this);
	for (unsigned int i = 0; i < m_vsFrtAddr.size(); i++)
	{
		m_pQtApi->RegisterFront((char*)m_vsFrtAddr[i].c_str());
	}
	m_pQtApi->Init();
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
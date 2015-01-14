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

	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///��������Ӧ��
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///����ѯ��Ӧ��
	virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///ȡ������ѯ��Ӧ��
	virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) ;

	///ѯ��֪ͨ
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


#include "SpStgyExec.h"
#include "SpreadStgy.h"
#include "CtpQtSpi.h"
#include "CtpTdSpi.h"
#include <process.h>

struct ExecModule
{
	SpStgyExec* pExec;
	CtpSpOrder spod;
};

//����ִ��ģ��
void OrderExecer(LPVOID p)
{
	ExecModule* pEM = (ExecModule*)p;
	SpStgyExec* pExec = pEM->pExec;
	CtpSpOrder spod = pEM->spod;

	//���ݲ������ý��б���
	pExec->ExecAOrder(spod);
}

//������Ϊ��������ⱨ������������������������ã���ִ�б���
void OrderWatcher(LPVOID p)
{
	SpStgyExec* pExec = (SpStgyExec*)p;
	while (true)
	{
		//��������
		WaitForSingleObject(pExec->m_MDEvent, INFINITE);
		//��ⱨ���Ƿ���������
		for (size_t i = 0; i < pExec->m_vNotExecSpOd.size();i++)
		{
			bool rlt = pExec->CheckOrder(pExec->m_vNotExecSpOd[i]);
			if (rlt)
			{
				pExec->m_vNotExecSpOd[i].SpOrderStatus = TOUCH;
				CtpSpOrder spod = pExec->m_vNotExecSpOd[i];
				ExecModule tmp;
				tmp.spod = spod;
				tmp.pExec = pExec;
				//�����߳�ִ�б���
				_beginthread(OrderExecer, 0, (LPVOID)&tmp);
				pExec->m_vNotExecSpOd.pop_back();
			}
		}
	}
}

SpStgyExec::SpStgyExec()
{
	InitializeCriticalSection(&m_cs);
	m_Event = CreateEvent(NULL, FALSE, TRUE, NULL);
	m_MDEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	InitSpi();
	ActiveInstCode = "IF1502";
	PassiveInstCode = "IF1501";
	_beginthread(OrderWatcher, 0, (LPVOID)this);
}
SpStgyExec::~SpStgyExec()
{
	DeleteCriticalSection(&m_cs);
}


bool SpStgyExec::CheckOrder(CtpSpOrder spod)
{
	return true;
}


void SpStgyExec::InitSpi()
{
	pQtSpi = new CtpQtSpi();
	pQtSpi->RegisterStgyExec(this);
	pQtSpi->Init();

	pTdSpi = new CtpTdSpi();
	pTdSpi->RegisterStgyExec(this);
	pTdSpi->Init();

}

//��������ִ��
void SpStgyExec::ExecAOrder(CtpSpOrder spod)
{
	//�۲��,��=������������������=�������򱻶�
	vector<ComOrder> splitOrderList = SplitSpOrder(spod);
	//���Ӳ����ɣ����ղ������õķ�ʽ�����µ�
	//����һ��ͬʱ����
	pTdSpi->ReqOrdLimit((char*)splitOrderList[0].Inst.c_str(),
		splitOrderList[0].Dir,
		splitOrderList[0].Offset,
		splitOrderList[0].Vol,
		splitOrderList[0].Price);
	pTdSpi->ReqOrdLimit((char*)splitOrderList[1].Inst.c_str(),
		splitOrderList[1].Dir,
		splitOrderList[1].Offset,
		splitOrderList[1].Vol,
		splitOrderList[1].Price);
}


vector<ComOrder> SpStgyExec::SplitSpOrder(CtpSpOrder spod)
{
	ComOrder od1, od2;
	od1.Inst = spod.pActOrder.InstrumentID;
	od1.Dir = spod.Direction;
	od1.Offset = THOST_FTDC_OF_Open;
	od1.Vol = 1;
	od1.Price = m_mInstTick[od1.Inst].AskPrice1;

	od2.Inst = spod.pPasOrder.InstrumentID;
	od2.Dir = '1' + '0' - od1.Dir;
	od2.Offset = THOST_FTDC_OF_Open;
	od2.Vol = 1;
	od2.Price = m_mInstTick[od2.Inst].BidPrice1;

	vector<ComOrder> tmp;
	tmp.push_back(od1);
	tmp.push_back(od2);
	return tmp;
}

//���������������ݱ�����״̬�����ò��Զ�Ӧ����Ӧ����
void SpStgyExec::OrderAdapter(CtpSpOrder spod)
{
	switch (spod.OrderAction)
	{
	case CancelOrder:
		{
			m_SpStgy->RtnOrderCancel(spod);
		}
		break;
	case SendOrder:
		{
			m_SpStgy->RtnOrderFill(spod);
		}
		break;
	default:
		break;
	}
}

//�����Ƹ�����
void SpStgyExec::OnCtpRtnTick(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	//���ݴ洢
	m_mInstTick[pDepthMarketData->InstrumentID] = *pDepthMarketData;

	//���ݼ���
	SpTick spt;
	spt.Spread = m_mInstTick[ActiveInstCode].LastPrice-
				 m_mInstTick[PassiveInstCode].LastPrice;
	spt.SpreadAsk1P = m_mInstTick[ActiveInstCode].AskPrice1-
					m_mInstTick[PassiveInstCode].BidPrice1;
	spt.SpreadBid1P = m_mInstTick[ActiveInstCode].BidPrice1-
		m_mInstTick[PassiveInstCode].AskPrice1;
	spt.SpreadAsk1V = m_mInstTick[ActiveInstCode].AskVolume1>m_mInstTick[PassiveInstCode].BidVolume1?
		m_mInstTick[PassiveInstCode].BidVolume1:m_mInstTick[ActiveInstCode].AskVolume1;
	spt.SpreadBid1V = m_mInstTick[ActiveInstCode].BidVolume1>m_mInstTick[PassiveInstCode].AskVolume1?
		m_mInstTick[PassiveInstCode].AskVolume1:m_mInstTick[ActiveInstCode].BidVolume1;
	
	//���ݴ���
	m_curSpTick = spt;
	m_vSpTickL.push_back(spt);

	SetEvent(m_MDEvent);

	//���͸�����
	m_SpStgy->RtnTick(spt);
}

void SpStgyExec::OnCtpRtnTrade(CThostFtdcTradeField* pTrade)
{
	for (size_t i=0;i<m_vSpOdList.size();i++)
	{
		if (m_vAllSpOd[i].pActOrder.OrderSysID == pTrade->OrderSysID)
		{

		}
		if (m_vAllSpOd[i].pPasOrder.OrderSysID == pTrade->OrderSysID)
		{
		}
	}
}

void SpStgyExec::OnCtpRtnOrder(CThostFtdcOrderField* pOrder)
{
	int idx = -1;

	for (size_t i=0;i<m_vSpOdList.size();i++)
	{
		if (m_vAllSpOd[i].pActOrder.OrderSysID == pOrder->OrderSysID)
		{
			m_vAllSpOd[i].pActOrder = *pOrder;
			idx=i;
			break;
		}
		else
		{
			if (m_vAllSpOd[i].pPasOrder.OrderSysID == pOrder->OrderSysID)
			{
				m_vAllSpOd[i].pPasOrder = *pOrder;
				idx=i;
				break;
			}
		}
	}
	if (idx==-1)
		return;
	
	//ͬ��ʱ��ֻ����һ��OrderAdapter��ִ�У���֤���Զ���˳����ջر�
	EnterCriticalSection(&m_cs);
	OrderAdapter(m_vAllSpOd[idx]);
	LeaveCriticalSection(&m_cs);
}

//�����仯�˾��Ƹ�����
void SpStgyExec::OnOrder(CtpSpOrder CtpSpOrder)
{
	switch (CtpSpOrder.SpOrderStatus)
	{
	case ALLFILL:
		m_SpStgy->RtnOrderFill(CtpSpOrder);
		break;
	

	default:
		break;
	}
}


void SpStgyExec::SendSpOrder(CtpSpOrder spod)
{
	EnterCriticalSection(&m_cs);
	spod.SpOrderStatus = NOTTOUCH;
	m_vAllSpOd.push_back(spod);
	m_vNotExecSpOd.push_back(spod);
	LeaveCriticalSection(&m_cs);
}

//���ĺ�Լ
void SpStgyExec::SubMarketData(StgyConfig astgycfg)
{
	char* pInst[2];
	pInst[0] = new char[200];
	pInst[1] = new char[200];

	strcpy(pInst[0],astgycfg.ActiveInst.code.c_str());
	strcpy(pInst[1],astgycfg.PassiveInst.code.c_str());

	pQtSpi->SubMarketData(pInst,2);
}

//�ϴ���������
void SpStgyExec::UpLoadStgyCfg(StgyConfig astgycfg)
{
	this->m_MyStgyCfg = astgycfg;
}

void SpStgyExec::RegisterExec(SpreadStgy* pSpStgy)
{
	this->m_SpStgy = pSpStgy;
}

//Ԥ���ӿ�
//�������ø���
void SpStgyExec::UpdateStgyCfg(StgyConfig* aStygCfg)
{
	m_MyStgyCfg = *aStygCfg; 
}
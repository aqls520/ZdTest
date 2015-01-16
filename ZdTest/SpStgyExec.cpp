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

//报单执行模块
void OrderExecer(LPVOID p)
{
	ExecModule* pEM = (ExecModule*)p;
	SpStgyExec* pExec = pEM->pExec;
	CtpSpOrder spod = pEM->spod;

	//根据策略配置进行报单
	pExec->ExecAOrder(spod);
}

//以行情为驱动，监测报单触发条件，即如果满足设置，则执行报单
void OrderWatcher(LPVOID p)
{
	SpStgyExec* pExec = (SpStgyExec*)p;
	while (true)
	{
		//行情驱动
		WaitForSingleObject(pExec->m_MDEvent, INFINITE);
		//检测报单是否满足条件
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
				//启动线程执行报单
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

//报单具体执行
void SpStgyExec::ExecAOrder(CtpSpOrder spod)
{
	//价差单拆单,买单=买主动卖被动，卖单=卖主动买被动
	vector<ComOrder> splitOrderList = SplitSpOrder(spod);
	//单子拆分完成，按照策略配置的方式进行下单
	//测试一：同时报单
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

//报单适配器，根据报单的状态，调用策略对应的响应函数
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

//行情推给策略
void SpStgyExec::OnCtpRtnTick(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	//数据存储
	m_mInstTick[pDepthMarketData->InstrumentID] = *pDepthMarketData;

	//数据计算
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
	
	//数据处理
	m_curSpTick = spt;
	m_vSpTickL.push_back(spt);

	SetEvent(m_MDEvent);

	//推送给策略
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
	
	//同个时刻只能有一个OrderAdapter在执行，保证策略端能顺序接收回报
	EnterCriticalSection(&m_cs);
	OrderAdapter(m_vAllSpOd[idx]);
	LeaveCriticalSection(&m_cs);
}

//报单变化了就推给策略
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

//订阅合约
void SpStgyExec::SubMarketData(StgyConfig astgycfg)
{
	char* pInst[2];
	pInst[0] = new char[200];
	pInst[1] = new char[200];

	strcpy(pInst[0],astgycfg.ActiveInst.code.c_str());
	strcpy(pInst[1],astgycfg.PassiveInst.code.c_str());

	pQtSpi->SubMarketData(pInst,2);
}

//上传策略配置
void SpStgyExec::UpLoadStgyCfg(StgyConfig astgycfg)
{
	this->m_MyStgyCfg = astgycfg;
}

void SpStgyExec::RegisterExec(SpreadStgy* pSpStgy)
{
	this->m_SpStgy = pSpStgy;
}

//预留接口
//策略配置更新
void SpStgyExec::UpdateStgyCfg(StgyConfig* aStygCfg)
{
	m_MyStgyCfg = *aStygCfg; 
}
#include "SpStgyExec.h"
#include "SpreadStgy.h"

void ExecOrder(LPVOID p)
{
	/*SpStgyExec* pStgyExec = (SpStgyExec*)p;
	while (true)
	{
		for (size_t i = 0; i < pStgyExec->m_vSpOdList.size();i++)
		{
			if (pStgyExec->m_vSpOdList[i].SpOrderStatus == NOTTOUCH)
			{
				
			}
		}
		WaitForSingleObject(pStgyExec->m_Event, INFINITE);
	}*/

}

SpStgyExec::SpStgyExec()
{
	InitializeCriticalSection(&m_cs);
	m_Event = CreateEvent(NULL, FALSE, TRUE, NULL);
	InitSpi();
	ActiveInstCode = "IF1502";
	PassiveInstCode = "IF1501";
}
SpStgyExec::~SpStgyExec()
{
	DeleteCriticalSection(&m_cs);
}

void SpStgyExec::InitSpi()
{
	pQtSpi = new CtpQtSpi();
	pQtSpi->RegisterStgyExec(this);
	pQtSpi->Init();
	//pTdSpi->Init();
}

void SpStgyExec::ExecAOrder(SpOrder)
{

}

//报单适配器，根据报单的状态，调用策略对应的响应函数
void SpStgyExec::OrderAdapter(SpOrder spod)
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

void SpStgyExec::OrderAdapter(CtpSpOrder ctpspod)
{
	switch (ctpspod.OrderAction)
	{
	case CancelOrder:
		{
			m_SpStgy->RtnOrderCancel(ctpspod);
		}
		break;
	case SendOrder:
		{
			m_SpStgy->RtnOrderFill(ctpspod);
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
	//推送给策略
	m_SpStgy->RtnTick(spt);
}

void SpStgyExec::OnCtpRtnTrade(CThostFtdcTradeField* pTrade)
{
	for (size_t i=0;i<m_vSpOdList.size();i++)
	{
		if (m_vCtpSpOdList[i].pActOrder.OrderSysID == pTrade->OrderSysID)
		{

		}
		if (m_vCtpSpOdList[i].pPasOrder.OrderSysID == pTrade->OrderSysID)
		{
		}
	}
	
}

void SpStgyExec::OnCtpRtnOrder(CThostFtdcOrderField* pOrder)
{
	int idx = -1;

	for (size_t i=0;i<m_vSpOdList.size();i++)
	{
		if (m_vCtpSpOdList[i].pActOrder.OrderSysID == pOrder->OrderSysID)
		{
			m_vCtpSpOdList[i].pActOrder = *pOrder;
			idx=i;
			break;
		}
		else
		{
			if (m_vCtpSpOdList[i].pPasOrder.OrderSysID == pOrder->OrderSysID)
			{
				m_vCtpSpOdList[i].pPasOrder = *pOrder;
				idx=i;
				break;
			}
		}
	}
	if (idx==-1)
		return;
	
	OrderAdapter(m_vCtpSpOdList[idx]);
}

//报单变化了就推给策略
void SpStgyExec::OnOrder(SpOrder spOrder)
{
	switch (spOrder.SpOrderStatus)
	{
	case ALLFILL:
		m_SpStgy->RtnOrderFill(spOrder);
		break;
	

	default:
		break;
	}
}

void SpStgyExec::SendSpOrder(SpOrder spod)
{
	EnterCriticalSection(&m_cs);
	m_vSpOdList.push_back(spod);
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
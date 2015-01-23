#include "SpStgyExec.h"
#include "SpreadStgy.h"
#include "CtpQtSpi.h"
#include "CtpTdSpi.h"
#include <process.h>

struct ExecModule
{
	SpStgyExec* pExec;
	CtpSpOrder* spod;
};

//报单执行模块
void OrderExecer(LPVOID p)
{
	ExecModule* pEM = (ExecModule*)p;
	//根据策略配置进行报单
	pEM->pExec->ExecAOrder(pEM->spod);
}

//以行情为驱动，监测报单触发条件，即如果满足设置，则执行报单
void OrderWatcher(LPVOID p)
{
	SpStgyExec* pExec = (SpStgyExec*)p;
	while (true)
	{
		int qSize = pExec->m_qSpOrder.size();
		if (qSize > 0)
		{
			pExec->m_vAllSpOd.push_back(pExec->m_qSpOrder.front());
			ExecModule EM;
			EM.pExec = pExec;
			EM.spod = &(pExec->m_vAllSpOd.back());
			_beginthread(OrderExecer, 0, (LPVOID)&EM);
			EnterCriticalSection(&pExec->m_qSpOdCS);
			pExec->m_qSpOrder.pop();
			LeaveCriticalSection(&pExec->m_qSpOdCS);
		}
		else
		{
			WaitForSingleObject(pExec->m_NewOrderEvent, INFINITE);
		}
	}
}

SpStgyExec::SpStgyExec()
{
	InitializeCriticalSection(&m_cs);
	InitializeCriticalSection(&m_qSpOdCS);
}
SpStgyExec::~SpStgyExec()
{
	DeleteCriticalSection(&m_cs);
	DeleteCriticalSection(&m_qSpOdCS);
}

void SpStgyExec::Init()
{
	m_Event = CreateEvent(NULL, FALSE, TRUE, NULL);
	m_MDEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	m_NewOrderEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	InitSpi();
	_beginthread(OrderWatcher, 0, (LPVOID)this);
}


bool SpStgyExec::CheckOrder(CtpSpOrder spod)
{
	return true;
}


void SpStgyExec::InitSpi()
{
	//初始化行情接口
	pQtSpi = new CtpQtSpi();
	pQtSpi->RegisterStgyExec(this);
	pQtSpi->Init();

	//初始化交易接口
	pTdSpi = new CtpTdSpi();
	pTdSpi->RegisterStgyExec(this);
	pTdSpi->Init();

}

//报单具体执行
void SpStgyExec::ExecAOrder(CtpSpOrder* spod)
{
	while (true)
	{
		switch (spod->SpOrderStatus)
		{
		case SP_NOTTOUCH: //未触发：监测行情
			{
				if (CheckOrder(*spod))
				{
					spod->SpOrderStatus = SP_TOUCH;
					SetEvent(spod->OrderStatusChgEvent);
				}
			}
			break;
		case SP_TOUCH://已触发：按策略配置下单
			{
				//主动腿成交了再报被动腿
				ComOrder od1 = GetActOrder(*spod);
				int OrderRef = pTdSpi->ReqOrdLimit((char*)od1.Inst.c_str(),
					od1.Dir,
					od1.Offset,
					od1.Vol,
					od1.Price-5.4);
				sprintf_s(spod->pActOrder.OrderRef,"%d",OrderRef);
				spod->SpOrderStatus = SP_ACT_ORDER;
			}
			break;
		case SP_ACT_ORDER://主动腿已报，待成交
			{
				
			}
			break;
		case SP_ACT_FILL://主动腿已成交
			{
				//主动腿全部成交，被动腿报单
				ComOrder od2 = GetPasOrder(*spod);
				int OrderRef = pTdSpi->ReqOrdLimit((char*)od2.Inst.c_str(),
					od2.Dir,
					od2.Offset,
					od2.Vol,
					od2.Price);
				sprintf_s(spod->pPasOrder.OrderRef, "%d", OrderRef);
			}
			break;

		case SP_PAS_ORDER://被动腿已报，待成交
			{
				
			}
			break;
		case SP_PAS_FILL://被动腿已成交
			{}
			break;

		//全部成交/撤单：单子执行完毕，退出线程
		case SP_ALL_FILL:return;
		case SP_CANCEL:return;
		}
		WaitForSingleObject(spod->OrderStatusChgEvent, INFINITE);
	}
}

void SpStgyExec::CancelAOrder(CtpSpOrder* pSpod)
{
	TThostFtdcInstrumentIDType inst;
	TThostFtdcOrderRefType orderRef;
	strcpy(inst, pSpod->pActOrder.InstrumentID);
	strcpy(orderRef, pSpod->pActOrder.OrderRef);
	pTdSpi->ReqOrderCancelByOrdRef(inst, orderRef);
}

//根据合约的持仓，校验开平仓字段
char SpStgyExec::CheckOrderOffset(ComOrder od)
{
	int NetOpenInt = m_InstPos[od.Inst];
	int OrderDir = (od.Dir == THOST_FTDC_D_Buy) ? 1 : -1;
	//同向为开仓，反向为平仓
	if (NetOpenInt*OrderDir >= 0)
		return THOST_FTDC_OF_Open;
	else
		return THOST_FTDC_OF_Close;
}

//价差单拆成主动腿单
ComOrder SpStgyExec::GetActOrder(CtpSpOrder spod)
{
	ComOrder od;
	od.Inst = m_MyStgyCfg.ActiveInst.code;
	od.Dir = spod.Direction;
	od.Vol = m_MyStgyCfg.ActiveInst.orderVolRatio * spod.Vol;
	od.Price = (od.Dir == THOST_FTDC_D_Buy) ?
		m_mInstTick[od.Inst].AskPrice1 : m_mInstTick[od.Inst].BidPrice1;
	od.Offset = CheckOrderOffset(od);
	return od;
}
//价差单拆成被动腿单
ComOrder SpStgyExec::GetPasOrder(CtpSpOrder spod)
{
	ComOrder od;
	od.Inst = m_MyStgyCfg.PassiveInst.code;
	od.Dir = '1' + '0' - spod.Direction;
	od.Vol = m_MyStgyCfg.PassiveInst.orderVolRatio * spod.Vol;
	od.Price = (od.Dir == THOST_FTDC_D_Buy) ?
		m_mInstTick[od.Inst].AskPrice1 : m_mInstTick[od.Inst].BidPrice1;
	od.Offset = CheckOrderOffset(od);
	return od;
}

vector<ComOrder> SpStgyExec::SplitSpOrder(CtpSpOrder spod)
{
	ComOrder od1, od2;
	od1.Inst = m_MyStgyCfg.ActiveInst.code;
	od1.Dir = spod.Direction;
	od1.Offset = THOST_FTDC_OF_Open;
	od1.Vol = 1;
	od1.Price = m_mInstTick[od1.Inst].AskPrice1;

	od2.Inst = m_MyStgyCfg.PassiveInst.code;
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
	spt.Spread = m_mInstTick[ActiveInstCode].LastPrice -
		m_mInstTick[PassiveInstCode].LastPrice;
	spt.SpreadAsk1P = m_mInstTick[ActiveInstCode].AskPrice1 -
		m_mInstTick[PassiveInstCode].BidPrice1;
	spt.SpreadBid1P = m_mInstTick[ActiveInstCode].BidPrice1 -
		m_mInstTick[PassiveInstCode].AskPrice1;
	spt.SpreadAsk1V = m_mInstTick[ActiveInstCode].AskVolume1 > m_mInstTick[PassiveInstCode].BidVolume1 ?
		m_mInstTick[PassiveInstCode].BidVolume1 : m_mInstTick[ActiveInstCode].AskVolume1;
	spt.SpreadBid1V = m_mInstTick[ActiveInstCode].BidVolume1 > m_mInstTick[PassiveInstCode].AskVolume1 ?
		m_mInstTick[PassiveInstCode].AskVolume1 : m_mInstTick[ActiveInstCode].BidVolume1;

	//数据处理
	m_curSpTick = spt;
	m_vSpTickL.push_back(spt);

	SetEvent(m_MDEvent);

	//推送给策略
	m_SpStgy->RtnTick(spt);
}

void SpStgyExec::OnCtpRtnTrade(CThostFtdcTradeField* pTrade)
{
	for (size_t i = 0; i < m_vAllSpOd.size(); i++)
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

	for (size_t i = 0; i < m_vAllSpOd.size(); i++)
	{
		if (!strcmp(m_vAllSpOd[i].pActOrder.OrderRef,pOrder->OrderRef))
		{
			m_vAllSpOd[i].pActOrder = *pOrder;
			
			if (pOrder->OrderStatus == THOST_FTDC_OST_AllTraded)
			{
				m_vAllSpOd[i].SpOrderStatus = SP_ACT_FILL;
				SetEvent(m_vAllSpOd[i].OrderStatusChgEvent);
			}
			//同个时刻只能有一个OrderAdapter在执行，保证策略端能顺序接收回报
			EnterCriticalSection(&m_cs);
			OrderAdapter(m_vAllSpOd[i]);
			LeaveCriticalSection(&m_cs);

			break;
		}
		else
		{
			if (!strcmp(m_vAllSpOd[i].pPasOrder.OrderRef, pOrder->OrderRef))
			{
				m_vAllSpOd[i].pPasOrder = *pOrder;

				if (pOrder->OrderStatus == THOST_FTDC_OST_AllTraded)
				{
					m_vAllSpOd[i].SpOrderStatus = SP_ACT_FILL;
					SetEvent(m_vAllSpOd[i].OrderStatusChgEvent);
				}
				EnterCriticalSection(&m_cs);
				OrderAdapter(m_vAllSpOd[i]);
				LeaveCriticalSection(&m_cs);

				break;
			}
		}
	}
}


void SpStgyExec::SendSpOrder(CtpSpOrder spod)
{
	EnterCriticalSection(&m_cs);
	spod.SpOrderStatus = SP_NOTTOUCH;
	spod.OrderStatusChgEvent = CreateEvent(NULL, false, true, NULL);
	m_qSpOrder.push(spod);
	LeaveCriticalSection(&m_cs);
	SetEvent(m_NewOrderEvent);
}

ErrInfo SpStgyExec::CancelSpOrder(int SpOrderRef)
{
	ErrInfo ei;
	for (size_t i = 0; i < m_vAllSpOd.size();i++)
	{
		if (m_vAllSpOd[i].SpOrderRef == SpOrderRef)
		{
			if (m_vAllSpOd[i].SpOrderStatus < '4')//主动腿未成交，允许撤单
			{
				ei.ErrorID = 0;
				ei.ErrorMsg = "";
				CancelAOrder(&m_vAllSpOd[i]);
				return ei;
			}
			else
			{
				//找到报单，但是至少主动腿已经成交了，此时不允许撤单
				ei.ErrorID = 2;
				ei.ErrorMsg = "报单状态不允许进行撤单操作";
				return ei;
			}
		}
	}
	//没有找到报单，返回错误码
	ei.ErrorID = 1;
	ei.ErrorMsg = "没有找到该价差单，请确认价差报单引用是否错误";
	return ei;
}

//订阅合约
void SpStgyExec::SubMarketData(StgyConfig astgycfg)
{
	char* pInst[2];
	pInst[0] = new char[200];
	pInst[1] = new char[200];

	strcpy(pInst[0], astgycfg.ActiveInst.code.c_str());
	strcpy(pInst[1], astgycfg.PassiveInst.code.c_str());

	pQtSpi->SubMarketData(pInst, 2);
}

//上传策略配置
void SpStgyExec::UpLoadStgyCfg(StgyConfig astgycfg)
{
	this->m_MyStgyCfg = astgycfg;
	ActiveInstCode = m_MyStgyCfg.ActiveInst.code;
	PassiveInstCode = m_MyStgyCfg.PassiveInst.code;
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

vector<CThostFtdcInvestorPositionField> SpStgyExec::GetCTPCurPosition()
{
	return pTdSpi->ReqQryIvstPosition();
}
CThostFtdcTradingAccountField SpStgyExec::GetCTPCurAccoutMoney()
{
	return pTdSpi->ReqQryTradingAccount();
}
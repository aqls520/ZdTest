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

SpStgyExec::SpStgyExec(SpreadStgy* pSpStgy)
{
	this->m_SpStgy = pSpStgy; 
	pSpStgy->m_SpSE = this;

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
	InitializeCriticalSection(&m_cs);
	InitializeCriticalSection(&m_qSpOdCS);
	m_Event = CreateEvent(NULL, FALSE, TRUE, NULL);
	m_MDEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	m_NewOrderEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	m_log = new Log();
	InitSpi();
	_beginthread(OrderWatcher, 0, (LPVOID)this);
}
//检测报单触发条件
bool SpStgyExec::CheckOrderTouch(CtpSpOrder spod)
{
	return true;
	if (spod.Direction == THOST_FTDC_D_Buy)
	{
		//价差校验
		if (spod.OrderSpread < m_curSpTick.SpreadAsk1P) 
			return false;
		//盘口校验:安全深度大于0，跳过最小盘口检测
		if (m_StgyCfg.PassiveInst.saveDepth <= 0) 
			return true;
		//检测最小盘口
		if (m_mInstTick[PasInstCode].BidVolume1 < m_StgyCfg.PassiveInst.minOrderBookVol)
			return false;
		else
			return true;
	}
	else
	{
		//价差校验
		if (spod.OrderSpread > m_curSpTick.SpreadBid1P)
			return false;
		//盘口校验:安全深度大于0，跳过最小盘口检测
		if (m_StgyCfg.PassiveInst.saveDepth <= 0)
			return true;
		//检测最小盘口
		if (m_mInstTick[PasInstCode].AskVolume1 < m_StgyCfg.PassiveInst.minOrderBookVol)
			return false;
		else
			return true;
	}
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
				while (true)
				{
					//等待行情变化，校验报单触发条件
					WaitForSingleObject(m_MDEvent, INFINITE);
					//校验成功，退出循环，修改报单状态变化
					if (CheckOrderTouch(*spod))
					{
						printf("触发");
						spod->SpOrderStatus = SP_TOUCH;
						SetEvent(spod->OrderStatusChgEvent);
						break;
					}
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
					od1.Price);
				sprintf_s(spod->pActOrder.OrderRef,"%d",OrderRef);
				spod->SpOrderStatus = SP_ACT_ORDER;
				SetEvent(spod->OrderStatusChgEvent);
			}
			break;
		case SP_ACT_ORDER://主动腿已报，待成交
			{
				printf("主动腿已报，待成交:%c\n", spod->SpOrderStatus);
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
					od2.Dir == '0' ? od2.Price - 2 : od2.Price + 2);
				sprintf_s(spod->pPasOrder.OrderRef, "%d", OrderRef);
				spod->SpOrderStatus = SP_PAS_ORDER;
				SetEvent(spod->OrderStatusChgEvent);
			}
			break;

		case SP_PAS_ORDER://被动腿已报，待成交
			{
				//可以添加对未成交单子的判断
				Sleep(5000);
				if (spod->SpOrderStatus == SP_PAS_ORDER)
				{
					pTdSpi->ReqOrderCancelByOrdRef(spod->pPasOrder.InstrumentID, spod->pPasOrder.OrderRef);
				}
				while (spod->pPasOrder.OrderStatus != THOST_FTDC_OST_Canceled)
				{}
				//撤单成功，重新报单
				int OrderRef = pTdSpi->ReqOrdLimit(spod->pPasOrder.InstrumentID,
					spod->pPasOrder.Direction,
					spod->pPasOrder.CombOffsetFlag[0],
					spod->pPasOrder.VolumeTotal,
					spod->pPasOrder.Direction == '0' ? spod->pPasOrder.LimitPrice + 5 : spod->pPasOrder.LimitPrice - 5);
				sprintf_s(spod->pPasOrder.OrderRef, "%d", OrderRef);
				spod->SpOrderStatus = SP_PAS_ORDER;
			}
			break;
		case SP_PAS_FILL://被动腿已成交
			return;

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

int SpStgyExec::GetInstPos(string Inst, char Dir)
{
	for (size_t i = 0; i < m_InstPos.size();i++)
	{
		if (m_InstPos[i].Inst == Inst && m_InstPos[i].Dir == Dir)
			return m_InstPos[i].PosVol;
	}
	return 0;	
}

//根据合约的持仓，校验开平仓字段
char SpStgyExec::CheckOrderOffset(ComOrder od)
{
	int PosVol = GetInstPos(od.Inst, '0' + '1' - od.Dir);
	if (PosVol >= od.Vol)
		return THOST_FTDC_OF_Close;
	else
		return THOST_FTDC_OF_Open;
}

//价差单拆成主动腿单
ComOrder SpStgyExec::GetActOrder(CtpSpOrder spod)
{
	ComOrder od;
	od.Inst = m_StgyCfg.ActiveInst.code;
	od.Dir = spod.Direction;
	od.Vol = m_StgyCfg.ActiveInst.orderVolRatio * spod.Vol;
	od.Price = (od.Dir == THOST_FTDC_D_Buy) ?
		m_mInstTick[od.Inst].AskPrice1 : m_mInstTick[od.Inst].BidPrice1;
	od.Offset = CheckOrderOffset(od);
	return od;
}
//价差单拆成被动腿单
ComOrder SpStgyExec::GetPasOrder(CtpSpOrder spod)
{
	ComOrder od;
	od.Inst = m_StgyCfg.PassiveInst.code;
	od.Dir = '1' + '0' - spod.Direction;
	od.Vol = m_StgyCfg.PassiveInst.orderVolRatio * spod.Vol;
	od.Price = (od.Dir == THOST_FTDC_D_Buy) ?
		m_mInstTick[od.Inst].AskPrice1 : m_mInstTick[od.Inst].BidPrice1;
	od.Offset = CheckOrderOffset(od);
	return od;
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

//解析公式
Formula SpStgyExec::ParseMathFormula(string strMathF)
{
	Formula f;
	string ratio;
	for (unsigned int i = 0; i < strMathF.length();i++)
	{
		char c = strMathF[i];
		if (c == '-' || c == '/')
		{
			f.op = c;			
			continue;
		}
		if (c == 'p' && strMathF[i+1] == '1')
		{
			f.ratio1 = atof(ratio.c_str());
			f.mode += 'A';
			ratio = "";
			i++;
			continue;
		}
		if (c == 'p' && strMathF[i + 1] == '2')
		{
			f.ratio2 = atof(ratio.c_str());
			f.mode += 'P';
			ratio = "";
			i++;
			continue;
		}
		ratio += c;
	}
	return f;
}

SpTick SpStgyExec::CalCurSpTick()
{
	SpTick spt;
	Formula f;
	f = m_SpStgy->m_StgyCfg.StgyFormula;
	if (f.op == '-')
	{
		if (f.mode == "AP")
		{
			spt.Spread = f.ratio1*m_mInstTick[ActInstCode].LastPrice - f.ratio2*m_mInstTick[PasInstCode].LastPrice;
			spt.SpreadAsk1P = f.ratio1*m_mInstTick[ActInstCode].AskPrice1 - f.ratio2*m_mInstTick[PasInstCode].BidPrice1;
			spt.SpreadBid1P = f.ratio1*m_mInstTick[ActInstCode].BidPrice1 - f.ratio2*m_mInstTick[PasInstCode].AskPrice1;
			spt.SpreadAsk1V = m_mInstTick[ActInstCode].AskVolume1 > m_mInstTick[PasInstCode].BidVolume1 ?
				m_mInstTick[PasInstCode].BidVolume1 : m_mInstTick[ActInstCode].AskVolume1;
			spt.SpreadBid1V = m_mInstTick[ActInstCode].BidVolume1 > m_mInstTick[PasInstCode].AskVolume1 ?
				m_mInstTick[PasInstCode].AskVolume1 : m_mInstTick[ActInstCode].BidVolume1;

			return spt;
		}
		if (f.mode == "PA")
		{
			spt.Spread = f.ratio2*m_mInstTick[PasInstCode].LastPrice - f.ratio1*m_mInstTick[ActInstCode].LastPrice;
			spt.SpreadAsk1P = f.ratio2*m_mInstTick[PasInstCode].AskPrice1 - f.ratio1*m_mInstTick[ActInstCode].BidPrice1;
			spt.SpreadBid1P = f.ratio2*m_mInstTick[PasInstCode].BidPrice1 - f.ratio1*m_mInstTick[ActInstCode].AskPrice1;
			spt.SpreadAsk1V = m_mInstTick[PasInstCode].AskVolume1 > m_mInstTick[ActInstCode].BidVolume1 ?
				m_mInstTick[ActInstCode].BidVolume1 : m_mInstTick[PasInstCode].AskVolume1;
			spt.SpreadBid1V = m_mInstTick[PasInstCode].BidVolume1 > m_mInstTick[ActInstCode].AskVolume1 ?
				m_mInstTick[ActInstCode].AskVolume1 : m_mInstTick[PasInstCode].BidVolume1;

			return spt;
		}
		
	}
	if (f.op == '/')
	{
		if (f.mode == "AP")
		{
			spt.Spread = f.ratio1*m_mInstTick[ActInstCode].LastPrice / f.ratio2*m_mInstTick[PasInstCode].LastPrice;
			spt.SpreadAsk1P = f.ratio1*m_mInstTick[ActInstCode].AskPrice1 / f.ratio2*m_mInstTick[PasInstCode].BidPrice1;
			spt.SpreadBid1P = f.ratio1*m_mInstTick[ActInstCode].BidPrice1 / f.ratio2*m_mInstTick[PasInstCode].AskPrice1;
			spt.SpreadAsk1V = m_mInstTick[ActInstCode].AskVolume1 > m_mInstTick[PasInstCode].BidVolume1 ?
				m_mInstTick[PasInstCode].BidVolume1 : m_mInstTick[ActInstCode].AskVolume1;
			spt.SpreadBid1V = m_mInstTick[ActInstCode].BidVolume1 > m_mInstTick[PasInstCode].AskVolume1 ?
				m_mInstTick[PasInstCode].AskVolume1 : m_mInstTick[ActInstCode].BidVolume1;

			return spt;
		}
		if (f.mode == "PA")
		{
			spt.Spread = f.ratio2*m_mInstTick[PasInstCode].LastPrice / f.ratio1*m_mInstTick[ActInstCode].LastPrice;
			spt.SpreadAsk1P = f.ratio2*m_mInstTick[PasInstCode].AskPrice1 / f.ratio1*m_mInstTick[ActInstCode].BidPrice1;
			spt.SpreadBid1P = f.ratio2*m_mInstTick[PasInstCode].BidPrice1 / f.ratio1*m_mInstTick[ActInstCode].AskPrice1;
			spt.SpreadAsk1V = m_mInstTick[PasInstCode].AskVolume1 > m_mInstTick[ActInstCode].BidVolume1 ?
				m_mInstTick[ActInstCode].BidVolume1 : m_mInstTick[PasInstCode].AskVolume1;
			spt.SpreadBid1V = m_mInstTick[PasInstCode].BidVolume1 > m_mInstTick[ActInstCode].AskVolume1 ?
				m_mInstTick[ActInstCode].AskVolume1 : m_mInstTick[PasInstCode].BidVolume1;

			return spt;
		}
	}
	return spt;
}

//行情推给策略
void SpStgyExec::OnCtpRtnTick(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	//数据存储
	m_mInstTick[pDepthMarketData->InstrumentID] = *pDepthMarketData;
	//数据计算
	SpTick spt = CalCurSpTick();
	//数据处理
	m_curSpTick = spt;
	m_vSpTickL.push_back(spt);
	//设置信号量
	SetEvent(m_MDEvent);
	//推送给策略
	m_SpStgy->RtnTick(spt);
}

//更新持仓
//前期不分今昨，后期再添加
int SpStgyExec::UpdateInstPos(CThostFtdcTradeField* pTrade)
{
	char Dir;
	if (pTrade->OffsetFlag == THOST_FTDC_OF_Open)
		Dir = pTrade->Direction;
	else
		Dir = '0' + '1' - pTrade->Direction;

	for (size_t i = 0; i < m_InstPos.size(); i++)
	{
		if (!strcmp(m_InstPos[i].Inst.c_str(), pTrade->InstrumentID) && m_InstPos[i].Dir == Dir)
		{
			if (pTrade->OffsetFlag == THOST_FTDC_OF_Open)
				m_InstPos[i].PosVol += pTrade->Volume;
			else
				m_InstPos[i].PosVol -= pTrade->Volume;
			return 0;
		}
	}

	//没有找到的话，新增一个（必定是新开仓的单）
	if (pTrade->OffsetFlag != THOST_FTDC_OF_Open) 
		return -1;
	InstPos ipos;
	ipos.Dir = pTrade->Direction;
	ipos.Inst = pTrade->InstrumentID;
	ipos.PosVol = pTrade->Volume;
	ipos.TdPosVol = pTrade->Volume;
	ipos.YdPosVol = 0;
	m_InstPos.push_back(ipos);
	return 0;
}

void SpStgyExec::OnCtpRtnTrade(CThostFtdcTradeField* pTrade)
{
	int rlt = UpdateInstPos(pTrade);
	if (rlt)
	{
		printf("更新持仓错误\n");
	}
}

void SpStgyExec::OnCtpRtnOrder(CThostFtdcOrderField* pOrder)
{
	m_log->RecordLog("return order");
	for (size_t i = 0; i < m_vAllSpOd.size(); i++)
	{
		if (!strcmp(m_vAllSpOd[i].pActOrder.OrderRef,pOrder->OrderRef))
		{
			
			m_vAllSpOd[i].pActOrder = *pOrder;
			m_log->RecordLog("copy");
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
					m_vAllSpOd[i].SpOrderStatus = SP_PAS_FILL;
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
	strcpy(spod.pActOrder.InstrumentID, ActInstCode.c_str());
	strcpy(spod.pPasOrder.InstrumentID, PasInstCode.c_str());
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
				printf("发出撤单指令\n");
				m_log->RecordLog("cancel");
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
	this->m_StgyCfg = astgycfg;
	ActInstCode = m_StgyCfg.ActiveInst.code;
	PasInstCode = m_StgyCfg.PassiveInst.code;
}

vector<CThostFtdcInvestorPositionField> SpStgyExec::GetCTPCurPosition()
{
	m_CtpPosition =  pTdSpi->ReqQryIvstPosition();
	if(m_CtpPosition.size()!=0)
	{
		m_InstPos.clear();

		for (size_t i = 0; i < m_CtpPosition.size();i++)
		{
			InstPos ipos;
			ipos.Inst = m_CtpPosition[i].InstrumentID;
			ipos.Dir = (m_CtpPosition[i].PosiDirection == THOST_FTDC_PD_Long)?THOST_FTDC_D_Buy:THOST_FTDC_D_Sell;
			ipos.PosVol = m_CtpPosition[i].Position;
			ipos.TdPosVol = m_CtpPosition[i].TodayPosition;
			ipos.YdPosVol = m_CtpPosition[i].Position - m_CtpPosition[i].TodayPosition;
			m_InstPos.push_back(ipos);
		}
	}
	return m_CtpPosition;
}
CThostFtdcTradingAccountField SpStgyExec::GetCTPCurAccoutMoney()
{
	m_CtpActInfo = pTdSpi->ReqQryTradingAccount();
	return m_CtpActInfo;
}
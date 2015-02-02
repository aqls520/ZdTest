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

//����ִ��ģ��
void OrderExecer(LPVOID p)
{
	ExecModule* pEM = (ExecModule*)p;
	//���ݲ������ý��б���
	pEM->pExec->ExecAOrder(pEM->spod);
}

//������Ϊ��������ⱨ������������������������ã���ִ�б���
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
//��ⱨ����������
bool SpStgyExec::CheckOrderTouch(CtpSpOrder spod)
{
	return true;
	if (spod.Direction == THOST_FTDC_D_Buy)
	{
		//�۲�У��
		if (spod.OrderSpread < m_curSpTick.SpreadAsk1P) 
			return false;
		//�̿�У��:��ȫ��ȴ���0��������С�̿ڼ��
		if (m_StgyCfg.PassiveInst.saveDepth <= 0) 
			return true;
		//�����С�̿�
		if (m_mInstTick[PasInstCode].BidVolume1 < m_StgyCfg.PassiveInst.minOrderBookVol)
			return false;
		else
			return true;
	}
	else
	{
		//�۲�У��
		if (spod.OrderSpread > m_curSpTick.SpreadBid1P)
			return false;
		//�̿�У��:��ȫ��ȴ���0��������С�̿ڼ��
		if (m_StgyCfg.PassiveInst.saveDepth <= 0)
			return true;
		//�����С�̿�
		if (m_mInstTick[PasInstCode].AskVolume1 < m_StgyCfg.PassiveInst.minOrderBookVol)
			return false;
		else
			return true;
	}
}


void SpStgyExec::InitSpi()
{
	//��ʼ������ӿ�
	pQtSpi = new CtpQtSpi();
	pQtSpi->RegisterStgyExec(this);
	pQtSpi->Init();

	//��ʼ�����׽ӿ�
	pTdSpi = new CtpTdSpi();
	pTdSpi->RegisterStgyExec(this);
	pTdSpi->Init();

}

//��������ִ��
void SpStgyExec::ExecAOrder(CtpSpOrder* spod)
{
	while (true)
	{
		switch (spod->SpOrderStatus)
		{
		case SP_NOTTOUCH: //δ�������������
			{
				while (true)
				{
					//�ȴ�����仯��У�鱨����������
					WaitForSingleObject(m_MDEvent, INFINITE);
					//У��ɹ����˳�ѭ�����޸ı���״̬�仯
					if (CheckOrderTouch(*spod))
					{
						printf("����");
						spod->SpOrderStatus = SP_TOUCH;
						SetEvent(spod->OrderStatusChgEvent);
						break;
					}
				}
			}
			break;
		case SP_TOUCH://�Ѵ����������������µ�
			{
				//�����ȳɽ����ٱ�������
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
		case SP_ACT_ORDER://�������ѱ������ɽ�
			{
				printf("�������ѱ������ɽ�:%c\n", spod->SpOrderStatus);
			}
			break;
		case SP_ACT_FILL://�������ѳɽ�
			{
				//������ȫ���ɽ��������ȱ���
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

		case SP_PAS_ORDER://�������ѱ������ɽ�
			{
				//������Ӷ�δ�ɽ����ӵ��ж�
				Sleep(5000);
				if (spod->SpOrderStatus == SP_PAS_ORDER)
				{
					pTdSpi->ReqOrderCancelByOrdRef(spod->pPasOrder.InstrumentID, spod->pPasOrder.OrderRef);
				}
				while (spod->pPasOrder.OrderStatus != THOST_FTDC_OST_Canceled)
				{}
				//�����ɹ������±���
				int OrderRef = pTdSpi->ReqOrdLimit(spod->pPasOrder.InstrumentID,
					spod->pPasOrder.Direction,
					spod->pPasOrder.CombOffsetFlag[0],
					spod->pPasOrder.VolumeTotal,
					spod->pPasOrder.Direction == '0' ? spod->pPasOrder.LimitPrice + 5 : spod->pPasOrder.LimitPrice - 5);
				sprintf_s(spod->pPasOrder.OrderRef, "%d", OrderRef);
				spod->SpOrderStatus = SP_PAS_ORDER;
			}
			break;
		case SP_PAS_FILL://�������ѳɽ�
			return;

		//ȫ���ɽ�/����������ִ����ϣ��˳��߳�
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

//���ݺ�Լ�ĳֲ֣�У�鿪ƽ���ֶ�
char SpStgyExec::CheckOrderOffset(ComOrder od)
{
	int PosVol = GetInstPos(od.Inst, '0' + '1' - od.Dir);
	if (PosVol >= od.Vol)
		return THOST_FTDC_OF_Close;
	else
		return THOST_FTDC_OF_Open;
}

//�۲��������ȵ�
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
//�۲��ɱ����ȵ�
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

//������ʽ
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

//�����Ƹ�����
void SpStgyExec::OnCtpRtnTick(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	//���ݴ洢
	m_mInstTick[pDepthMarketData->InstrumentID] = *pDepthMarketData;
	//���ݼ���
	SpTick spt = CalCurSpTick();
	//���ݴ���
	m_curSpTick = spt;
	m_vSpTickL.push_back(spt);
	//�����ź���
	SetEvent(m_MDEvent);
	//���͸�����
	m_SpStgy->RtnTick(spt);
}

//���³ֲ�
//ǰ�ڲ��ֽ��򣬺��������
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

	//û���ҵ��Ļ�������һ�����ض����¿��ֵĵ���
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
		printf("���³ֲִ���\n");
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
			//ͬ��ʱ��ֻ����һ��OrderAdapter��ִ�У���֤���Զ���˳����ջر�
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
			if (m_vAllSpOd[i].SpOrderStatus < '4')//������δ�ɽ���������
			{
				ei.ErrorID = 0;
				ei.ErrorMsg = "";
				printf("��������ָ��\n");
				m_log->RecordLog("cancel");
				CancelAOrder(&m_vAllSpOd[i]);
				return ei;
			}
			else
			{
				//�ҵ����������������������Ѿ��ɽ��ˣ���ʱ��������
				ei.ErrorID = 2;
				ei.ErrorMsg = "����״̬��������г�������";
				return ei;
			}
		}
	}
	//û���ҵ����������ش�����
	ei.ErrorID = 1;
	ei.ErrorMsg = "û���ҵ��ü۲����ȷ�ϼ۲�������Ƿ����";
	return ei;
}

//���ĺ�Լ
void SpStgyExec::SubMarketData(StgyConfig astgycfg)
{
	char* pInst[2];
	pInst[0] = new char[200];
	pInst[1] = new char[200];

	strcpy(pInst[0], astgycfg.ActiveInst.code.c_str());
	strcpy(pInst[1], astgycfg.PassiveInst.code.c_str());

	pQtSpi->SubMarketData(pInst, 2);
}

//�ϴ���������
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
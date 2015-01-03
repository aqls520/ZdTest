#include "SpStgyExec.h"

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
}
SpStgyExec::~SpStgyExec()
{
	DeleteCriticalSection(&m_cs);
}

void SpStgyExec::ExecAOrder(SpOrder)
{

}

//行情推给策略
void SpStgyExec::OnTick(SpTick t)
{
	//数据处理
	m_curSpTick = t;
	m_vSpTickL.push_back(t);
	SetEvent(m_Event);//接收到行情，开始Check报单

	m_SpStgy->RtnTick(t);
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





//预留接口
//策略配置更新
void SpStgyExec::UpdateStgyCfg(StgyConfig* aStygCfg)
{
	m_MyStgyCfg = *aStygCfg; 
}
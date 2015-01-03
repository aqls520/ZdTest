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

//�����Ƹ�����
void SpStgyExec::OnTick(SpTick t)
{
	//���ݴ���
	m_curSpTick = t;
	m_vSpTickL.push_back(t);
	SetEvent(m_Event);//���յ����飬��ʼCheck����

	m_SpStgy->RtnTick(t);
}

//�����仯�˾��Ƹ�����
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





//Ԥ���ӿ�
//�������ø���
void SpStgyExec::UpdateStgyCfg(StgyConfig* aStygCfg)
{
	m_MyStgyCfg = *aStygCfg; 
}
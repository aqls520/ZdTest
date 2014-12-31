#pragma once

#include "ZdStruct.h"
#include "SpreadStgy.h"
#include <map>
#include <vector>
#include <windows.h>
using namespace std;

typedef void* LPVOID;

class SpStgyExec
{
public:
	SpStgyExec();
	~SpStgyExec();

private:
	//�����Ƹ�����
	void OnTick(SpTick t);
	//�����仯�˾��Ƹ�����
	void OnOrder(SpOrder);

	void ExecAOrder(SpOrder);

	//Ԥ���ӿ�
	//�������ø���
	void UpdateStgyCfg(StgyConfig* aStygCfg);


public:
	void SendSpOrder(SpOrder spod);
	
public:
	StgyConfig m_MyStgyCfg;
	LPVOID pTradeSpiAct;
	LPVOID pTradeSpiPas;
	map<string,Tick> m_mInstTick;
	SpreadStgy* m_SpStgy;
	vector<SpOrder> m_vSpOdList;
	SpOrder m_CurSpOrder;
	CRITICAL_SECTION m_cs;
	HANDLE m_Event;
	SpTick m_curSpTick;
	vector<SpTick> m_vSpTickL;

};


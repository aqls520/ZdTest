/*
������Ե���
*/
#pragma once
#include "SpreadStgy.h"
#include "SpStgyExec.h"

#include "ZdSpi.h"


class SpreadMgr
{
public:
	SpreadMgr();
	~SpreadMgr();

	
	//ע��ִ��ģ��Ự��
	int RegisterExecSession(SpreadStgy*);
	
	//����ִ��ģ��
	SpStgyExec* CreateStgyExec();

private:
	//����ָ��
	map<int,SpStgyExec*> m_StgyExecList;
	//����ִ����ָ��
	map<int,SpreadStgy*> m_SpStgyList;
	//�Ự��
	int m_SessionID;

	CtpQtSpi* pQtSpi;
	CtpTdSpi* pTdSpi;

};




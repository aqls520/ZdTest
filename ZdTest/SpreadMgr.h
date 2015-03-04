/*
处理策略的类
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

	
	//注册执行模块会话号
	int RegisterExecSession(SpreadStgy*);
	
	//创建执行模块
	SpStgyExec* CreateStgyExec();

private:
	//策略指针
	map<int,SpStgyExec*> m_StgyExecList;
	//策略执行类指针
	map<int,SpreadStgy*> m_SpStgyList;
	//会话号
	int m_SessionID;

	CtpQtSpi* pQtSpi;
	CtpTdSpi* pTdSpi;

};




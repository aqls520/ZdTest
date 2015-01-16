#pragma once

#include <string>
#include "ZdStruct.h"
#include "ZdDefine.h"
#include "SpStgyExec.h"

using namespace std;

class SpreadStgy
{
public:
	SpreadStgy();
	~SpreadStgy();

public:
	void Init();
	void RtnConnect();
	void RtnTick(SpTick);
	void RtnOrderFill(CtpSpOrder od);
	void RtnOrderCancel(CtpSpOrder od);
	void RtnOrderFail(CtpSpOrder od);
public:
 	StgyConfig m_MyStgyCfg;
 	SpStgyExec* m_thisSpExec;
	
	
};


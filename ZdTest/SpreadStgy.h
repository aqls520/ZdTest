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
	void RtnTick(SpTick);
	void RtnOrderFill(CtpSpOrder od);
	void RtnOrderCancel(CtpSpOrder od);
	void RtnOrderFail(CtpSpOrder od);

private:
	void LoadStgyCfg(char* cfgpath);

public:
 	StgyConfig m_StgyCfg;
 	SpStgyExec* m_SpSE;
	vector<SpTick> m_vSpTickL;
	double sp_high, sp_low;
};


#pragma once

#include <string>
#include "ZdStruct.h"
#include "ZdDefine.h"
using namespace std;

class SpreadStgy
{
public:
	SpreadStgy();
	~SpreadStgy();

public:
	void Init();
	void RtnConnect();
	void RtnLogin();
	void RtnTick(SpTick);
	void RtnOrderFill(SpOrder od);
	void RtnOrderCancel(SpOrder od);
	void RtnOrderFail(SpOrder od);

public:
	void SubMarketData(StgyConfig);
	void LoadStgyCfg(char* cfgpath);
	void UpLoadStgyCfg(StgyConfig);


public:
	StgyConfig m_MyStgyCfg;
	
	
};


#include "SpreadStgy.h"


SpreadStgy::SpreadStgy()
{
}


SpreadStgy::~SpreadStgy()
{
}

void SpreadStgy::Init(){}
void SpreadStgy::RtnConnect(){}
void SpreadStgy::RtnLogin(){}
void SpreadStgy::RtnTick(SpTick){}
void SpreadStgy::RtnOrderFill(SpOrder od){}
void SpreadStgy::RtnOrderCancel(SpOrder od){}
void SpreadStgy::RtnOrderFail(SpOrder od){}
void SpreadStgy::SubMarketData(StgyConfig){}
void SpreadStgy::LoadStgyCfg(char* cfgpath){}
void SpreadStgy::UpLoadStgyCfg(StgyConfig){}
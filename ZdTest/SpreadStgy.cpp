#include "SpreadStgy.h"


SpreadStgy::SpreadStgy()
{

}


SpreadStgy::~SpreadStgy()
{
}

void SpreadStgy::Init(){}
void SpreadStgy::RtnConnect(){}
void SpreadStgy::RtnTick(SpTick spt)
{
	printf("sp=%f,spb1p=%f,spa1p=%f,spb1v=%d,spa1v=%d\n",
			spt.Spread,spt.SpreadBid1P,spt.SpreadAsk1P,spt.SpreadBid1V,spt.SpreadAsk1V);

}
void SpreadStgy::RtnOrderFill(SpOrder od){}
void SpreadStgy::RtnOrderCancel(SpOrder od){}
void SpreadStgy::RtnOrderFail(SpOrder od){}

void SpreadStgy::RtnOrderFill(CtpSpOrder od){}
void SpreadStgy::RtnOrderCancel(CtpSpOrder od){}
void SpreadStgy::RtnOrderFail(CtpSpOrder od){}
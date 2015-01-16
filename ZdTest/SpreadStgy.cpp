#include "SpreadStgy.h"


SpreadStgy::SpreadStgy()
{

}


SpreadStgy::~SpreadStgy()
{
}

void SpreadStgy::Init(){}
void SpreadStgy::RtnConnect(){}

int i=1;

void SpreadStgy::RtnTick(SpTick spt)
{
	printf("sp=%f,spb1p=%f,spa1p=%f,spb1v=%d,spa1v=%d\n",
			spt.Spread,spt.SpreadBid1P,spt.SpreadAsk1P,spt.SpreadBid1V,spt.SpreadAsk1V);
	i++;
	if (i%20==0)
	{
		CtpSpOrder spod;
		spod.OrderAction = SendOrder;
		spod.OrderSpread = spt.Spread;
		spod.Direction = THOST_FTDC_D_Buy;
		spod.SpOrderRef = i;
		strcpy(spod.pActOrder.InstrumentID,"IF1502");
		strcpy(spod.pPasOrder.InstrumentID, "IF1501");
		this->m_thisSpExec->SendSpOrder(spod);
	}
	


}
void SpreadStgy::RtnOrderFill(CtpSpOrder od){}
void SpreadStgy::RtnOrderCancel(CtpSpOrder od){}
void SpreadStgy::RtnOrderFail(CtpSpOrder od){}
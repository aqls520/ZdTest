#include "SpreadStgy.h"


SpreadStgy::SpreadStgy()
{

}


SpreadStgy::~SpreadStgy()
{
}

void SpreadStgy::Init()
{
	//加载策略配置
	this->m_MyStgyCfg.ActiveInst.code = "IF1503";
	this->m_MyStgyCfg.PassiveInst.code = "IF1502";
	m_MyStgyCfg.ActiveInst.orderVolRatio = 1;
	m_MyStgyCfg.PassiveInst.orderVolRatio = 2;
	//上传策略配置
	this->m_thisSpExec->UpLoadStgyCfg(m_MyStgyCfg);
}
void SpreadStgy::RtnConnect(){}

int i=1;

void SpreadStgy::RtnTick(SpTick spt)
{
	printf("sp=%f,spb1p=%f,spa1p=%f,spb1v=%d,spa1v=%d\n",
			spt.Spread,spt.SpreadBid1P,spt.SpreadAsk1P,spt.SpreadBid1V,spt.SpreadAsk1V);
	i++;
	if (i%40==0)
	{
		CtpSpOrder spod;
		spod.OrderAction = SendOrder;
		spod.OrderSpread = spt.Spread;
		spod.Direction = THOST_FTDC_D_Buy;
		spod.SpOrderRef = i;
		spod.Vol = 1;
		this->m_thisSpExec->SendSpOrder(spod);
	}
}
void SpreadStgy::RtnOrderFill(CtpSpOrder od){}
void SpreadStgy::RtnOrderCancel(CtpSpOrder od){}
void SpreadStgy::RtnOrderFail(CtpSpOrder od){}
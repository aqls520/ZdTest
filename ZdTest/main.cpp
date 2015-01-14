#include "ZdSpi.h"
#include "SpreadStgy.h"

void main()
{
	SpreadStgy* spstgy = new SpreadStgy();
	spstgy->m_MyStgyCfg.ActiveInst.code = "IF1502";
	spstgy->m_MyStgyCfg.PassiveInst.code = "IF1501";

	spstgy->m_MyStgyCfg.ActiveInst.code = "cu1504";
	spstgy->m_MyStgyCfg.PassiveInst.code = "cu1503";

	SpStgyExec* pStgyExec = new SpStgyExec();
	spstgy->m_thisSpExec = pStgyExec;
	spstgy->m_thisSpExec->RegisterExec(spstgy);
	spstgy->m_thisSpExec->SubMarketData(spstgy->m_MyStgyCfg);

	getchar();
}
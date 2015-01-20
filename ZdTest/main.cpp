#include "ZdSpi.h"
#include "SpreadStgy.h"

void main()
{
	SpreadStgy* spstgy = new SpreadStgy();
	SpStgyExec* pStgyExec = new SpStgyExec();
	spstgy->m_thisSpExec = pStgyExec;
	spstgy->Init();

	spstgy->m_thisSpExec->RegisterExec(spstgy);
	spstgy->m_thisSpExec->UpLoadStgyCfg(spstgy->m_MyStgyCfg);
	spstgy->m_thisSpExec->SubMarketData(spstgy->m_MyStgyCfg);

	getchar();
}
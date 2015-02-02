#include "ZdSpi.h"
#include "SpreadStgy.h"

void main()
{
	SpreadStgy* pSpStgy = new SpreadStgy();
	SpStgyExec* pStgyExec = new SpStgyExec(pSpStgy);
	pStgyExec->Init();
	pSpStgy->Init();

	getchar();
}
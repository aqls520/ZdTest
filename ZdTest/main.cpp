#include "ZdSpi.h"

void main()
{
	ZdSpi* pSpi = new ZdSpi();
	pSpi->Init();
	getchar();
}
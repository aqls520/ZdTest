#include "SpreadStgy.h"
#include <fstream>


SpreadStgy::SpreadStgy()
{}
SpreadStgy::~SpreadStgy()
{}

//加载策略配置
void SpreadStgy::LoadStgyCfg(char* cfgpath)
{
	fstream fin;
	fin.open(cfgpath, ios::in);

	string section;

	while (!fin.eof())
	{
		string cfgdata;
		int idx_begin = 0;
		int idx_split = 0;
		getline(fin, cfgdata);
		if (cfgdata.length() < 1)
			continue;
		if (cfgdata.substr(0,2) == "//")
			continue;
		if (!strcmp("[Main]",cfgdata.c_str()))
		{
			section = "main";
		}
		if (!strcmp("[ActiveInstrument]", cfgdata.c_str()))
		{
			section = "ActiveInstrument";
		}
		if (!strcmp("[PassiveInstrument]", cfgdata.c_str()))
		{
			section = "PassiveInstrument";
		}
		
		if (section == "main")
		{
			idx_split = cfgdata.find('=', idx_begin);
			string key = cfgdata.substr(idx_begin, idx_split);
			string keyvalue = cfgdata.substr(idx_split + 1, cfgdata.length());
			if (key == "StgyCode")
				m_StgyCfg.StgyCode = keyvalue;
			if (key == "StgyName")
				m_StgyCfg.StgyName = keyvalue;
			if (key == "Formula")
				m_StgyCfg.strStgyFormula = keyvalue;
			if (key == "PriceTick")
				m_StgyCfg.PriceTick = atof(keyvalue.c_str());
			if (key == "ActiveBatchVol")
				m_StgyCfg.ActiveBatchVol = atoi(keyvalue.c_str());
		}

		if (section == "ActiveInstrument")
		{
			idx_split = cfgdata.find('=', idx_begin);
			string key = cfgdata.substr(idx_begin, idx_split);
			string keyvalue = cfgdata.substr(idx_split + 1, cfgdata.length());
			if (key == "code")
				m_StgyCfg.ActiveInst.code = keyvalue;
			if (key == "exchange")
				m_StgyCfg.ActiveInst.exchange = keyvalue;
			if (key == "dir")
				m_StgyCfg.ActiveInst.dir = keyvalue;
			if (key == "orderType")
				m_StgyCfg.ActiveInst.orderType = keyvalue;
			if (key == "isActive")
				m_StgyCfg.ActiveInst.isActive = (keyvalue == "1") ? true : false;
			if (key == "orderVolRatio")
				m_StgyCfg.ActiveInst.orderVolRatio = atof(keyvalue.c_str());
			if (key == "minOrderBookVol")
				m_StgyCfg.ActiveInst.minOrderBookVol = atoi(keyvalue.c_str());
			if (key == "saveDepth")
				m_StgyCfg.ActiveInst.saveDepth = atoi(keyvalue.c_str());
			if (key == "priceTolerance")
				m_StgyCfg.ActiveInst.priceTolerance = atoi(keyvalue.c_str());
			
		}
		if (section == "PassiveInstrument")
		{
			idx_split = cfgdata.find('=', idx_begin);
			string key = cfgdata.substr(idx_begin, idx_split);
			string keyvalue = cfgdata.substr(idx_split + 1, cfgdata.length());
			if (key == "code")
				m_StgyCfg.PassiveInst.code = keyvalue;
			if (key == "exchange")
				m_StgyCfg.PassiveInst.exchange = keyvalue;
			if (key == "dir")
				m_StgyCfg.PassiveInst.dir = keyvalue;
			if (key == "orderType")
				m_StgyCfg.PassiveInst.orderType = keyvalue;
			if (key == "isActive")
				m_StgyCfg.PassiveInst.isActive = (keyvalue == "1") ? true : false;
			if (key == "orderVolRatio")
				m_StgyCfg.PassiveInst.orderVolRatio = atof(keyvalue.c_str());
			if (key == "minOrderBookVol")
				m_StgyCfg.PassiveInst.minOrderBookVol = atoi(keyvalue.c_str());
			if (key == "saveDepth")
				m_StgyCfg.PassiveInst.saveDepth = atoi(keyvalue.c_str());
			if (key == "priceTolerance")
				m_StgyCfg.PassiveInst.priceTolerance = atoi(keyvalue.c_str());
		}
	}
	fin.close();
}

void SpreadStgy::Init()
{
	//加载策略配置
	LoadStgyCfg("StrategyCfg/StgyCfg.txt");
	m_StgyCfg.StgyFormula = m_SpSE->ParseMathFormula(m_StgyCfg.strStgyFormula);
	//上传策略配置
	m_SpSE->UpLoadStgyCfg(m_StgyCfg);

	//策略初始化-账户信息查询（资金、持仓）
	vector<CThostFtdcInvestorPositionField> tmp = m_SpSE->GetCTPCurPosition();
	Sleep(1000);
	CThostFtdcTradingAccountField t = m_SpSE->GetCTPCurAccoutMoney();

	m_SpSE->SubMarketData(m_StgyCfg);

	sp_high = 0;
	sp_low = 1000;
}

int i=1;
int ordRef = 1;

void SpreadStgy::RtnTick(SpTick spt)
{
	/*printf("sp=%f,spb1p=%f,spa1p=%f,spb1v=%d,spa1v=%d\n",
			spt.Spread,spt.SpreadBid1P,spt.SpreadAsk1P,spt.SpreadBid1V,spt.SpreadAsk1V);*/
	if (spt.Spread > 1000)
		return;

	m_vSpTickL.push_back(spt);

	if (i<=10)
	{
		sp_high = sp_high > spt.Spread ? sp_high : spt.Spread;
		sp_low = sp_low < spt.Spread ? sp_low : spt.Spread;
		i++;
		return;
	}

	if (spt.Spread > sp_high)
	{
		printf("high=%f,sp=%f\n", sp_high, spt.Spread);
		CtpSpOrder spod;
		spod.OrderAction = SendOrder;
		spod.OrderSpread = spt.Spread;
		spod.Direction = THOST_FTDC_D_Sell;
		spod.SpOrderRef = ordRef++;
		spod.Vol = 1;
		m_SpSE->SendSpOrder(spod);
		//sp_high = 1000;
		return;
	}
	if (spt.Spread < sp_low)
	{
		printf("low=%f,sp=%f\n", sp_low, spt.Spread);
		CtpSpOrder spod;
		spod.OrderAction = SendOrder;
		spod.OrderSpread = spt.Spread;
		spod.Direction = THOST_FTDC_D_Buy;
		spod.SpOrderRef = ordRef++;
		spod.Vol = 1;
		m_SpSE->SendSpOrder(spod);
		//sp_low = 0;
		return;
	}
}
void SpreadStgy::RtnOrderFill(CtpSpOrder od){}
void SpreadStgy::RtnOrderCancel(CtpSpOrder od){}
void SpreadStgy::RtnOrderFail(CtpSpOrder od){}
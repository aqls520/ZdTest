#include "ZdSpi.h"
#include <iostream>
#include <windows.h>
#include <vector>

HANDLE g_Event = CreateEvent(NULL, false, false, NULL);

vector<ControlRtn> g_vCr;

#pragma region 原始API响应

int ZdSpi::OnReceiveTradeInfo(const CShZdMessage * re)
{
	cout << "OnReceiveTradeInfo" << endl;
	CShZdMessage* msg = (CShZdMessage*)re;
	cout << msg->GetAllString() <<endl;
	ChgRspFunction(msg);
	return 0;
}
int ZdSpi::OnReceiveMarketInfo(const CShZdMessage * re)
{
	//cout << "OnReceiveMarketInfo" << endl;
	CShZdMessage* msg = (CShZdMessage*)re;
	ChgRspFunction(msg);
	return 0;
}
int ZdSpi::OnReceiveErrorInfo(int errorCode, const char* re)
{
	cout << "OnReceiveErrorInfo:errorcode=" << errorCode<< "," << re<< endl;
	
	if (errorCode == 7 || errorCode == 17)
	{
		SetEvent(g_Event);
	}
	return 0;
}

#pragma endregion

#pragma region 请求函数

void ZdSpi::ReqLogin(LoginReq lr)
{
	CShZdMessage* req = GetZdReqMessage(Logon, &lr);
	InHandle->ShZdSendInfoToTrade(req);
}

void ZdSpi::ReqAccountSearch(AccountSearchReq acr)
{
	CShZdMessage* req = GetZdReqMessage(AccountSearch, &acr);
	InHandle->ShZdSendInfoToTrade(req);
}

void ZdSpi::ReqExchangeSearch(ExchangeSearchReq esr)
{
	CShZdMessage* req = GetZdReqMessage(ExchangeSearch, &esr);
	InHandle->ShZdSendInfoToTrade(req);
}

void ZdSpi::ReqOrder(OrderReq or)
{
	CShZdMessage* req = GetZdReqMessage(Order, &or);
	InHandle->ShZdSendInfoToTrade(req);
}

void ZdSpi::ReqModify(ModifyReq mr)
{
	CShZdMessage* req = GetZdReqMessage(Modify, &mr);
	InHandle->ShZdSendInfoToTrade(req);
}

void ZdSpi::ReqCancel(OrderCancelReq ocr)
{
	CShZdMessage* req = GetZdReqMessage(OrderCancel, &ocr);
	InHandle->ShZdSendInfoToTrade(req);
}

void ZdSpi::ReqGetUserControl(ControlReq cr)
{
	CShZdMessage* req = GetZdReqMessage(GetUseControl, &cr);
	InHandle->ShZdSendInfoToTrade(req);
}

void ZdSpi::ReqMarket(MarketReq mr)
{
	CShZdMessage* req = GetZdReqMessage(Market, &mr);
	InHandle->ShZdSendInfoToMarket(req);
}

void ZdSpi::ReqFillSearch(FillSearchReq fsr)
{
	CShZdMessage* req = GetZdReqMessage(FillSearch, &fsr);
	InHandle->ShZdSendInfoToTrade(req);
}

void ZdSpi::ReqOrderSearch(OrderSearchReq osr)
{
	CShZdMessage* req = GetZdReqMessage(OrderSearch, &osr);
	InHandle->ShZdSendInfoToTrade(req);
}

void ZdSpi::ReqOpenDetail(OpenDetailReq odr)
{
	CShZdMessage* req = GetZdReqMessage(OpenDetail, &odr);
	InHandle->ShZdSendInfoToTrade(req);
}

#pragma endregion

#pragma region 响应函数

void ZdSpi::RtnLoginReq(LoginRtn* lr,bool bIsLast)
{
	if (lr!=NULL)
	{
		cout << lr->accountNo << endl;
	}
	if (bIsLast)
	{
		SetEvent(g_Event);
	}
}

void ZdSpi::RtnAccountSearchReq(AccountSearchRtn* acr, bool bIsLast)
{
	if (acr != NULL)
	{
		cout << acr->todayBalance << endl;
	}
	if (bIsLast)
	{
		SetEvent(g_Event);
	}
}

void ZdSpi::RtnExchangeSearchReq(ExchangeSearchRtn* esr, bool bIsLast)
{
	if (esr != NULL)
	{
		cout << esr->exchangeCode << " " << esr->exchangeName << endl;
	}
	if (bIsLast)
	{
		SetEvent(g_Event);
	}
}

void ZdSpi::RtnGetUserControlReq(ControlRtn* cr,bool bIsLast)
{
	if (cr != NULL)
	{
		//cout << cr->code << endl;
		g_vCr.push_back(*cr);
	}
	if (bIsLast)
	{
		SetEvent(g_Event);
	}
}

void ZdSpi::RtnFillSearchReq(FillSearchRtn* fsr,bool bIsLast)
{
	if (fsr != NULL)
	{
		cout << fsr->systemNo <<endl;
	}
	if (bIsLast)
	{
		SetEvent(g_Event);
	}
}

void ZdSpi::RtnOrderSearchReq(OrderSearchRtn* osr,bool bIsLast)
{
	if (osr != NULL)
	{
		cout << osr->systemNo <<endl;
	}
	if (bIsLast)
	{
		SetEvent(g_Event);
	}
}

void ZdSpi::RtnOpenDetailReq(OpenDetailRtn* odr,bool bIsLast)
{
	if (odr != NULL)
	{
		cout << odr->filledNo <<endl;
	}
	if (bIsLast)
	{
		SetEvent(g_Event);
	}
}

void ZdSpi::RtnCancelReq(OrderCancelRtn* ocr,bool bIsLast)
{
	if (ocr != NULL)
	{
		cout << ocr->cancelNo <<endl;
	}
	if (bIsLast)
	{
		SetEvent(g_Event);
	}
}

int vol = 0;

void ZdSpi::RtnMarket(MarketRtn* mr)
{
	cout << mr->time << " " << mr->currPrice << " " << atoi(mr->filledNum.c_str())-vol<<endl;
	//vol = atoi(mr->filledNum.c_str());
}

void ZdSpi::RtnMarketFive(MarketFiveRtn* mfr)
{
	cout << "MarketFive:" << mfr->buyPrice2 << endl;
}
void ZdSpi::RtnMarketNew(MarketNewRtn* mnr)
{
	cout << "MarketNew:" << mnr->filledNum << endl;
}

void ZdSpi::RtnModifyReq(ModifyRtn* mr)
{

}

void ZdSpi::RtnSystemNo(SystemNoRtn* snr)
{
	cout << "SYNO:" << snr->systemNo << endl;
}

void ZdSpi::RtnOrderBack(OrderBackRtn* obr){;} //O1:报单返回
void ZdSpi::RtnOrderStatus(OrderStatusRtn* osr){ ; } //OST:报单状态返回
void ZdSpi::RtnOrderAccount(OrderAccountRtn* oar){ ; } //O2：报单资金返还
void ZdSpi::RtnOrderSysNo(){ ; }  //SY：报单返回系统号
void ZdSpi::RtnOrderFilled(OrderFilledRtn* ofr){ ; } //O3：报单成交返回

#pragma endregion

#pragma region 消息生成器

CShZdMessage* ZdSpi::GetZdReqMessage(string msgType, void* Msg)
{
	if (msgType == Logon)
	{
		LoginReq* lr = (LoginReq*)Msg;

		CShZdMessage* login = new CShZdMessage();
		login->SetMsgType(msgType.c_str());
		login->SetTag(USERID, lr->userId.c_str());
		login->SetTag(USERPWD, lr->userPwd.c_str());
		return login;
	}

	if (msgType == Order)
	{
		OrderReq* or = (OrderReq*)Msg;
		CShZdMessage* order = new CShZdMessage();
		order->SetMsgType(msgType.c_str());

		order->SetTag(USERID, or->userId.c_str());
		order->SetTag(ACCOUNTNO, or->accountNo.c_str());
		order->SetTag(EXCHANGECODE, or->exchangeCode.c_str());
		order->SetTag(CODE, or->code.c_str());
		order->SetTag(BUYSALE, or->buySale.c_str());
		order->SetTag(ORDERNUMBER,or->orderNumber.c_str());
		order->SetTag(ORDERPRICE,or->orderPrice.c_str());
		order->SetTag(PRICETYPE,or->priceType.c_str());
		order->SetTag(LOCALNO,or->localN0.c_str());
		return order;
	}

	if (msgType == OrderCancel)
	{
		OrderCancelReq* oc = (OrderCancelReq*)Msg;
		CShZdMessage* ordercancle = new CShZdMessage();
		ordercancle->SetMsgType(msgType.c_str());

		ordercancle->SetTag(USERID, oc->userId.c_str());
		ordercancle->SetTag(ACCOUNTNO, oc->accountNo.c_str());
		ordercancle->SetTag(TRADEPWD, oc->tradePwd.c_str());
		ordercancle->SetTag(SYSTEMNO, oc->systemNo.c_str());
		ordercancle->SetTag(ORDERNO, oc->orderNo.c_str());
		ordercancle->SetTag(EXCHANGECODE, oc->exchangeCode.c_str());
		ordercancle->SetTag(CODE, oc->code.c_str());
		ordercancle->SetTag(BUYSALE, oc->buySale.c_str());
		ordercancle->SetTag(ORDERNUMBER, oc->orderNumber.c_str());
		ordercancle->SetTag(ORDERPRICE, oc->orderPrice.c_str());
		ordercancle->SetTag(FILLEDNUMBER, oc->filledNumber.c_str());
		return ordercancle;
	}
	if (msgType == Modify)
	{
		ModifyReq* mr = (ModifyReq*)Msg;
		CShZdMessage* modify = new CShZdMessage();
		modify->SetMsgType(msgType.c_str());
		
		modify->SetTag(USERID, mr->userId.c_str());
		modify->SetTag(ACCOUNTNO, mr->accountNo.c_str());
		modify->SetTag(TRADEPWD, mr->tradePwd.c_str());
		modify->SetTag(ORDERNO, mr->orderNo.c_str());
		modify->SetTag(EXCHANGECODE, mr->exchangeCode.c_str());
		modify->SetTag(CODE, mr->code.c_str());
		modify->SetTag(BUYSALE, mr->buySale.c_str());
		modify->SetTag(ORDERNUMBER, mr->orderNumber.c_str());
		modify->SetTag(ORDERPRICE, mr->orderPrice.c_str());
		modify->SetTag(FILLEDNUMBER, mr->filledNumber.c_str());
		modify->SetTag(MODIFYNUMBER, mr->modifyNumber.c_str());
		modify->SetTag(MODIFYPRICE, mr->modifyPrice.c_str());
		modify->SetTag(FISRISKORDER, mr->fIsRiskOrder.c_str());
		modify->SetTag(TRIGGERPRICE, mr->triggerPrice.c_str());
		modify->SetTag(MODIFYTRIGGERPRICE, mr->modifyTriggerPrice.c_str());
		modify->SetTag(SYSTEMNO, mr->systemNo.c_str());
		modify->SetTag(PRICETYPE, mr->priceType.c_str());

		return modify;
	}

	if (msgType == AccountSearch)
	{
		AccountSearchReq* acr = (AccountSearchReq*)Msg;
		CShZdMessage* accountSearch = new CShZdMessage();
		accountSearch->SetMsgType(msgType.c_str());

		accountSearch->SetTag(USERID, acr->userId.c_str());
		accountSearch->SetTag(ACCOUNTNO, acr->accountNo.c_str());
		accountSearch->SetTag(TRADEPWD, acr->tradePwd.c_str());
		return accountSearch;
	}

	if (msgType == ExchangeSearch)
	{
		ExchangeSearchReq* esr = (ExchangeSearchReq*)Msg;
		CShZdMessage* exchangeSearch = new CShZdMessage();
		exchangeSearch->SetMsgType(msgType.c_str());

		exchangeSearch->SetTag(ACCOUNTNO, esr->accountNo.c_str());
		return exchangeSearch;
	}

	if (msgType == GetUseControl)
	{
		ControlReq* cr = (ControlReq*)Msg;
		CShZdMessage* HY = new CShZdMessage();
		HY->SetMsgType(msgType.c_str());

		HY->SetTag(ACCOUNTNO, cr->accountNo.c_str());
		return HY;
	}

	if (msgType == Market)
	{
		MarketReq* mr = (MarketReq*)Msg;
		CShZdMessage* market = new CShZdMessage();
		market->SetMsgType(msgType.c_str());

		market->SetTag(ACCOUNTNO,mr->accountNo.c_str());
		market->SetTag(TODAYCANUSE, mr->todayCanUse.c_str());
		market->SetTag(CODE, mr->code.c_str());
		return market;
	}

	if (msgType == FillSearch)
	{
		FillSearchReq* fsr = (FillSearchReq*)Msg;
		CShZdMessage* fillsearch = new CShZdMessage();
		fillsearch->SetMsgType(msgType.c_str());

		fillsearch->SetTag(USERID, fsr->userId.c_str());
		fillsearch->SetTag(TRADEPWD, fsr->tradePwd.c_str());
		fillsearch->SetTag(SYSTEMNO, fsr->systemNo.c_str());
		return fillsearch;
	}

	if (msgType == OrderSearch)
	{
		OrderSearchReq* osr = (OrderSearchReq*)Msg;
		CShZdMessage* ordersearch = new CShZdMessage();
		ordersearch->SetMsgType(msgType.c_str());

		ordersearch->SetTag(USERID, osr->userId.c_str());
		ordersearch->SetTag(ACCOUNTNO,osr->accountNo.c_str());
		ordersearch->SetTag(TRADEPWD, osr->tradePwd.c_str());
		ordersearch->SetTag(TODAYCANUSE, osr->todayCanuse.c_str());
		ordersearch->SetTag(SYSTEMNO, osr->systemNo.c_str());

		return ordersearch;
	}

	if (msgType == OpenDetail)
	{
		OpenDetailReq* odr = (OpenDetailReq*)Msg;
		CShZdMessage* opendetail = new CShZdMessage();
		opendetail->SetMsgType(msgType.c_str());

		opendetail->SetTag(USERID, odr->userId.c_str());
		opendetail->SetTag(TRADEPWD, odr->tradePwd.c_str());

		return opendetail;
	}
}

#pragma endregion

#pragma region 响应适配器

void ZdSpi::ChgRspFunction(CShZdMessage* msg)
{
	string msgType = msg->GetMesgType();
	if (msgType == LogonBack)
	{
		string isLast = msg->GetString(INFOOVER);
		if (isLast == "1")
		{
			this->RtnLoginReq(NULL, true);
			return;
		}
		LoginRtn lr;
		lr.accountNo = msg->GetString(ACCOUNTNO);
		lr.currencyNo = msg->GetString(CURRENCYNO);
		lr.FSelAll = msg->GetString(FSELALL);
		lr.isSimulation = msg->GetString(ISSIMULATION);
		lr.status = msg->GetString(STATUS);
		lr.tradePwd = msg->GetString(TRADEPWD);
		lr.userId = msg->GetString(USERID);
		lr.userName = msg->GetString(USERNAME);
		lr.userPwd = msg->GetString(USERPWD);
		lr.userType = msg->GetString(USERTYPE);

		this->RtnLoginReq(&lr, false);
		return;
	}
	if (msgType == OrderBack)
	{
		OrderBackRtn obr;
		obr.userId = msg->GetString(USERID);
		obr.accountNo = msg->GetString(ACCOUNTNO);
		obr.systemNo = msg->GetString(SYSTEMNO);
		obr.localNo = msg->GetString(LOCALNO);
		obr.orderNo = msg->GetString(ORDERNO);
		obr.origOrderNo = msg->GetString(ORIGORDERNO);
		obr.exchangeCode = msg->GetString(EXCHANGECODE);
		obr.code = msg->GetString(CODE);
		obr.buySale = msg->GetString(BUYSALE);
		obr.orderNumber = msg->GetString(ORDERNUMBER);
		obr.orderPrice = msg->GetString(ORDERPRICE);
		obr.priceType = msg->GetString(PRICETYPE);
		obr.orderDate = msg->GetString(ORDERDATE);
		obr.orderTime = msg->GetString(ORDERTIME);
		obr.errorCode = msg->GetString(ERRORCODE);
		obr.status = msg->GetString(STATUS);
		obr.fIsRiskOrder = msg->GetString(FISRISKORDER);

		this->RtnOrderBack(&obr);
		return;
	}
	if (msgType == OrderAccount)
	{
		OrderAccountRtn oar;
		
		oar.clientNo = msg->GetString(CLIENTNO);
		oar.accountNo = msg->GetString(ACCOUNTNO);
		oar.oldCanUse = msg->GetString(OLDCANUSE);
		oar.todayAmount = msg->GetString(TODAYAMOUNT);
		oar.expiredProfit = msg->GetString(EXPIREDPROFIT);
		oar.frozenDeposit = msg->GetString(FROZENDEPOSIT);
		oar.fee = msg->GetString(FEE);
		oar.deposit = msg->GetString(DEPOSIT);
		oar.keepDeposit = msg->GetString(KEEPDEPOSIT);
		oar.inMoney = msg->GetString(INMONEY);
		oar.outMoney = msg->GetString(OUTMONEY);
		oar.unexpiredProfit = msg->GetString(UNEXPIREDPROFIT);
		oar.unaccountProfit = msg->GetString(UNACCOUNTPROFIT);
		oar.royalty = msg->GetString(ROYALTY);

		this->RtnOrderAccount(&oar);
		return;
	}
	if (msgType == FilledCast)
	{
		OrderFilledRtn ofr;
		ofr.userId = msg->GetString(USERID);
		ofr.accountNo = msg->GetString(ACCOUNTNO);
		ofr.filledNo = msg->GetString(FILLEDNO);
		ofr.orderNo = msg->GetString(ORDERNO);
		ofr.systemNo = msg->GetString(SYSTEMNO);
		ofr.localNo = msg->GetString(LOCALNO);
		ofr.exchangeCode = msg->GetString(EXCHANGECODE);
		ofr.code = msg->GetString(CODE);
		ofr.buySale = msg->GetString(BUYSALE);
		ofr.filledNumber = msg->GetString(FILLEDNUMBER);
		ofr.openPrice = msg->GetString(OPENPRICE);
		ofr.filledDate = msg->GetString(FILLEDDATE);
		ofr.cancelTime = msg->GetString(CANCELTIME);
		ofr.fee = msg->GetString(FEE);
		ofr.errorCode = msg->GetString(ERRORCODE);
		ofr.changeDate = msg->GetString(CHANGEDATE);
		ofr.filledType = msg->GetString(FILLEDTYPE);

		this->RtnOrderFilled(&ofr);
		return;
	}
	if (msgType == OrderStatus)
	{
		OrderStatusRtn osr;
		osr.exchangeCode = msg->GetString(EXCHANGECODE);
		osr.zd_Code = msg->GetString(ZD_CODE);
		osr.orderNo = msg->GetString(ORDERNO);
		osr.orderNumber = msg->GetString(ORDERNUMBER);
		osr.filledNumber = msg->GetString(FILLEDNUMBER);
		osr.filledAdvPrice = msg->GetString(FILLEDADVPRICE);
		osr.buyHoldNumber = msg->GetString(BUYHOLDNUMBER);
		osr.buyHoldOpenPrice = msg->GetString(BUYHOLDOPENPRICE);
		osr.buyHoldPrice = msg->GetString(BUYHOLDPRICE);
		osr.saleHoldNumber = msg->GetString(SALEHOLDNUMBER);
		osr.saleHoldOpenPrice = msg->GetString(SALEHOLDOPENPRICE);
		osr.saleHoldPrice = msg->GetString(SALEHOLDPRICE);
		osr.isCanceled = msg->GetString(ISCANCELED);
		osr.filledTotalFee = msg->GetString(FILLEDTOTALFEE);
		osr.oIndex = msg->GetString(OINDEX);
		osr.accountNo = msg->GetString(ACCOUNTNO);

		this->RtnOrderStatus(&osr);
	}
	if (msgType == AccountSearchBack)
	{
		string isLast = msg->GetString(INFOOVER);
		if (isLast == "1")
		{
			this->RtnAccountSearchReq(NULL, true);
			return;
		}
		AccountSearchRtn acr;
		acr.userId = msg->GetString(USERID);
		acr.accountNo = msg->GetString(ACCOUNTNO);

		acr.oldCanUse = msg->GetString(OLDCANUSE);
		acr.oldBalance = msg->GetString(OLDBALANCE);
		acr.oldAmount = msg->GetString(OLDAMOUNT);
		acr.inMoney = msg->GetString(INMONEY);
		acr.outMoney = msg->GetString(OUTMONEY);
		acr.todayBalance = msg->GetString(TODAYBALANCE);
		acr.todayAmount = msg->GetString(TODAYAMOUNT);
		acr.frozenDeposit = msg->GetString(FROZENDEPOSIT);
		acr.todayCanUse = msg->GetString(TODAYCANUSE);
		acr.deposit = msg->GetString(DEPOSIT);
		acr.commission = msg->GetString(COMMISSION);
		acr.expiredProfit = msg->GetString(EXPIREDPROFIT);
		acr.netProfit = msg->GetString(NETPROFIT);
		acr.profitRate = msg->GetString(PROFITRATE);
		acr.riskRate = msg->GetString(RISKRATE);
		acr.currencyNo = msg->GetString(CURRENCYNO);
		acr.currencyRate = msg->GetString(CURRENCYRATE);
		acr.unexpiredProfit = msg->GetString(UNEXPIREDPROFIT);
		acr.unaccountProfit = msg->GetString(UNACCOUNTPROFIT);
		acr.keepDeposit = msg->GetString(KEEPDEPOSIT);
		acr.royalty = msg->GetString(ROYALTY);

		this->RtnAccountSearchReq(&acr, false);
		return;
	}
	if (msgType == CancelBack)
	{
		string isLast = msg->GetString(INFOOVER);
		if (isLast == "1")
		{
			this->RtnCancelReq(NULL, true);
			return;
		}
		OrderCancelRtn ocr;
		
		ocr.userId=msg->GetString(USERID);
		ocr.accountNo=msg->GetString(ACCOUNTNO);
		ocr.systemNo=msg->GetString(SYSTEMNO);
		ocr.orderNo=msg->GetString(ORDERNO);
		ocr.cancelNo=msg->GetString(CANCELNO);
		ocr.exchangeCode=msg->GetString(EXCHANGECODE);
		ocr.code=msg->GetString(CODE);
		ocr.buySale=msg->GetString(BUYSALE);
		ocr.orderNumber=msg->GetString(ORDERNUMBER);
		ocr.orderPrice=msg->GetString(ORDERPRICE);
		ocr.filledNumber=msg->GetString(FILLEDNUMBER);
		ocr.cancelNumber=msg->GetString(CANCELNUMBER);
		ocr.cancelDate=msg->GetString(CANCELDATE);
		ocr.cancelTime=msg->GetString(CANCELTIME);
		ocr.errorCode=msg->GetString(ERRORCODE);


		this->RtnCancelReq(&ocr, false);
		return;
	}
	if (msgType == FillSearchBack)
	{
		string isLast = msg->GetString(INFOOVER);
		if (isLast == "1")
		{
			this->RtnFillSearchReq(NULL, true);
			return;
		}
		FillSearchRtn fsr;

		fsr.userId=msg->GetString(USERID);
		fsr.accountNo=msg->GetString(ACCOUNTNO);
		fsr.filledNo=msg->GetString(FILLEDNO);
		fsr.orderNo=msg->GetString(ORDERNO);
		fsr.systemNo=msg->GetString(SYSTEMNO);
		fsr.localNo=msg->GetString(LOCALNO);
		fsr.exchangeCode=msg->GetString(EXCHANGECODE);
		fsr.code=msg->GetString(CODE);
		fsr.buySale=msg->GetString(BUYSALE);
		fsr.filledNumber=msg->GetString(FILLEDNUMBER);
		fsr.openPrice=msg->GetString(OPENPRICE);
		fsr.filledDate=msg->GetString(FILLEDDATE);
		fsr.fee=msg->GetString(FEE);
		fsr.errorCode=msg->GetString(ERRORCODE);
		fsr.changeDate=msg->GetString(CHANGEDATE);
		fsr.filledType=msg->GetString(FILLEDTYPE);
				
		this->RtnFillSearchReq(&fsr, false);
		return;
	}
	if (msgType == ModifyBack)
	{
		ModifyRtn mr;
		mr.userId = msg->GetString(USERID);
		mr.accountNo = msg->GetString(ACCOUNTNO);
		mr.systemNo = msg->GetString(SYSTEMNO);
		mr.localNo = msg->GetString(LOCALNO);
		mr.orderNo = msg->GetString(ORDERNO);
		mr.orderMethod = msg->GetString(ORDERMETHOD);
		mr.acceptType = msg->GetString(ACCEPTTYPE);
		mr.exchangeCode = msg->GetString(EXCHANGECODE);
		mr.code = msg->GetString(CODE);
		mr.buySale = msg->GetString(BUYSALE);
		mr.orderNumber = msg->GetString(ORDERNUMBER);
		mr.orderPrice = msg->GetString(ORDERPRICE);
		mr.priceType = msg->GetString(PRICETYPE);
		mr.cancelDate = msg->GetString(CANCELDATE);
		mr.cancelTime = msg->GetString(CANCELTIME);
		mr.errorCode = msg->GetString(ERRORCODE);
		mr.status = msg->GetString(STATUS);
		mr.fIsRiskOrder = msg->GetString(FISRISKORDER);
		mr.triggerPrice = msg->GetString(TRIGGERPRICE);
		
		return this->RtnModifyReq(&mr);
	}
	if (msgType == OrderSearchBack)
	{
		string isLast = msg->GetString(INFOOVER);
		if (isLast == "1")
		{
			this->RtnOrderSearchReq(NULL, true);
			return;
		}
		OrderSearchRtn osr;

		osr.userId=msg->GetString(USERID);
		osr.accountNo=msg->GetString(ACCOUNTNO);
		osr.systemNo=msg->GetString(SYSTEMNO);
		osr.localNo=msg->GetString(LOCALNO);
		osr.orderNo=msg->GetString(ORDERNO);
		osr.origOrderNo=msg->GetString(ORIGORDERNO);
		osr.exchangeCode=msg->GetString(EXCHANGECODE);
		osr.code=msg->GetString(CODE);
		osr.buySale=msg->GetString(BUYSALE);
		osr.orderNumber=msg->GetString(ORDERNUMBER);
		osr.orderPrice=msg->GetString(ORDERPRICE);
		osr.filledNumber=msg->GetString(FILLEDNUMBER);
		osr.openPrice=msg->GetString(OPENPRICE);
		osr.priceType=msg->GetString(PRICETYPE);
		osr.cancelDate=msg->GetString(CANCELDATE);
		osr.cancelTime=msg->GetString(CANCELTIME);
		osr.errorCode=msg->GetString(ERRORCODE);
		osr.status=msg->GetString(STATUS);
		osr.fIsRiskOrder=msg->GetString(FISRISKORDER);
		osr.cancelUserId=msg->GetString(CANCELUSERID);
		osr.orderDate=msg->GetString(ORDERDATE);
		osr.orderTime=msg->GetString(ORDERTIME);

		return this->RtnOrderSearchReq(&osr, false);
	}
	if (msgType == MarketBack)
	{
		MarketRtn mr;
		mr.exchangeCode = msg->GetString(EXCHANGECODE);
		mr.code = msg->GetString(CODE);
		mr.buyPrice = msg->GetString(BUYPRICE);
		mr.buyNumber = msg->GetString(BUYNUMBER);
		mr.salePrice = msg->GetString(SALEPRICE);
		mr.saleNumber = msg->GetString(SALENUMBER);
		mr.currPrice = msg->GetString(CURRPRICE);
		mr.currNumber = msg->GetString(CURRNUMBER);
		mr.high = msg->GetString(HIGH);
		mr.low = msg->GetString(LOW);
		mr.open = msg->GetString(OPEN);
		mr.oldClose = msg->GetString(OLDCLOSE);
		mr.close = msg->GetString(CLOSE);
		mr.time = msg->GetString(TIME);
		mr.filledNum = msg->GetString(FILLEDNUM);
		mr.holdNum = msg->GetString(HOLDNUM);
		mr.buyPrice2 = msg->GetString(BUYPRICE2);
		mr.buyPrice3 = msg->GetString(BUYPRICE3);
		mr.buyPrice4 = msg->GetString(BUYPRICE4);
		mr.buyPrice5 = msg->GetString(BUYPRICE5);
		mr.buyNumber2 = msg->GetString(BUYNUMBER2);
		mr.buyNumber3 = msg->GetString(BUYNUMBER3);
		mr.buyNumber4 = msg->GetString(BUYNUMBER4);
		mr.buyNumber5 = msg->GetString(BUYNUMBER5);
		mr.salePrice2 = msg->GetString(SALEPRICE2);
		mr.salePrice3 = msg->GetString(SALEPRICE3);
		mr.salePrice4 = msg->GetString(SALEPRICE4);
		mr.salePrice5 = msg->GetString(SALEPRICE5);
		mr.saleNumber2 = msg->GetString(SALENUMBER2);
		mr.saleNumber3 = msg->GetString(SALENUMBER3);
		mr.saleNumber4 = msg->GetString(SALENUMBER4);
		mr.saleNumber5 = msg->GetString(SALENUMBER5);
		mr.hideBuyPrice = msg->GetString(HIDEBUYPRICE);
		mr.hideBuyNumber = msg->GetString(HIDEBUYNUMBER);
		mr.hideSalePrice = msg->GetString(HIDESALEPRICE);
		mr.hideSaleNumber = msg->GetString(HIDESALENUMBER);
		return this->RtnMarket(&mr);
	}

	if (msgType == MarketFive)
	{
		MarketFiveRtn mfr;
		
		mfr.exchangeCode = msg->GetString(EXCHANGECODE);
		mfr.code = msg->GetString(CODE);
		mfr.buyPrice = msg->GetString(BUYPRICE);
		mfr.buyNumber = msg->GetString(BUYNUMBER);
		mfr.salePrice = msg->GetString(SALEPRICE);
		mfr.saleNumber = msg->GetString(SALENUMBER);
		mfr.time = msg->GetString(TIME);
		mfr.buyPrice2 = msg->GetString(BUYPRICE2);
		mfr.buyPrice3 = msg->GetString(BUYPRICE3);
		mfr.buyPrice4 = msg->GetString(BUYPRICE4);
		mfr.buyPrice5 = msg->GetString(BUYPRICE5);
		mfr.buyNumber2 = msg->GetString(BUYNUMBER2);
		mfr.buyNumber3 = msg->GetString(BUYNUMBER3);
		mfr.buyNumber4 = msg->GetString(BUYNUMBER4);
		mfr.buyNumber5 = msg->GetString(BUYNUMBER5);
		mfr.salePrice2 = msg->GetString(SALEPRICE2);
		mfr.salePrice3 = msg->GetString(SALEPRICE3);
		mfr.salePrice4 = msg->GetString(SALEPRICE4);
		mfr.salePrice5 = msg->GetString(SALEPRICE5);
		mfr.saleNumber2 = msg->GetString(SALENUMBER2);
		mfr.saleNumber3 = msg->GetString(SALENUMBER3);
		mfr.saleNumber4 = msg->GetString(SALENUMBER4);
		mfr.saleNumber5 = msg->GetString(SALENUMBER5);
		mfr.hideBuyPrice = msg->GetString(HIDEBUYPRICE);
		mfr.hideBuyNumber = msg->GetString(HIDEBUYNUMBER);
		mfr.hideSalePrice = msg->GetString(HIDESALEPRICE);
		mfr.hideSaleNumber = msg->GetString(HIDESALENUMBER);

		return this->RtnMarketFive(&mfr);
	}
	if (msgType == MarketNew)
	{
		MarketNewRtn mnr;

		mnr.exchangeCode = msg->GetString(EXCHANGECODE);
		mnr.code = msg->GetString(CODE);
		mnr.buyPrice = msg->GetString(BUYPRICE);
		mnr.buyNumber = msg->GetString(BUYNUMBER);
		mnr.salePrice = msg->GetString(SALEPRICE);
		mnr.saleNumber = msg->GetString(SALENUMBER);
		mnr.currPrice = msg->GetString(CURRPRICE);
		mnr.currNumber = msg->GetString(CURRNUMBER);
		mnr.high = msg->GetString(HIGH);
		mnr.low = msg->GetString(LOW);
		mnr.open = msg->GetString(OPEN);
		mnr.oldClose = msg->GetString(OLDCLOSE);
		mnr.close = msg->GetString(CLOSE);
		mnr.time = msg->GetString(TIME);
		mnr.filledNum = msg->GetString(FILLEDNUM);
		mnr.holdNum = msg->GetString(HOLDNUM);
		mnr.hideBuyPrice = msg->GetString(HIDEBUYPRICE);
		mnr.hideBuyNumber = msg->GetString(HIDEBUYNUMBER);
		mnr.hideSalePrice = msg->GetString(HIDESALEPRICE);
		mnr.hideSaleNumber = msg->GetString(HIDESALENUMBER);
		
		return this->RtnMarketNew(&mnr);
	}
	if (msgType == SysNo)
	{
		SystemNoRtn snr;
		snr.accountNo = msg->GetString(ACCOUNTNO);
		snr.userId = msg->GetString(USERID);
		snr.tradePwd = msg->GetString(TRADEPWD);
		snr.userType = msg->GetString(USERTYPE);
		snr.todayCanUse = msg->GetString(TODAYCANUSE);
		snr.systemNo = msg->GetString(SYSTEMNO);
		snr.localNo = msg->GetString(LOCALNO);
		snr.exchangeCode = msg->GetString(EXCHANGECODE);
		snr.code = msg->GetString(CODE);
		snr.buySale = msg->GetString(BUYSALE);
		snr.orderNumber = msg->GetString(ORDERNUMBER);
		snr.orderPrice = msg->GetString(ORDERPRICE);
		snr.tradeType = msg->GetString(TRADETYPE);
		snr.priceType = msg->GetString(PRICETYPE);
		snr.htsType = msg->GetString(HTSTYPE);
		snr.fIsRiskOrder = msg->GetString(FISRISKORDER);
		snr.errorCode = msg->GetString(ERRORCODE);

		return this->RtnSystemNo(&snr);

	}
	
	if (msgType == GetUseControl)
	{
		string isLast = msg->GetString(INFOOVER);
		if (isLast == "1")
		{
			this->RtnGetUserControlReq(NULL, true);
			return;
		}
		ControlRtn cr;
		cr.zd_Code = msg->GetString(ZD_CODE);
		cr.exchangeCode = msg->GetString(EXCHANGECODE);
		cr.code = msg->GetString(CODE);
		cr.codeName = msg->GetString(CODENAME);
		cr.comNo = msg->GetString(COMNO);
		cr.comName = msg->GetString(COMNAME);
		cr.comType = msg->GetString(COMTYPE);
		cr.currencyNo = msg->GetString(CURRENCYNO);
		cr.currencyName = msg->GetString(CURRENCYNAME);
		cr.productDot = msg->GetString(PRODUCTDOT);
		cr.upperTick = msg->GetString(UPPERTICK);
		cr.exchangeName = msg->GetString(EXCHANGENAME);
		cr.settlePrice = msg->GetString(SETTLEPRICE);
		cr.cancelDate = msg->GetString(CANCELDATE);
		cr.marketDot = msg->GetString(MARKETDOT);
		cr.marketUnit = msg->GetString(MARKETUNIT);
		cr.fDotNum_Carry = msg->GetString(FDOTNUM_CARRY);
		cr.fUpperTick_Carry = msg->GetString(FUPPERTICK_CARRY);
		cr.fFirstNoticeDay = msg->GetString(FFIRSTNOTICEDAY);
		cr.fFreezenPercent = msg->GetString(FFREEZENPERCENT);
		cr.fFreezenMoney = msg->GetString(FFREENZENMONEY);
		cr.fFreePercent = msg->GetString(FFREEPERCENT);
		cr.fPriceStrike = msg->GetString(FPRICESTRIKE);
		cr.fProductDotStrike = msg->GetString(FPRODUCTDOTSTRIKE);
		cr.fUpperTickStrike = msg->GetString(FUPPERTICKSTRIKE);

		return this->RtnGetUserControlReq(&cr, false);
	}
	if (msgType == ExchangeSearchBack)
	{
		string isLast = msg->GetString(INFOOVER);
		if (isLast == "1")
		{
			this->RtnExchangeSearchReq(NULL, true);
			return;
		}
		ExchangeSearchRtn esr;
		esr.exchangeCode = msg->GetString(EXCHANGECODE);
		esr.exchangeName = msg->GetString(EXCHANGENAME);

		this->RtnExchangeSearchReq(&esr, false);
		return;
	}
	if (msgType == CurrencySearchBack)
	{
		LoginRtn lr;
		lr.accountNo = msg->GetString(ACCOUNTNO);
		this->RtnLoginReq(&lr, true);
	}
	if (msgType == OpenDetailBack)
	{
		string isLast = msg->GetString(INFOOVER);
		if (isLast == "1")
		{
			this->RtnOpenDetailReq(NULL, true);
			return;
		}
		OpenDetailRtn odr;
		
		odr.clientNo=msg->GetString(CLIENTNO);
		odr.accountNo=msg->GetString(ACCOUNTNO);
		odr.exchangeCode=msg->GetString(EXCHANGECODE);
		odr.currencyNo=msg->GetString(CURRENCYNO);
		odr.comNo=msg->GetString(COMNO);
		odr.code=msg->GetString(CODE);
		odr.filledDate=msg->GetString(FILLEDDATE);
		odr.filledNo=msg->GetString(FILLEDNO);
		odr.buySale=msg->GetString(BUYSALE);
		odr.openNumber=msg->GetString(OPENNUMBER);
		odr.initOpenPrice=msg->GetString(INITOPENPRICE);
		odr.openPrice=msg->GetString(OPENPRICE);
		odr.status=msg->GetString(STATUS);
		odr.changeDate=msg->GetString(CHANGEDATE);

		return this->RtnOpenDetailReq(&odr, false);
	}
}

#pragma endregion


void ZdSpi::Init()
{
	InHandle = GetShZdTradeLib();
	InHandle->InitShZdServer();
	InHandle->RegisterOutLib(this);
	//InHandle->RegisterFront("222.73.106.130", 7787);//上海电信
	InHandle->RegisterFront("203.186.175.82", 9787);//香港电信
	WaitForSingleObject(g_Event, INFINITE);

	LoginReq lr = LoginReq();
	lr.userId = "demo000666";
	lr.userPwd = "888888";
	ReqLogin(lr);

	WaitForSingleObject(g_Event, INFINITE);

	/*AccountSearchReq acr = AccountSearchReq();
	acr.userId = "demo000666";
	acr.accountNo = "00003152";
	acr.tradePwd = "888888";
	ReqAccountSearch(acr);

	WaitForSingleObject(g_Event, INFINITE);

	ExchangeSearchReq esr = ExchangeSearchReq();
	esr.accountNo = "00003152";
	ReqExchangeSearch(esr);

	WaitForSingleObject(g_Event, INFINITE);

	ControlReq cr = ControlReq();
	cr.accountNo = "00003151";
	ReqGetUserControl(cr);

	WaitForSingleObject(g_Event, INFINITE);*/

	/*InHandle->RegisterMarket("222.73.106.130", 9984);
	WaitForSingleObject(g_Event, INFINITE);
	MarketReq mr = MarketReq();
	mr.accountNo = "00003152";
	mr.todayCanUse = "+";
	mr.code = "CME,6J1412";
	ReqMarket(mr);*/

	/*FillSearchReq fsr = FillSearchReq();
	fsr.userId = "demo000666";
	ReqFillSearch(fsr);*/

	/*OrderSearchReq osr = OrderSearchReq();
	osr.userId = "demo000666";
	ReqOrderSearch(osr);*/

	/*OpenDetailReq odr = OpenDetailReq();
	odr.userId = "demo000666";
	ReqOpenDetail(odr);*/
	
	for (int i = 1; i <= 1;i++)
	{
		OrderReq or = OrderReq();
		or.userId = "demo000666";
		or.accountNo = "00003152";
		or.exchangeCode = "CME";
		or.code = "6A1412";
		or.buySale = "1";
		or.orderNumber = "2";
		or.orderPrice = "0.8531";
		or.priceType = "1";
		or.localN0 = "-1";

		ReqOrder(or);
	}
	

	WaitForSingleObject(g_Event, INFINITE);

	cout << "good" << endl;
	//test github
}




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
	CShZdMessage* req = GetZdReqMessage(ZD_Logon, &lr);
	InHandle->ShZdSendInfoToTrade(req);
}

void ZdSpi::ReqAccountSearch(AccountSearchReq acr)
{
	CShZdMessage* req = GetZdReqMessage(ZD_AccountSearch, &acr);
	InHandle->ShZdSendInfoToTrade(req);
}

void ZdSpi::ReqExchangeSearch(ExchangeSearchReq esr)
{
	CShZdMessage* req = GetZdReqMessage(ZD_ExchangeSearch, &esr);
	InHandle->ShZdSendInfoToTrade(req);
}

void ZdSpi::ReqOrder(OrderReq or)
{
	CShZdMessage* req = GetZdReqMessage(ZD_Order, &or);
	InHandle->ShZdSendInfoToTrade(req);
}

void ZdSpi::ReqModify(ModifyReq mr)
{
	CShZdMessage* req = GetZdReqMessage(ZD_Modify, &mr);
	InHandle->ShZdSendInfoToTrade(req);
}

void ZdSpi::ReqCancel(OrderCancelReq ocr)
{
	CShZdMessage* req = GetZdReqMessage(ZD_OrderCancel, &ocr);
	InHandle->ShZdSendInfoToTrade(req);
}

void ZdSpi::ReqGetUserControl(ControlReq cr)
{
	CShZdMessage* req = GetZdReqMessage(ZD_GetUseControl, &cr);
	InHandle->ShZdSendInfoToTrade(req);
}

void ZdSpi::ReqMarket(MarketReq mr)
{
	CShZdMessage* req = GetZdReqMessage(ZD_Market, &mr);
	InHandle->ShZdSendInfoToMarket(req);
}

void ZdSpi::ReqFillSearch(FillSearchReq fsr)
{
	CShZdMessage* req = GetZdReqMessage(ZD_FillSearch, &fsr);
	InHandle->ShZdSendInfoToTrade(req);
}

void ZdSpi::ReqOrderSearch(OrderSearchReq osr)
{
	CShZdMessage* req = GetZdReqMessage(ZD_OrderSearch, &osr);
	InHandle->ShZdSendInfoToTrade(req);
}

void ZdSpi::ReqOpenDetail(OpenDetailReq odr)
{
	CShZdMessage* req = GetZdReqMessage(ZD_OpenDetail, &odr);
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
	if (msgType == ZD_Logon)
	{
		LoginReq* lr = (LoginReq*)Msg;

		CShZdMessage* login = new CShZdMessage();
		login->SetMsgType(msgType.c_str());
		login->SetTag(ZD_USERID, lr->userId.c_str());
		login->SetTag(ZD_USERPWD, lr->userPwd.c_str());
		return login;
	}

	if (msgType == ZD_Order)
	{
		OrderReq* or = (OrderReq*)Msg;
		CShZdMessage* order = new CShZdMessage();
		order->SetMsgType(msgType.c_str());

		order->SetTag(ZD_USERID, or->userId.c_str());
		order->SetTag(ZD_ACCOUNTNO, or->accountNo.c_str());
		order->SetTag(ZD_EXCHANGECODE, or->exchangeCode.c_str());
		order->SetTag(ZD_CODE, or->code.c_str());
		order->SetTag(ZD_BUYSALE, or->buySale.c_str());
		order->SetTag(ZD_ORDERNUMBER,or->orderNumber.c_str());
		order->SetTag(ZD_ORDERPRICE,or->orderPrice.c_str());
		order->SetTag(ZD_PRICETYPE,or->priceType.c_str());
		order->SetTag(ZD_LOCALNO,or->localN0.c_str());
		return order;
	}

	if (msgType == ZD_OrderCancel)
	{
		OrderCancelReq* oc = (OrderCancelReq*)Msg;
		CShZdMessage* ordercancle = new CShZdMessage();
		ordercancle->SetMsgType(msgType.c_str());

		ordercancle->SetTag(ZD_USERID, oc->userId.c_str());
		ordercancle->SetTag(ZD_ACCOUNTNO, oc->accountNo.c_str());
		ordercancle->SetTag(ZD_TRADEPWD, oc->tradePwd.c_str());
		ordercancle->SetTag(ZD_SYSTEMNO, oc->systemNo.c_str());
		ordercancle->SetTag(ZD_ORDERNO, oc->orderNo.c_str());
		ordercancle->SetTag(ZD_EXCHANGECODE, oc->exchangeCode.c_str());
		ordercancle->SetTag(ZD_CODE, oc->code.c_str());
		ordercancle->SetTag(ZD_BUYSALE, oc->buySale.c_str());
		ordercancle->SetTag(ZD_ORDERNUMBER, oc->orderNumber.c_str());
		ordercancle->SetTag(ZD_ORDERPRICE, oc->orderPrice.c_str());
		ordercancle->SetTag(ZD_FILLEDNUMBER, oc->filledNumber.c_str());
		return ordercancle;
	}
	if (msgType == ZD_Modify)
	{
		ModifyReq* mr = (ModifyReq*)Msg;
		CShZdMessage* modify = new CShZdMessage();
		modify->SetMsgType(msgType.c_str());
		
		modify->SetTag(ZD_USERID, mr->userId.c_str());
		modify->SetTag(ZD_ACCOUNTNO, mr->accountNo.c_str());
		modify->SetTag(ZD_TRADEPWD, mr->tradePwd.c_str());
		modify->SetTag(ZD_ORDERNO, mr->orderNo.c_str());
		modify->SetTag(ZD_EXCHANGECODE, mr->exchangeCode.c_str());
		modify->SetTag(ZD_CODE, mr->code.c_str());
		modify->SetTag(ZD_BUYSALE, mr->buySale.c_str());
		modify->SetTag(ZD_ORDERNUMBER, mr->orderNumber.c_str());
		modify->SetTag(ZD_ORDERPRICE, mr->orderPrice.c_str());
		modify->SetTag(ZD_FILLEDNUMBER, mr->filledNumber.c_str());
		modify->SetTag(ZD_MODIFYNUMBER, mr->modifyNumber.c_str());
		modify->SetTag(ZD_MODIFYPRICE, mr->modifyPrice.c_str());
		modify->SetTag(ZD_FISRISKORDER, mr->fIsRiskOrder.c_str());
		modify->SetTag(ZD_TRIGGERPRICE, mr->triggerPrice.c_str());
		modify->SetTag(ZD_MODIFYTRIGGERPRICE, mr->modifyTriggerPrice.c_str());
		modify->SetTag(ZD_SYSTEMNO, mr->systemNo.c_str());
		modify->SetTag(ZD_PRICETYPE, mr->priceType.c_str());

		return modify;
	}

	if (msgType == ZD_AccountSearch)
	{
		AccountSearchReq* acr = (AccountSearchReq*)Msg;
		CShZdMessage* accountSearch = new CShZdMessage();
		accountSearch->SetMsgType(msgType.c_str());

		accountSearch->SetTag(ZD_USERID, acr->userId.c_str());
		accountSearch->SetTag(ZD_ACCOUNTNO, acr->accountNo.c_str());
		accountSearch->SetTag(ZD_TRADEPWD, acr->tradePwd.c_str());
		return accountSearch;
	}

	if (msgType == ZD_ExchangeSearch)
	{
		ExchangeSearchReq* esr = (ExchangeSearchReq*)Msg;
		CShZdMessage* exchangeSearch = new CShZdMessage();
		exchangeSearch->SetMsgType(msgType.c_str());

		exchangeSearch->SetTag(ZD_ACCOUNTNO, esr->accountNo.c_str());
		return exchangeSearch;
	}

	if (msgType == ZD_GetUseControl)
	{
		ControlReq* cr = (ControlReq*)Msg;
		CShZdMessage* HY = new CShZdMessage();
		HY->SetMsgType(msgType.c_str());

		HY->SetTag(ZD_ACCOUNTNO, cr->accountNo.c_str());
		return HY;
	}

	if (msgType == ZD_Market)
	{
		MarketReq* mr = (MarketReq*)Msg;
		CShZdMessage* market = new CShZdMessage();
		market->SetMsgType(msgType.c_str());

		market->SetTag(ZD_ACCOUNTNO,mr->accountNo.c_str());
		market->SetTag(ZD_TODAYCANUSE, mr->todayCanUse.c_str());
		market->SetTag(ZD_CODE, mr->code.c_str());
		return market;
	}

	if (msgType == ZD_FillSearch)
	{
		FillSearchReq* fsr = (FillSearchReq*)Msg;
		CShZdMessage* fillsearch = new CShZdMessage();
		fillsearch->SetMsgType(msgType.c_str());

		fillsearch->SetTag(ZD_USERID, fsr->userId.c_str());
		fillsearch->SetTag(ZD_TRADEPWD, fsr->tradePwd.c_str());
		fillsearch->SetTag(ZD_SYSTEMNO, fsr->systemNo.c_str());
		return fillsearch;
	}

	if (msgType == ZD_OrderSearch)
	{
		OrderSearchReq* osr = (OrderSearchReq*)Msg;
		CShZdMessage* ordersearch = new CShZdMessage();
		ordersearch->SetMsgType(msgType.c_str());

		ordersearch->SetTag(ZD_USERID, osr->userId.c_str());
		ordersearch->SetTag(ZD_ACCOUNTNO,osr->accountNo.c_str());
		ordersearch->SetTag(ZD_TRADEPWD, osr->tradePwd.c_str());
		ordersearch->SetTag(ZD_TODAYCANUSE, osr->todayCanuse.c_str());
		ordersearch->SetTag(ZD_SYSTEMNO, osr->systemNo.c_str());

		return ordersearch;
	}

	if (msgType == ZD_OpenDetail)
	{
		OpenDetailReq* odr = (OpenDetailReq*)Msg;
		CShZdMessage* opendetail = new CShZdMessage();
		opendetail->SetMsgType(msgType.c_str());

		opendetail->SetTag(ZD_USERID, odr->userId.c_str());
		opendetail->SetTag(ZD_TRADEPWD, odr->tradePwd.c_str());

		return opendetail;
	}
}

#pragma endregion

#pragma region 响应适配器

void ZdSpi::ChgRspFunction(CShZdMessage* msg)
{
	string msgType = msg->GetMesgType();
	if (msgType == ZD_LogonBack)
	{
		string isLast = msg->GetString(ZD_INFOOVER);
		if (isLast == "1")
		{
			this->RtnLoginReq(NULL, true);
			return;
		}
		LoginRtn lr;
		lr.accountNo = msg->GetString(ZD_ACCOUNTNO);
		lr.currencyNo = msg->GetString(ZD_CURRENCYNO);
		lr.FSelAll = msg->GetString(ZD_FSELALL);
		lr.isSimulation = msg->GetString(ZD_ISSIMULATION);
		lr.status = msg->GetString(ZD_STATUS);
		lr.tradePwd = msg->GetString(ZD_TRADEPWD);
		lr.userId = msg->GetString(ZD_USERID);
		lr.userName = msg->GetString(ZD_USERNAME);
		lr.userPwd = msg->GetString(ZD_USERPWD);
		lr.userType = msg->GetString(ZD_USERTYPE);

		this->RtnLoginReq(&lr, false);
		return;
	}
	if (msgType == ZD_OrderBack)
	{
		OrderBackRtn obr;
		obr.userId = msg->GetString(ZD_USERID);
		obr.accountNo = msg->GetString(ZD_ACCOUNTNO);
		obr.systemNo = msg->GetString(ZD_SYSTEMNO);
		obr.localNo = msg->GetString(ZD_LOCALNO);
		obr.orderNo = msg->GetString(ZD_ORDERNO);
		obr.origOrderNo = msg->GetString(ZD_ORIGORDERNO);
		obr.exchangeCode = msg->GetString(ZD_EXCHANGECODE);
		obr.code = msg->GetString(ZD_CODE);
		obr.buySale = msg->GetString(ZD_BUYSALE);
		obr.orderNumber = msg->GetString(ZD_ORDERNUMBER);
		obr.orderPrice = msg->GetString(ZD_ORDERPRICE);
		obr.priceType = msg->GetString(ZD_PRICETYPE);
		obr.orderDate = msg->GetString(ZD_ORDERDATE);
		obr.orderTime = msg->GetString(ZD_ORDERTIME);
		obr.errorCode = msg->GetString(ZD_ERRORCODE);
		obr.status = msg->GetString(ZD_STATUS);
		obr.fIsRiskOrder = msg->GetString(ZD_FISRISKORDER);

		this->RtnOrderBack(&obr);
		return;
	}
	if (msgType == ZD_OrderAccount)
	{
		OrderAccountRtn oar;
		
		oar.clientNo = msg->GetString(ZD_CLIENTNO);
		oar.accountNo = msg->GetString(ZD_ACCOUNTNO);
		oar.oldCanUse = msg->GetString(ZD_OLDCANUSE);
		oar.todayAmount = msg->GetString(ZD_TODAYAMOUNT);
		oar.expiredProfit = msg->GetString(ZD_EXPIREDPROFIT);
		oar.frozenDeposit = msg->GetString(ZD_FROZENDEPOSIT);
		oar.fee = msg->GetString(ZD_FEE);
		oar.deposit = msg->GetString(ZD_DEPOSIT);
		oar.keepDeposit = msg->GetString(ZD_KEEPDEPOSIT);
		oar.inMoney = msg->GetString(ZD_INMONEY);
		oar.outMoney = msg->GetString(ZD_OUTMONEY);
		oar.unexpiredProfit = msg->GetString(ZD_UNEXPIREDPROFIT);
		oar.unaccountProfit = msg->GetString(ZD_UNACCOUNTPROFIT);
		oar.royalty = msg->GetString(ZD_ROYALTY);

		this->RtnOrderAccount(&oar);
		return;
	}
	if (msgType == ZD_FilledCast)
	{
		OrderFilledRtn ofr;
		ofr.userId = msg->GetString(ZD_USERID);
		ofr.accountNo = msg->GetString(ZD_ACCOUNTNO);
		ofr.filledNo = msg->GetString(ZD_FILLEDNO);
		ofr.orderNo = msg->GetString(ZD_ORDERNO);
		ofr.systemNo = msg->GetString(ZD_SYSTEMNO);
		ofr.localNo = msg->GetString(ZD_LOCALNO);
		ofr.exchangeCode = msg->GetString(ZD_EXCHANGECODE);
		ofr.code = msg->GetString(ZD_CODE);
		ofr.buySale = msg->GetString(ZD_BUYSALE);
		ofr.filledNumber = msg->GetString(ZD_FILLEDNUMBER);
		ofr.openPrice = msg->GetString(ZD_OPENPRICE);
		ofr.filledDate = msg->GetString(ZD_FILLEDDATE);
		ofr.cancelTime = msg->GetString(ZD_CANCELTIME);
		ofr.fee = msg->GetString(ZD_FEE);
		ofr.errorCode = msg->GetString(ZD_ERRORCODE);
		ofr.changeDate = msg->GetString(ZD_CHANGEDATE);
		ofr.filledType = msg->GetString(ZD_FILLEDTYPE);

		this->RtnOrderFilled(&ofr);
		return;
	}
	if (msgType == ZD_OrderStatus)
	{
		OrderStatusRtn osr;
		osr.exchangeCode = msg->GetString(ZD_EXCHANGECODE);
		osr.zd_Code = msg->GetString(ZD_ZD_CODE);
		osr.orderNo = msg->GetString(ZD_ORDERNO);
		osr.orderNumber = msg->GetString(ZD_ORDERNUMBER);
		osr.filledNumber = msg->GetString(ZD_FILLEDNUMBER);
		osr.filledAdvPrice = msg->GetString(ZD_FILLEDADVPRICE);
		osr.buyHoldNumber = msg->GetString(ZD_BUYHOLDNUMBER);
		osr.buyHoldOpenPrice = msg->GetString(ZD_BUYHOLDOPENPRICE);
		osr.buyHoldPrice = msg->GetString(ZD_BUYHOLDPRICE);
		osr.saleHoldNumber = msg->GetString(ZD_SALEHOLDNUMBER);
		osr.saleHoldOpenPrice = msg->GetString(ZD_SALEHOLDOPENPRICE);
		osr.saleHoldPrice = msg->GetString(ZD_SALEHOLDPRICE);
		osr.isCanceled = msg->GetString(ZD_ISCANCELED);
		osr.filledTotalFee = msg->GetString(ZD_FILLEDTOTALFEE);
		osr.oIndex = msg->GetString(ZD_OINDEX);
		osr.accountNo = msg->GetString(ZD_ACCOUNTNO);

		this->RtnOrderStatus(&osr);
	}
	if (msgType == ZD_AccountSearchBack)
	{
		string isLast = msg->GetString(ZD_INFOOVER);
		if (isLast == "1")
		{
			this->RtnAccountSearchReq(NULL, true);
			return;
		}
		AccountSearchRtn acr;
		acr.userId = msg->GetString(ZD_USERID);
		acr.accountNo = msg->GetString(ZD_ACCOUNTNO);

		acr.oldCanUse = msg->GetString(ZD_OLDCANUSE);
		acr.oldBalance = msg->GetString(ZD_OLDBALANCE);
		acr.oldAmount = msg->GetString(ZD_OLDAMOUNT);
		acr.inMoney = msg->GetString(ZD_INMONEY);
		acr.outMoney = msg->GetString(ZD_OUTMONEY);
		acr.todayBalance = msg->GetString(ZD_TODAYBALANCE);
		acr.todayAmount = msg->GetString(ZD_TODAYAMOUNT);
		acr.frozenDeposit = msg->GetString(ZD_FROZENDEPOSIT);
		acr.todayCanUse = msg->GetString(ZD_TODAYCANUSE);
		acr.deposit = msg->GetString(ZD_DEPOSIT);
		acr.commission = msg->GetString(ZD_COMMISSION);
		acr.expiredProfit = msg->GetString(ZD_EXPIREDPROFIT);
		acr.netProfit = msg->GetString(ZD_NETPROFIT);
		acr.profitRate = msg->GetString(ZD_PROFITRATE);
		acr.riskRate = msg->GetString(ZD_RISKRATE);
		acr.currencyNo = msg->GetString(ZD_CURRENCYNO);
		acr.currencyRate = msg->GetString(ZD_CURRENCYRATE);
		acr.unexpiredProfit = msg->GetString(ZD_UNEXPIREDPROFIT);
		acr.unaccountProfit = msg->GetString(ZD_UNACCOUNTPROFIT);
		acr.keepDeposit = msg->GetString(ZD_KEEPDEPOSIT);
		acr.royalty = msg->GetString(ZD_ROYALTY);

		this->RtnAccountSearchReq(&acr, false);
		return;
	}
	if (msgType == ZD_CancelBack)
	{
		string isLast = msg->GetString(ZD_INFOOVER);
		if (isLast == "1")
		{
			this->RtnCancelReq(NULL, true);
			return;
		}
		OrderCancelRtn ocr;
		
		ocr.userId=msg->GetString(ZD_USERID);
		ocr.accountNo=msg->GetString(ZD_ACCOUNTNO);
		ocr.systemNo=msg->GetString(ZD_SYSTEMNO);
		ocr.orderNo=msg->GetString(ZD_ORDERNO);
		ocr.cancelNo=msg->GetString(ZD_CANCELNO);
		ocr.exchangeCode=msg->GetString(ZD_EXCHANGECODE);
		ocr.code=msg->GetString(ZD_CODE);
		ocr.buySale=msg->GetString(ZD_BUYSALE);
		ocr.orderNumber=msg->GetString(ZD_ORDERNUMBER);
		ocr.orderPrice=msg->GetString(ZD_ORDERPRICE);
		ocr.filledNumber=msg->GetString(ZD_FILLEDNUMBER);
		ocr.cancelNumber=msg->GetString(ZD_CANCELNUMBER);
		ocr.cancelDate=msg->GetString(ZD_CANCELDATE);
		ocr.cancelTime=msg->GetString(ZD_CANCELTIME);
		ocr.errorCode=msg->GetString(ZD_ERRORCODE);


		this->RtnCancelReq(&ocr, false);
		return;
	}
	if (msgType == ZD_FillSearchBack)
	{
		string isLast = msg->GetString(ZD_INFOOVER);
		if (isLast == "1")
		{
			this->RtnFillSearchReq(NULL, true);
			return;
		}
		FillSearchRtn fsr;

		fsr.userId=msg->GetString(ZD_USERID);
		fsr.accountNo=msg->GetString(ZD_ACCOUNTNO);
		fsr.filledNo=msg->GetString(ZD_FILLEDNO);
		fsr.orderNo=msg->GetString(ZD_ORDERNO);
		fsr.systemNo=msg->GetString(ZD_SYSTEMNO);
		fsr.localNo=msg->GetString(ZD_LOCALNO);
		fsr.exchangeCode=msg->GetString(ZD_EXCHANGECODE);
		fsr.code=msg->GetString(ZD_CODE);
		fsr.buySale=msg->GetString(ZD_BUYSALE);
		fsr.filledNumber=msg->GetString(ZD_FILLEDNUMBER);
		fsr.openPrice=msg->GetString(ZD_OPENPRICE);
		fsr.filledDate=msg->GetString(ZD_FILLEDDATE);
		fsr.fee=msg->GetString(ZD_FEE);
		fsr.errorCode=msg->GetString(ZD_ERRORCODE);
		fsr.changeDate=msg->GetString(ZD_CHANGEDATE);
		fsr.filledType=msg->GetString(ZD_FILLEDTYPE);
				
		this->RtnFillSearchReq(&fsr, false);
		return;
	}
	if (msgType == ZD_ModifyBack)
	{
		ModifyRtn mr;
		mr.userId = msg->GetString(ZD_USERID);
		mr.accountNo = msg->GetString(ZD_ACCOUNTNO);
		mr.systemNo = msg->GetString(ZD_SYSTEMNO);
		mr.localNo = msg->GetString(ZD_LOCALNO);
		mr.orderNo = msg->GetString(ZD_ORDERNO);
		mr.orderMethod = msg->GetString(ZD_ORDERMETHOD);
		mr.acceptType = msg->GetString(ZD_ACCEPTTYPE);
		mr.exchangeCode = msg->GetString(ZD_EXCHANGECODE);
		mr.code = msg->GetString(ZD_CODE);
		mr.buySale = msg->GetString(ZD_BUYSALE);
		mr.orderNumber = msg->GetString(ZD_ORDERNUMBER);
		mr.orderPrice = msg->GetString(ZD_ORDERPRICE);
		mr.priceType = msg->GetString(ZD_PRICETYPE);
		mr.cancelDate = msg->GetString(ZD_CANCELDATE);
		mr.cancelTime = msg->GetString(ZD_CANCELTIME);
		mr.errorCode = msg->GetString(ZD_ERRORCODE);
		mr.status = msg->GetString(ZD_STATUS);
		mr.fIsRiskOrder = msg->GetString(ZD_FISRISKORDER);
		mr.triggerPrice = msg->GetString(ZD_TRIGGERPRICE);
		
		return this->RtnModifyReq(&mr);
	}
	if (msgType == ZD_OrderSearchBack)
	{
		string isLast = msg->GetString(ZD_INFOOVER);
		if (isLast == "1")
		{
			this->RtnOrderSearchReq(NULL, true);
			return;
		}
		OrderSearchRtn osr;

		osr.userId=msg->GetString(ZD_USERID);
		osr.accountNo=msg->GetString(ZD_ACCOUNTNO);
		osr.systemNo=msg->GetString(ZD_SYSTEMNO);
		osr.localNo=msg->GetString(ZD_LOCALNO);
		osr.orderNo=msg->GetString(ZD_ORDERNO);
		osr.origOrderNo=msg->GetString(ZD_ORIGORDERNO);
		osr.exchangeCode=msg->GetString(ZD_EXCHANGECODE);
		osr.code=msg->GetString(ZD_CODE);
		osr.buySale=msg->GetString(ZD_BUYSALE);
		osr.orderNumber=msg->GetString(ZD_ORDERNUMBER);
		osr.orderPrice=msg->GetString(ZD_ORDERPRICE);
		osr.filledNumber=msg->GetString(ZD_FILLEDNUMBER);
		osr.openPrice=msg->GetString(ZD_OPENPRICE);
		osr.priceType=msg->GetString(ZD_PRICETYPE);
		osr.cancelDate=msg->GetString(ZD_CANCELDATE);
		osr.cancelTime=msg->GetString(ZD_CANCELTIME);
		osr.errorCode=msg->GetString(ZD_ERRORCODE);
		osr.status=msg->GetString(ZD_STATUS);
		osr.fIsRiskOrder=msg->GetString(ZD_FISRISKORDER);
		osr.cancelUserId=msg->GetString(ZD_CANCELUSERID);
		osr.orderDate=msg->GetString(ZD_ORDERDATE);
		osr.orderTime=msg->GetString(ZD_ORDERTIME);

		return this->RtnOrderSearchReq(&osr, false);
	}
	if (msgType == ZD_MarketBack)
	{
		MarketRtn mr;
		mr.exchangeCode = msg->GetString(ZD_EXCHANGECODE);
		mr.code = msg->GetString(ZD_CODE);
		mr.buyPrice = msg->GetString(ZD_BUYPRICE);
		mr.buyNumber = msg->GetString(ZD_BUYNUMBER);
		mr.salePrice = msg->GetString(ZD_SALEPRICE);
		mr.saleNumber = msg->GetString(ZD_SALENUMBER);
		mr.currPrice = msg->GetString(ZD_CURRPRICE);
		mr.currNumber = msg->GetString(ZD_CURRNUMBER);
		mr.high = msg->GetString(ZD_HIGH);
		mr.low = msg->GetString(ZD_LOW);
		mr.open = msg->GetString(ZD_OPEN);
		mr.oldClose = msg->GetString(ZD_OLDCLOSE);
		mr.close = msg->GetString(ZD_CLOSE);
		mr.time = msg->GetString(ZD_TIME);
		mr.filledNum = msg->GetString(ZD_FILLEDNUM);
		mr.holdNum = msg->GetString(ZD_HOLDNUM);
		mr.buyPrice2 = msg->GetString(ZD_BUYPRICE2);
		mr.buyPrice3 = msg->GetString(ZD_BUYPRICE3);
		mr.buyPrice4 = msg->GetString(ZD_BUYPRICE4);
		mr.buyPrice5 = msg->GetString(ZD_BUYPRICE5);
		mr.buyNumber2 = msg->GetString(ZD_BUYNUMBER2);
		mr.buyNumber3 = msg->GetString(ZD_BUYNUMBER3);
		mr.buyNumber4 = msg->GetString(ZD_BUYNUMBER4);
		mr.buyNumber5 = msg->GetString(ZD_BUYNUMBER5);
		mr.salePrice2 = msg->GetString(ZD_SALEPRICE2);
		mr.salePrice3 = msg->GetString(ZD_SALEPRICE3);
		mr.salePrice4 = msg->GetString(ZD_SALEPRICE4);
		mr.salePrice5 = msg->GetString(ZD_SALEPRICE5);
		mr.saleNumber2 = msg->GetString(ZD_SALENUMBER2);
		mr.saleNumber3 = msg->GetString(ZD_SALENUMBER3);
		mr.saleNumber4 = msg->GetString(ZD_SALENUMBER4);
		mr.saleNumber5 = msg->GetString(ZD_SALENUMBER5);
		mr.hideBuyPrice = msg->GetString(ZD_HIDEBUYPRICE);
		mr.hideBuyNumber = msg->GetString(ZD_HIDEBUYNUMBER);
		mr.hideSalePrice = msg->GetString(ZD_HIDESALEPRICE);
		mr.hideSaleNumber = msg->GetString(ZD_HIDESALENUMBER);
		return this->RtnMarket(&mr);
	}

	if (msgType == ZD_MarketFive)
	{
		MarketFiveRtn mfr;
		
		mfr.exchangeCode = msg->GetString(ZD_EXCHANGECODE);
		mfr.code = msg->GetString(ZD_CODE);
		mfr.buyPrice = msg->GetString(ZD_BUYPRICE);
		mfr.buyNumber = msg->GetString(ZD_BUYNUMBER);
		mfr.salePrice = msg->GetString(ZD_SALEPRICE);
		mfr.saleNumber = msg->GetString(ZD_SALENUMBER);
		mfr.time = msg->GetString(ZD_TIME);
		mfr.buyPrice2 = msg->GetString(ZD_BUYPRICE2);
		mfr.buyPrice3 = msg->GetString(ZD_BUYPRICE3);
		mfr.buyPrice4 = msg->GetString(ZD_BUYPRICE4);
		mfr.buyPrice5 = msg->GetString(ZD_BUYPRICE5);
		mfr.buyNumber2 = msg->GetString(ZD_BUYNUMBER2);
		mfr.buyNumber3 = msg->GetString(ZD_BUYNUMBER3);
		mfr.buyNumber4 = msg->GetString(ZD_BUYNUMBER4);
		mfr.buyNumber5 = msg->GetString(ZD_BUYNUMBER5);
		mfr.salePrice2 = msg->GetString(ZD_SALEPRICE2);
		mfr.salePrice3 = msg->GetString(ZD_SALEPRICE3);
		mfr.salePrice4 = msg->GetString(ZD_SALEPRICE4);
		mfr.salePrice5 = msg->GetString(ZD_SALEPRICE5);
		mfr.saleNumber2 = msg->GetString(ZD_SALENUMBER2);
		mfr.saleNumber3 = msg->GetString(ZD_SALENUMBER3);
		mfr.saleNumber4 = msg->GetString(ZD_SALENUMBER4);
		mfr.saleNumber5 = msg->GetString(ZD_SALENUMBER5);
		mfr.hideBuyPrice = msg->GetString(ZD_HIDEBUYPRICE);
		mfr.hideBuyNumber = msg->GetString(ZD_HIDEBUYNUMBER);
		mfr.hideSalePrice = msg->GetString(ZD_HIDESALEPRICE);
		mfr.hideSaleNumber = msg->GetString(ZD_HIDESALENUMBER);

		return this->RtnMarketFive(&mfr);
	}
	if (msgType == ZD_MarketNew)
	{
		MarketNewRtn mnr;

		mnr.exchangeCode = msg->GetString(ZD_EXCHANGECODE);
		mnr.code = msg->GetString(ZD_CODE);
		mnr.buyPrice = msg->GetString(ZD_BUYPRICE);
		mnr.buyNumber = msg->GetString(ZD_BUYNUMBER);
		mnr.salePrice = msg->GetString(ZD_SALEPRICE);
		mnr.saleNumber = msg->GetString(ZD_SALENUMBER);
		mnr.currPrice = msg->GetString(ZD_CURRPRICE);
		mnr.currNumber = msg->GetString(ZD_CURRNUMBER);
		mnr.high = msg->GetString(ZD_HIGH);
		mnr.low = msg->GetString(ZD_LOW);
		mnr.open = msg->GetString(ZD_OPEN);
		mnr.oldClose = msg->GetString(ZD_OLDCLOSE);
		mnr.close = msg->GetString(ZD_CLOSE);
		mnr.time = msg->GetString(ZD_TIME);
		mnr.filledNum = msg->GetString(ZD_FILLEDNUM);
		mnr.holdNum = msg->GetString(ZD_HOLDNUM);
		mnr.hideBuyPrice = msg->GetString(ZD_HIDEBUYPRICE);
		mnr.hideBuyNumber = msg->GetString(ZD_HIDEBUYNUMBER);
		mnr.hideSalePrice = msg->GetString(ZD_HIDESALEPRICE);
		mnr.hideSaleNumber = msg->GetString(ZD_HIDESALENUMBER);
		
		return this->RtnMarketNew(&mnr);
	}
	if (msgType == ZD_SysNo)
	{
		SystemNoRtn snr;
		snr.accountNo = msg->GetString(ZD_ACCOUNTNO);
		snr.userId = msg->GetString(ZD_USERID);
		snr.tradePwd = msg->GetString(ZD_TRADEPWD);
		snr.userType = msg->GetString(ZD_USERTYPE);
		snr.todayCanUse = msg->GetString(ZD_TODAYCANUSE);
		snr.systemNo = msg->GetString(ZD_SYSTEMNO);
		snr.localNo = msg->GetString(ZD_LOCALNO);
		snr.exchangeCode = msg->GetString(ZD_EXCHANGECODE);
		snr.code = msg->GetString(ZD_CODE);
		snr.buySale = msg->GetString(ZD_BUYSALE);
		snr.orderNumber = msg->GetString(ZD_ORDERNUMBER);
		snr.orderPrice = msg->GetString(ZD_ORDERPRICE);
		snr.tradeType = msg->GetString(ZD_TRADETYPE);
		snr.priceType = msg->GetString(ZD_PRICETYPE);
		snr.htsType = msg->GetString(ZD_HTSTYPE);
		snr.fIsRiskOrder = msg->GetString(ZD_FISRISKORDER);
		snr.errorCode = msg->GetString(ZD_ERRORCODE);

		return this->RtnSystemNo(&snr);

	}
	
	if (msgType == ZD_GetUseControl)
	{
		string isLast = msg->GetString(ZD_INFOOVER);
		if (isLast == "1")
		{
			this->RtnGetUserControlReq(NULL, true);
			return;
		}
		ControlRtn cr;
		cr.zd_Code = msg->GetString(ZD_ZD_CODE);
		cr.exchangeCode = msg->GetString(ZD_EXCHANGECODE);
		cr.code = msg->GetString(ZD_CODE);
		cr.codeName = msg->GetString(ZD_CODENAME);
		cr.comNo = msg->GetString(ZD_COMNO);
		cr.comName = msg->GetString(ZD_COMNAME);
		cr.comType = msg->GetString(ZD_COMTYPE);
		cr.currencyNo = msg->GetString(ZD_CURRENCYNO);
		cr.currencyName = msg->GetString(ZD_CURRENCYNAME);
		cr.productDot = msg->GetString(ZD_PRODUCTDOT);
		cr.upperTick = msg->GetString(ZD_UPPERTICK);
		cr.exchangeName = msg->GetString(ZD_EXCHANGENAME);
		cr.settlePrice = msg->GetString(ZD_SETTLEPRICE);
		cr.cancelDate = msg->GetString(ZD_CANCELDATE);
		cr.marketDot = msg->GetString(ZD_MARKETDOT);
		cr.marketUnit = msg->GetString(ZD_MARKETUNIT);
		cr.fDotNum_Carry = msg->GetString(ZD_FDOTNUM_CARRY);
		cr.fUpperTick_Carry = msg->GetString(ZD_FUPPERTICK_CARRY);
		cr.fFirstNoticeDay = msg->GetString(ZD_FFIRSTNOTICEDAY);
		cr.fFreezenPercent = msg->GetString(ZD_FFREEZENPERCENT);
		cr.fFreezenMoney = msg->GetString(ZD_FFREENZENMONEY);
		cr.fFreePercent = msg->GetString(ZD_FFREEPERCENT);
		cr.fPriceStrike = msg->GetString(ZD_FPRICESTRIKE);
		cr.fProductDotStrike = msg->GetString(ZD_FPRODUCTDOTSTRIKE);
		cr.fUpperTickStrike = msg->GetString(ZD_FUPPERTICKSTRIKE);

		return this->RtnGetUserControlReq(&cr, false);
	}
	if (msgType == ZD_ExchangeSearchBack)
	{
		string isLast = msg->GetString(ZD_INFOOVER);
		if (isLast == "1")
		{
			this->RtnExchangeSearchReq(NULL, true);
			return;
		}
		ExchangeSearchRtn esr;
		esr.exchangeCode = msg->GetString(ZD_EXCHANGECODE);
		esr.exchangeName = msg->GetString(ZD_EXCHANGENAME);

		this->RtnExchangeSearchReq(&esr, false);
		return;
	}
	if (msgType == ZD_CurrencySearchBack)
	{
		LoginRtn lr;
		lr.accountNo = msg->GetString(ZD_ACCOUNTNO);
		this->RtnLoginReq(&lr, true);
	}
	if (msgType == ZD_OpenDetailBack)
	{
		string isLast = msg->GetString(ZD_INFOOVER);
		if (isLast == "1")
		{
			this->RtnOpenDetailReq(NULL, true);
			return;
		}
		OpenDetailRtn odr;
		
		odr.clientNo=msg->GetString(ZD_CLIENTNO);
		odr.accountNo=msg->GetString(ZD_ACCOUNTNO);
		odr.exchangeCode=msg->GetString(ZD_EXCHANGECODE);
		odr.currencyNo=msg->GetString(ZD_CURRENCYNO);
		odr.comNo=msg->GetString(ZD_COMNO);
		odr.code=msg->GetString(ZD_CODE);
		odr.filledDate=msg->GetString(ZD_FILLEDDATE);
		odr.filledNo=msg->GetString(ZD_FILLEDNO);
		odr.buySale=msg->GetString(ZD_BUYSALE);
		odr.openNumber=msg->GetString(ZD_OPENNUMBER);
		odr.initOpenPrice=msg->GetString(ZD_INITOPENPRICE);
		odr.openPrice=msg->GetString(ZD_OPENPRICE);
		odr.status=msg->GetString(ZD_STATUS);
		odr.changeDate=msg->GetString(ZD_CHANGEDATE);

		return this->RtnOpenDetailReq(&odr, false);
	}
}

#pragma endregion


void ZdSpi::Init()
{
	InHandle = GetShZdTradeLib();
	InHandle->InitShZdServer();
	InHandle->RegisterOutLib(this);
	InHandle->RegisterFront("222.73.106.130", 7787);//上海电信
	//InHandle->RegisterFront("203.186.175.82", 9787);//香港电信
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
	
	/*for (int i = 1; i <= 1;i++)
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
	}*/

	OrderCancelReq ocr = OrderCancelReq();
	//ocr.userId = "demo000666";
	//ocr.accountNo = "00003152";
	ocr.systemNo = "00003152200005";
	ocr.orderNo = "1006437";
	/*ocr.exchangeCode = "CME";
	ocr.code = "6A1412";
	ocr.buySale = "1";
	ocr.orderNumber = "1";
	ocr.orderPrice = "0.8411";
	ocr.filledNumber = "0";*/

	ReqCancel(ocr);
	

	WaitForSingleObject(g_Event, INFINITE);

	cout << "good" << endl;
	//test github
}




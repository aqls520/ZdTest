#pragma once

#include "Lib_dll/ShZdMessage.h"
#include "Lib_dll/ShZdTradeLib.h"

#include "MyDefine.h"
#include "MyStruct.h"

class ZdSpi:public IShZdTradeOutLib
{
public:
	virtual int OnReceiveTradeInfo(const CShZdMessage * re);
	virtual int OnReceiveMarketInfo(const CShZdMessage * re);
	virtual int OnReceiveErrorInfo(int errorCode, const char* re);

	CShZdMessage* GetZdReqMessage(string MsgType, void* Msg);
	void ChgRspFunction(CShZdMessage* msg);
	void Init();

	//请求
public:
	void ReqLogin(LoginReq lr);//登录
	void ReqAccountSearch(AccountSearchReq acr);//账户查询
	void ReqExchangeSearch(ExchangeSearchReq esr);//交易所查询
	void ReqOrder(OrderReq or);//报单
	void ReqCancel(OrderCancelReq ocr);//撤单
	void ReqModify(ModifyReq mr);
	void ReqGetUserControl(ControlReq cr);//查询可用合约
	void ReqMarket(MarketReq mr);//订阅行情
	void ReqFillSearch(FillSearchReq fsr);//查询成交
	void ReqOrderSearch(OrderSearchReq osr);//查询报单
	void ReqOpenDetail(OpenDetailReq odr);//查询持仓

	//响应
public:
	//登录响应
	void RtnLoginReq(LoginRtn* lr, bool bIsLast);
	//账户查询响应
	void RtnAccountSearchReq(AccountSearchRtn* acr, bool bIsLast);
	//交易所查询响应
	void RtnExchangeSearchReq(ExchangeSearchRtn* esr, bool bIsLast);
	//请求可用合约响应
	void RtnGetUserControlReq(ControlRtn* cr,bool bIsLast);
	//完整行情响应
	void RtnMarket(MarketRtn* mr);
	//五档行情响应
	void RtnMarketFive(MarketFiveRtn* mfr);
	//成交行情响应
	void RtnMarketNew(MarketNewRtn* mnr);
	//（当日）成交查询响应
	void RtnFillSearchReq(FillSearchRtn* fsr,bool bIsLast);
	//（当日）订单查询响应
	void RtnOrderSearchReq(OrderSearchRtn* osr,bool bIsLast);
	//持仓查询响应
	void RtnOpenDetailReq(OpenDetailRtn* odr,bool bIsLast);
	//撤单响应
	void RtnCancelReq(OrderCancelRtn* ocr,bool bIsLast);

	void RtnSystemNo(SystemNoRtn* snr);
	void RtnOrderBack(OrderBackRtn* obr); //O1:报单返回
	void RtnOrderStatus(OrderStatusRtn* osr); //OST:报单状态返回
	void RtnOrderAccount(OrderAccountRtn* oar);//O2：报单资金返还
	void RtnOrderSysNo(); //SY：报单返回系统号
	void RtnOrderFilled(OrderFilledRtn* ofr);//O3：报单成交返回

	void RtnModifyReq(ModifyRtn* mr);


protected:
	ShZdTradeInLibHandle InHandle;
private:
};
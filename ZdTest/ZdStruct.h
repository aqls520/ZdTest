#pragma once

#include <string>
using namespace std;

struct LoginReq
{
	string userId;
	string userPwd;
};

struct LoginRtn
{
	string userId;
	string userName;
	string userType;
	string userPwd;
	string accountNo;
	string tradePwd;
	string isSimulation;
	string currencyNo;
	string status;
	string FSelAll;
};

struct OrderReq
{
	string userId;
	string accountNo;
	string tradePwd;
	string exchangeCode;
	string code;
	string buySale;
	string orderNumber;
	string orderPrice;
	string priceType;
	string localN0;
	string triggerPrice;
	string validDate;

};

struct SystemNoRtn
{
	string accountNo;
	string userId;
	string tradePwd;
	string userType;
	string todayCanUse;
	string systemNo;
	string localNo;
	string exchangeCode;
	string code;
	string buySale;
	string orderNumber;
	string orderPrice;
	string tradeType;
	string priceType;
	string htsType;
	string fIsRiskOrder;
	string errorCode;
};

struct OrderBackRtn
{
	string userId;
	string accountNo;
	string systemNo;
	string localNo;
	string orderNo;
	string origOrderNo;
	string exchangeCode;
	string code;
	string buySale;
	string orderNumber;
	string orderPrice;
	string priceType;
	string orderDate;
	string orderTime;
	string errorCode;
	string status;
	string fIsRiskOrder;
};

struct OrderAccountRtn
{
	string clientNo;
	string accountNo;
	string oldCanUse;
	string todayAmount;
	string expiredProfit;
	string frozenDeposit;
	string fee;
	string deposit;
	string keepDeposit;
	string inMoney;
	string outMoney;
	string unexpiredProfit;
	string unaccountProfit;
	string royalty;

};

struct OrderStatusRtn
{
	string exchangeCode;
	string zd_Code;
	string orderNo;
	string orderNumber;
	string filledNumber;
	string filledAdvPrice;
	string buyHoldNumber;
	string buyHoldOpenPrice;
	string buyHoldPrice;
	string saleHoldNumber;
	string saleHoldOpenPrice;
	string saleHoldPrice;
	string isCanceled;
	string filledTotalFee;
	string oIndex;
	string accountNo;

};

struct OrderFilledRtn
{
	string userId;
	string accountNo;
	string filledNo;
	string orderNo;
	string systemNo;
	string localNo;
	string exchangeCode;
	string code;
	string buySale;
	string filledNumber;
	string openPrice;
	string filledDate;
	string cancelTime;
	string fee;
	string errorCode;
	string changeDate;
	string filledType;

};

struct OrderCancelReq
{
	string userId;
	string accountNo;
	string tradePwd;
	string systemNo;//±ØÌî
	string orderNo;//±ØÌî
	string exchangeCode;
	string code;
	string buySale;
	string orderNumber;
	string orderPrice;
	string filledNumber;

};

struct OrderCancelRtn
{
	string userId;
	string accountNo;
	string systemNo;
	string orderNo;
	string cancelNo;
	string exchangeCode;
	string code;
	string buySale;
	string orderNumber;
	string orderPrice;
	string filledNumber;
	string cancelNumber;
	string cancelDate;
	string cancelTime;
	string errorCode;

};

struct AccountSearchReq
{
	string userId;
	string accountNo;
	string tradePwd;

};

struct AccountSearchRtn
{
	string userId;
	string accountNo;
	string oldCanUse;
	string oldBalance;
	string oldAmount;
	string inMoney;
	string outMoney;
	string todayBalance;
	string todayAmount;
	string frozenDeposit;
	string todayCanUse;
	string deposit;
	string commission;
	string expiredProfit;
	string netProfit;
	string profitRate;
	string riskRate;
	string currencyNo;
	string currencyRate;
	string unexpiredProfit;
	string unaccountProfit;
	string keepDeposit;
	string royalty;


};

struct FillSearchReq
{
	string userId;
	string tradePwd;
	string systemNo;

};

struct FillSearchRtn
{
	string userId;
	string accountNo;
	string filledNo;
	string orderNo;
	string systemNo;
	string localNo;
	string exchangeCode;
	string code;
	string buySale;
	string filledNumber;
	string openPrice;
	string filledDate;
	string fee;
	string errorCode;
	string changeDate;
	string filledType;

};

struct ExchangeSearchReq
{
	string accountNo;
};

struct ExchangeSearchRtn
{
	string exchangeCode;
	string exchangeName;

};

struct CurrencySearchReq
{
	string accountNo;
};

struct CurrencySearchRtn
{
	string currencyNo;
	string isCurrencyBase;
	string currencyRate;

};

struct OrderSearchReq
{
	string userId;
	string accountNo;
	string tradePwd;
	string todayCanuse;
	string systemNo;

};

struct OrderSearchRtn
{
	string userId;
	string accountNo;
	string systemNo;
	string localNo;
	string orderNo;
	string origOrderNo;
	string exchangeCode;
	string code;
	string buySale;
	string orderNumber;
	string orderPrice;
	string filledNumber;
	string openPrice;
	string priceType;
	string cancelDate;
	string cancelTime;
	string errorCode;
	string status;
	string fIsRiskOrder;
	string cancelUserId;
	string orderDate;
	string orderTime;

};

struct OpenDetailReq
{
	string userId;
	string tradePwd;

};

struct OpenDetailRtn
{
	string clientNo;
	string accountNo;
	string exchangeCode;
	string currencyNo;
	string comNo;
	string code;
	string filledDate;
	string filledNo;
	string buySale;
	string openNumber;
	string initOpenPrice;
	string openPrice;
	string status;
	string changeDate;

};

struct ModifyReq
{
	string userId;
	string accountNo;
	string tradePwd;
	string orderNo;
	string exchangeCode;
	string code;
	string buySale;
	string orderNumber;
	string orderPrice;
	string filledNumber;
	string modifyNumber;
	string modifyPrice;
	string fIsRiskOrder;
	string triggerPrice;
	string modifyTriggerPrice;
	string systemNo;
	string priceType;

};

struct ModifyRtn
{
	string userId;
	string accountNo;
	string systemNo;
	string localNo;
	string orderNo;
	string orderMethod;
	string acceptType;
	string exchangeCode;
	string code;
	string buySale;
	string orderNumber;
	string orderPrice;
	string priceType;
	string cancelDate;
	string cancelTime;
	string errorCode;
	string status;
	string fIsRiskOrder;
	string triggerPrice;

};

struct ControlReq
{
	string accountNo;
};

struct ControlRtn
{

	string zd_Code;
	string exchangeCode;
	string code;
	string codeName;
	string comNo;
	string comName;
	string comType;
	string currencyNo;
	string currencyName;
	string productDot;
	string upperTick;
	string exchangeName;
	string settlePrice;
	string cancelDate;
	string marketDot;
	string marketUnit;
	string fDotNum_Carry;
	string fUpperTick_Carry;
	string fFirstNoticeDay;
	string fFreezenPercent;
	string fFreezenMoney;
	string fFreePercent;
	string fPriceStrike;
	string fProductDotStrike;
	string fUpperTickStrike;

};

struct MarketReq
{
	string todayCanUse;
	string code;
	string accountNo;
};

struct MarketRtn
{
	string exchangeCode;
	string code;
	string buyPrice;
	string buyNumber;
	string salePrice;
	string saleNumber;
	string currPrice;
	string currNumber;
	string high;
	string low;
	string open;
	string oldClose;
	string close;
	string time;
	string filledNum;
	string holdNum;
	string buyPrice2;
	string buyPrice3;
	string buyPrice4;
	string buyPrice5;
	string buyNumber2;
	string buyNumber3;
	string buyNumber4;
	string buyNumber5;
	string salePrice2;
	string salePrice3;
	string salePrice4;
	string salePrice5;
	string saleNumber2;
	string saleNumber3;
	string saleNumber4;
	string saleNumber5;
	string hideBuyPrice;
	string hideBuyNumber;
	string hideSalePrice;
	string hideSaleNumber;


};

struct MarketFiveRtn
{
	string exchangeCode;
	string code;
	string buyPrice;
	string buyNumber;
	string salePrice;
	string saleNumber;
	string time;
	string buyPrice2;
	string buyPrice3;
	string buyPrice4;
	string buyPrice5;
	string buyNumber2;
	string buyNumber3;
	string buyNumber4;
	string buyNumber5;
	string salePrice2;
	string salePrice3;
	string salePrice4;
	string salePrice5;
	string saleNumber2;
	string saleNumber3;
	string saleNumber4;
	string saleNumber5;
	string hideBuyPrice;
	string hideBuyNumber;
	string hideSalePrice;
	string hideSaleNumber;

};

struct MarketNewRtn
{
	string exchangeCode;
	string code;
	string buyPrice;
	string buyNumber;
	string salePrice;
	string saleNumber;
	string currPrice;
	string currNumber;
	string high;
	string low;
	string open;
	string oldClose;
	string close;
	string time;
	string filledNum;
	string holdNum;
	string hideBuyPrice;
	string hideBuyNumber;
	string hideSalePrice;
	string hideSaleNumber;

};

struct MarketPrRtn
{
	string code;
	string oldClose;

};
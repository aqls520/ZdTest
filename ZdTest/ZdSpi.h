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

	//����
public:
	void ReqLogin(LoginReq lr);//��¼
	void ReqAccountSearch(AccountSearchReq acr);//�˻���ѯ
	void ReqExchangeSearch(ExchangeSearchReq esr);//��������ѯ
	void ReqOrder(OrderReq or);//����
	void ReqCancel(OrderCancelReq ocr);//����
	void ReqModify(ModifyReq mr);
	void ReqGetUserControl(ControlReq cr);//��ѯ���ú�Լ
	void ReqMarket(MarketReq mr);//��������
	void ReqFillSearch(FillSearchReq fsr);//��ѯ�ɽ�
	void ReqOrderSearch(OrderSearchReq osr);//��ѯ����
	void ReqOpenDetail(OpenDetailReq odr);//��ѯ�ֲ�

	//��Ӧ
public:
	//��¼��Ӧ
	void RtnLoginReq(LoginRtn* lr, bool bIsLast);
	//�˻���ѯ��Ӧ
	void RtnAccountSearchReq(AccountSearchRtn* acr, bool bIsLast);
	//��������ѯ��Ӧ
	void RtnExchangeSearchReq(ExchangeSearchRtn* esr, bool bIsLast);
	//������ú�Լ��Ӧ
	void RtnGetUserControlReq(ControlRtn* cr,bool bIsLast);
	//����������Ӧ
	void RtnMarket(MarketRtn* mr);
	//�嵵������Ӧ
	void RtnMarketFive(MarketFiveRtn* mfr);
	//�ɽ�������Ӧ
	void RtnMarketNew(MarketNewRtn* mnr);
	//�����գ��ɽ���ѯ��Ӧ
	void RtnFillSearchReq(FillSearchRtn* fsr,bool bIsLast);
	//�����գ�������ѯ��Ӧ
	void RtnOrderSearchReq(OrderSearchRtn* osr,bool bIsLast);
	//�ֲֲ�ѯ��Ӧ
	void RtnOpenDetailReq(OpenDetailRtn* odr,bool bIsLast);
	//������Ӧ
	void RtnCancelReq(OrderCancelRtn* ocr,bool bIsLast);

	void RtnSystemNo(SystemNoRtn* snr);
	void RtnOrderBack(OrderBackRtn* obr); //O1:��������
	void RtnOrderStatus(OrderStatusRtn* osr); //OST:����״̬����
	void RtnOrderAccount(OrderAccountRtn* oar);//O2�������ʽ𷵻�
	void RtnOrderSysNo(); //SY����������ϵͳ��
	void RtnOrderFilled(OrderFilledRtn* ofr);//O3�������ɽ�����

	void RtnModifyReq(ModifyRtn* mr);


protected:
	ShZdTradeInLibHandle InHandle;
private:
};
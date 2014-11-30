#pragma once

#define	CLIENTNO	10
#define	ACCOUNTNO	11
#define	USERID	12
#define	TRADEPWD	13
#define	USERTYPE	14
#define	ISSIMULATION	15
#define	USERPWD	16
#define	NEWUSERPWD	17
#define	FSTATE	18
#define	FSELALL	19
#define	FRONTENDIP	20
#define	FRONTENDPORT	21
#define	USERNAME	22
#define	CURRENCYNO	200
#define	TODAYCANUSE	201
#define	CANCASHOUT	202
#define	TODAYBALANCE	203
#define	TODAYAMOUNT	204
#define	EXPIREDPROFIT	205
#define	FLOATINGPROFIT	206
#define	NETPROFIT	207
#define	PROFITRATE	208
#define	RISKRATE	209
#define	FROZENDEPOSIT	210
#define	FEE	211
#define	DEPOSIT	212
#define	KEEPDEPOSIT	213
#define	INMONEY	214
#define	OUTMONEY	215
#define	UNEXPIREDPROFIT	216
#define	OLDCANUSE	217
#define	OLDBALANCE	218
#define	OLDAMOUNT	219
#define	YESTDAYCANOUT	220
#define	COMMISSION	221
#define	CHANGECOMMISION	222
#define	CURRENCYNAME	223
#define	UNACCOUNTPROFIT	224
#define	ISCURRENCYBASE	225
#define	CURRENCYRATE	226
#define	ZD_CODE	227
#define	SYSTEMNO	300
#define	ORDERNO	301
#define	CANCELNO	302
#define	CHANGENO	303
#define	FILLEDNO	304
#define	LOCALNO	305
#define	EXCHANGECODE	306
#define	CODE	307
#define	BUYSALE	308
#define	ORDERNUMBER	309
#define	ORDERPRICE	310
#define	CHANGEPRICE	312
#define	OPENPRICE	313
#define	INITOPENPRICE	314
#define	FILLEDNUMBER	315
#define	CANCELNUMBER	316
#define	CHANGENUMBER	317
#define	OPENNUMBER	318
#define	CASEPROFIT	319
#define	MARKETPROFIT	320
#define	SINGLEPROFIT	321
#define	ADDREDUCE	322
#define	ORIGORDERNO	323
#define	CANCELUSERID	324
#define	CANCELDATE	325
#define	CANCELTIME	326
#define	CHANGEDATE	327
#define	FILLEDDATE	328
#define	OLDDELIVERYDATE	329
#define	FILLEDDATETIME	330
#define	COMCODE	331
#define	CODENAME	332
#define	COMNO	333
#define	COMNAME	334
#define	COMTYPE	335
#define	PRODUCTDOT	336
#define	UPPERTICK	337
#define	EXCHANGENAME	338
#define	SETTLEPRICE	339
#define	MODIFYNUMBER	340
#define	MODIFYPRICE	341
#define	TRIGGERPRICE	342
#define	MODIFYTRIGGERPRICE	343
#define	MODIFYNO	344
#define	ORDERDATE	345
#define	ORDERTIME	346
#define	VALIDDATE	347
#define	FILLEDADVPRICE	348
#define	BUYHOLDNUMBER	349
#define	BUYHOLDOPENPRICE	350
#define	BUYHOLDPRICE	351
#define	SALEHOLDNUMBER	352
#define	SALEHOLDOPENPRICE	353
#define	SALEHOLDPRICE	354
#define	ISCANCELED	355
#define	FILLEDTOTALFEE	356
#define	OINDEX	357
#define	TRADETYPE	400
#define	PRICETYPE	401
#define	HTSTYPE	402
#define	FISRISKORDER	403
#define	ERRORCODE	404
#define	STATUS	405
#define	FILLEDTYPE	406
#define	ORDERMETHOD	407
#define	ACCEPTTYPE	408
#define	ERRORCONTENT	409
#define	INFOOVER	410
#define	CODEMONTH	411
#define	ROYALTY	412
#define	FLID	413
#define	STOPWINNO	414
#define	STOPLOSSPRICE	415
#define	STOPPROFITPRICE	416
#define	TRIGGERTYPE	417
#define	STOPLOSSDOT	418
#define	STOPPROFITDOT	419
#define	ORDERQUANTITY	420
#define	ISPERMANENT	421
#define	LOGOID	422
#define	FDOTNUM_CARRY	423
#define	FUPPERTICK_CARRY	424
#define	FFIRSTNOTICEDAY	425
#define	FFREEZENPERCENT	426
#define	FFREENZENMONEY	427
#define	FFREEMONEY	428
#define	FFREEPERCENT	429
#define	FPRICESTRIKE	430
#define	FPRODUCTDOTSTRIKE	431
#define	FUPPERTICKSTRIKE	432
#define	BUYPRICE	500
#define	BUYNUMBER	501
#define	SALEPRICE	502
#define	SALENUMBER	503
#define	CURRPRICE	504
#define	CURRNUMBER	505
#define	HIGH	506
#define	LOW	507
#define	OPEN	508
#define	OLDCLOSE	509
#define	CLOSE	510
#define	TIME	512
#define	FILLEDNUM	513
#define	HOLDNUM	514
#define	BUYPRICE2	515
#define	BUYPRICE3	516
#define	BUYPRICE4	517
#define	BUYPRICE5	518
#define	BUYNUMBER2	519
#define	BUYNUMBER3	520
#define	BUYNUMBER4	521
#define	BUYNUMBER5	522
#define	SALEPRICE2	523
#define	SALEPRICE3	524
#define	SALEPRICE4	525
#define	SALEPRICE5	526
#define	SALENUMBER2	527
#define	SALENUMBER3	528
#define	SALENUMBER4	529
#define	SALENUMBER5	530
#define	MARKETDOT	531
#define	MARKETUNIT	532
#define	HIDEBUYPRICE	533
#define	HIDEBUYNUMBER	534
#define	HIDESALEPRICE	535
#define	HIDESALENUMBER	536
#define	MARKETTIME	537
/*
#define	LOGON	A
#define	LOGOUT	5
#define	LOGONBACK	A1
#define	ORDER	O
#define	ORDERBACK	O1
#define	ORDERACCOUNT	O2
#define	ACCOUNTSEARCH	AC
#define	ACCOUNTSEARCHBACK	AC1
#define	CANCEL	C
#define	CANCELBACK	C1
#define	OPENSEARCH	OS
#define	OPENSEARCHBACK	OS1
#define	FILLSEARCH	FS
#define	FILLSEARCHBACK	FS1
#define	MODIFY	M
#define	MODIFYBACK	M1
#define	ORDERSEARCH	ORS
#define	ORDERSEARCHBACK	ORS1
#define	MARKET	MA
#define	MARKETBACK	MA1
#define	ERRORINFO	E
#define	SYSNO	SY
#define	FILLEDCAST	O3
#define	GETUSECONTROL	HY
#define	EXCHANGESEARCH	ES
#define	EXCHANGESEARCHBACK	ES1
#define	CURRENCYSEARCH	CS
#define	CURRENCYSEARCHBACK	CS1
#define	OPENSEARCHY	OSY
#define	OPENSEARCHYBACK	OSY1
#define	OPENDETAIL	OD
#define	OPENDETAILBACK	OD1
#define	STOPWINLIST	SWL
#define	STOPWINLISTBACK	SWL1
#define	SETYINGSUN	SYS
#define	SETYINGSUNBACK	SYS1
#define	DELYINGSUN	DYS
#define	DELYINGSUNBACK	DYS1
#define	MARKETFIVE	MA2
#define	MARKETNEW	MA3
#define	MARKETPR	MA4
#define	HEATBIT	HB
#define	ORDERSTATUS	OST
*/


//通讯类请求标志
#define	Logon	"A"
#define	Logout	"5"
#define	LogonBack	"A1"
#define	Order	"O"
#define	OrderBack	"O1"
#define	OrderAccount	"O2"
#define	AccountSearch	"AC"
#define	AccountSearchBack	"AC1"
#define	OrderCancel	"C"
#define	CancelBack	"C1"
#define	OpenSearch	"OS"
#define	OpenSearchBack	"OS1"
#define	FillSearch	"FS"
#define	FillSearchBack	"FS1"
#define	Modify	"M"
#define	ModifyBack	"M1"
#define	OrderSearch	"ORS"
#define	OrderSearchBack	"ORS1"
#define	Market	"MA"
#define	MarketBack	"MA1"
//#define	ErrorInfo	"E"
#define	SysNo	"SY"
#define	FilledCast	"O3"
#define	GetUseControl	"HY"
#define	ExchangeSearch	"ES"
#define	ExchangeSearchBack	"ES1"
#define	CurrencySearch	"CS"
#define	CurrencySearchBack	"CS1"
#define	OpenSearchY	"OSY"
#define	OpenSearchYBack	"OSY1"
#define	OpenDetail	"OD"
#define	OpenDetailBack	"OD1"
#define	StopWinList	"SWL"
#define	StopWinListBack	"SWL1"
#define	SetYingSun	"SYS"
#define	SetYingSunBack	"SYS1"
#define	DelYingSun	"DYS"
#define	DelYingSunBack	"DYS1"
#define	MarketFive	"MA2"
#define	MarketNew	"MA3"
#define	MarketPr	"MA4"
#define	HeatBit	"HB"
#define	OrderStatus	"OST"



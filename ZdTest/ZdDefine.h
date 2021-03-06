#pragma once

//字段对应Tag值
#define ZD_CLIENTNO 10
#define ZD_ACCOUNTNO 11
#define ZD_USERID 12
#define ZD_TRADEPWD 13
#define ZD_USERTYPE 14
#define ZD_ISSIMULATION 15
#define ZD_USERPWD 16
#define ZD_NEWUSERPWD 17
#define ZD_FSTATE 18
#define ZD_FSELALL 19
#define ZD_FRONTENDIP 20
#define ZD_FRONTENDPORT 21
#define ZD_USERNAME 22
#define ZD_CURRENCYNO 200
#define ZD_TODAYCANUSE 201
#define ZD_CANCASHOUT 202
#define ZD_TODAYBALANCE 203
#define ZD_TODAYAMOUNT 204
#define ZD_EXPIREDPROFIT 205
#define ZD_FLOATINGPROFIT 206
#define ZD_NETPROFIT 207
#define ZD_PROFITRATE 208
#define ZD_RISKRATE 209
#define ZD_FROZENDEPOSIT 210
#define ZD_FEE 211
#define ZD_DEPOSIT 212
#define ZD_KEEPDEPOSIT 213
#define ZD_INMONEY 214
#define ZD_OUTMONEY 215
#define ZD_UNEXPIREDPROFIT 216
#define ZD_OLDCANUSE 217
#define ZD_OLDBALANCE 218
#define ZD_OLDAMOUNT 219
#define ZD_YESTDAYCANOUT 220
#define ZD_COMMISSION 221
#define ZD_CHANGECOMMISION 222
#define ZD_CURRENCYNAME 223
#define ZD_UNACCOUNTPROFIT 224
#define ZD_ISCURRENCYBASE 225
#define ZD_CURRENCYRATE 226
#define ZD_ZD_CODE 227
#define ZD_SYSTEMNO 300
#define ZD_ORDERNO 301
#define ZD_CANCELNO 302
#define ZD_CHANGENO 303
#define ZD_FILLEDNO 304
#define ZD_LOCALNO 305
#define ZD_EXCHANGECODE 306
#define ZD_CODE 307
#define ZD_BUYSALE 308
#define ZD_ORDERNUMBER 309
#define ZD_ORDERPRICE 310
#define ZD_CHANGEPRICE 312
#define ZD_OPENPRICE 313
#define ZD_INITOPENPRICE 314
#define ZD_FILLEDNUMBER 315
#define ZD_CANCELNUMBER 316
#define ZD_CHANGENUMBER 317
#define ZD_OPENNUMBER 318
#define ZD_CASEPROFIT 319
#define ZD_MARKETPROFIT 320
#define ZD_SINGLEPROFIT 321
#define ZD_ADDREDUCE 322
#define ZD_ORIGORDERNO 323
#define ZD_CANCELUSERID 324
#define ZD_CANCELDATE 325
#define ZD_CANCELTIME 326
#define ZD_CHANGEDATE 327
#define ZD_FILLEDDATE 328
#define ZD_OLDDELIVERYDATE 329
#define ZD_FILLEDDATETIME 330
#define ZD_COMCODE 331
#define ZD_CODENAME 332
#define ZD_COMNO 333
#define ZD_COMNAME 334
#define ZD_COMTYPE 335
#define ZD_PRODUCTDOT 336
#define ZD_UPPERTICK 337
#define ZD_EXCHANGENAME 338
#define ZD_SETTLEPRICE 339
#define ZD_MODIFYNUMBER 340
#define ZD_MODIFYPRICE 341
#define ZD_TRIGGERPRICE 342
#define ZD_MODIFYTRIGGERPRICE 343
#define ZD_MODIFYNO 344
#define ZD_ORDERDATE 345
#define ZD_ORDERTIME 346
#define ZD_VALIDDATE 347
#define ZD_FILLEDADVPRICE 348
#define ZD_BUYHOLDNUMBER 349
#define ZD_BUYHOLDOPENPRICE 350
#define ZD_BUYHOLDPRICE 351
#define ZD_SALEHOLDNUMBER 352
#define ZD_SALEHOLDOPENPRICE 353
#define ZD_SALEHOLDPRICE 354
#define ZD_ISCANCELED 355
#define ZD_FILLEDTOTALFEE 356
#define ZD_OINDEX 357
#define ZD_TRADETYPE 400
#define ZD_PRICETYPE 401
#define ZD_HTSTYPE 402
#define ZD_FISRISKORDER 403
#define ZD_ERRORCODE 404
#define ZD_STATUS 405
#define ZD_FILLEDTYPE 406
#define ZD_ORDERMETHOD 407
#define ZD_ACCEPTTYPE 408
#define ZD_ERRORCONTENT 409
#define ZD_INFOOVER 410
#define ZD_CODEMONTH 411
#define ZD_ROYALTY 412
#define ZD_FLID 413
#define ZD_STOPWINNO 414
#define ZD_STOPLOSSPRICE 415
#define ZD_STOPPROFITPRICE 416
#define ZD_TRIGGERTYPE 417
#define ZD_STOPLOSSDOT 418
#define ZD_STOPPROFITDOT 419
#define ZD_ORDERQUANTITY 420
#define ZD_ISPERMANENT 421
#define ZD_LOGOID 422
#define ZD_FDOTNUM_CARRY 423
#define ZD_FUPPERTICK_CARRY 424
#define ZD_FFIRSTNOTICEDAY 425
#define ZD_FFREEZENPERCENT 426
#define ZD_FFREENZENMONEY 427
#define ZD_FFREEMONEY 428
#define ZD_FFREEPERCENT 429
#define ZD_FPRICESTRIKE 430
#define ZD_FPRODUCTDOTSTRIKE 431
#define ZD_FUPPERTICKSTRIKE 432
#define ZD_BUYPRICE 500
#define ZD_BUYNUMBER 501
#define ZD_SALEPRICE 502
#define ZD_SALENUMBER 503
#define ZD_CURRPRICE 504
#define ZD_CURRNUMBER 505
#define ZD_HIGH 506
#define ZD_LOW 507
#define ZD_OPEN 508
#define ZD_OLDCLOSE 509
#define ZD_CLOSE 510
#define ZD_TIME 512
#define ZD_FILLEDNUM 513
#define ZD_HOLDNUM 514
#define ZD_BUYPRICE2 515
#define ZD_BUYPRICE3 516
#define ZD_BUYPRICE4 517
#define ZD_BUYPRICE5 518
#define ZD_BUYNUMBER2 519
#define ZD_BUYNUMBER3 520
#define ZD_BUYNUMBER4 521
#define ZD_BUYNUMBER5 522
#define ZD_SALEPRICE2 523
#define ZD_SALEPRICE3 524
#define ZD_SALEPRICE4 525
#define ZD_SALEPRICE5 526
#define ZD_SALENUMBER2 527
#define ZD_SALENUMBER3 528
#define ZD_SALENUMBER4 529
#define ZD_SALENUMBER5 530
#define ZD_MARKETDOT 531
#define ZD_MARKETUNIT 532
#define ZD_HIDEBUYPRICE 533
#define ZD_HIDEBUYNUMBER 534
#define ZD_HIDESALEPRICE 535
#define ZD_HIDESALENUMBER 536
#define ZD_MARKETTIME 537

//通讯类请求标志
#define ZD_Logon "A"
#define ZD_Logout "5"
#define ZD_LogonBack "A1"
#define ZD_Order "O"
#define ZD_OrderBack "O1"
#define ZD_OrderAccount "O2"
#define ZD_AccountSearch "AC"
#define ZD_AccountSearchBack "AC1"
#define ZD_OrderCancel "C"
#define ZD_CancelBack "C1"
#define ZD_OpenSearch "OS"
#define ZD_OpenSearchBack "OS1"
#define ZD_FillSearch "FS"
#define ZD_FillSearchBack "FS1"
#define ZD_Modify "M"
#define ZD_ModifyBack "M1"
#define ZD_OrderSearch "ORS"
#define ZD_OrderSearchBack "ORS1"
#define ZD_Market "MA"
#define ZD_MarketBack "MA1"
//#define ZD_ErrorInfo "E"
#define ZD_SysNo "SY"
#define ZD_FilledCast "O3"
#define ZD_GetUseControl "HY"
#define ZD_ExchangeSearch "ES"
#define ZD_ExchangeSearchBack "ES1"
#define ZD_CurrencySearch "CS"
#define ZD_CurrencySearchBack "CS1"
#define ZD_OpenSearchY "OSY"
#define ZD_OpenSearchYBack "OSY1"
#define ZD_OpenDetail "OD"
#define ZD_OpenDetailBack "OD1"
#define ZD_StopWinList "SWL"
#define ZD_StopWinListBack "SWL1"
#define ZD_SetYingSun "SYS"
#define ZD_SetYingSunBack "SYS1"
#define ZD_DelYingSun "DYS"
#define ZD_DelYingSunBack "DYS1"
#define ZD_MarketFive "MA2"
#define ZD_MarketNew "MA3"
#define ZD_MarketPr "MA4"
#define ZD_HeatBit "HB"
#define ZD_OrderStatus "OST"

//错误代码定义
#define NoErr "00000"
#define ServerDisconnected "00001"

//SpreadFormulaType是一个价差公式类型
#define MINUS '1' //减
#define DIVIDED '2' //除
typedef char SpreadFormulaType;

#define SP_NOTTOUCH '1' //未触发
#define SP_TOUCH '2' //已触发
#define SP_ACT_ORDER '3' //主动腿已报单
#define SP_ACT_FILL '4' //主动腿成交
#define SP_PAS_ORDER '5' //被动腿已报单
#define SP_PAS_FILL '6' //被动腿成交
#define SP_ALL_FILL '7' //全部成交
#define SP_CANCEL '8' //已撤单
typedef char SpreadOrderStatus;

#define SUBMIT '1'//已提交
#define QUEUEING '2'//排队中
#define PARTFILL '3'//部分成交
#define ALLFILL '4'//全部成交
typedef char SingleOrderStatus;

#define SendOrder '1' //报单
#define CancelOrder '2' //撤单
typedef char OrderActionType;//报单操作类型

#define CTP_CONNECTED '1'
#define CTP_DISCONNECTED '2'
typedef char ConnectStatus;//连接状态

#define CTP_LOGIN '1'
#define CTP_LOGOUT '2'
typedef char LoginStatus;//登录状态
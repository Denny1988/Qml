#ifndef VIEWCOLUMNSCONSTANTS_H
#define VIEWCOLUMNSCONSTANTS_H

#include <QString>
#include "traderbusiness_global.h"

namespace Common 
{
	namespace Constants
	{
		class TRADERBUSINESS_EXPORT InstrumentListConstants
		{
		public:
			static const QString ID;
			static const QString CODE;
			static const QString TIMESTAMP;
			static const QString BID;
			static const QString ASK;
			static const QString FASTTRADE;
			static const QString SINGLEPRICE;
			static const QString HIGHT;
			static const QString LOW;
			static const QString OPEN;
			static const QString LAST;
			static const QString PCLOSE;
			static const QString VOLUME;
			static const QString TOTALVOLUME;
			static const QString CHANGE;
			static const QString BUYRATE;
			static const QString SELLRATE;
			static const QString FIFO;
			static const QString LOT;
			static const QString MOVEPIPS;
		};

		class TRADERBUSINESS_EXPORT AccountListConstants
		{
		public:
			static const QString CHECK;
			static const QString ACCOUNTCODE;
			static const QString ACCOUNTNAME;
			static const QString SALESCODE;
			static const QString CURRENCY;
			static const QString BALANCE;
			static const QString NECESSARY;
			static const QString PARTIALPAYMENTPHYSICALNECESSARY;
			static const QString PLEDGEAMOUNT;
			static const QString FLOATING;
			static const QString ESTIMATECLOSECOMMISSION;
			static const QString ESTIMATECLOSELEVY;
			static const QString EQUITY;
			static const QString NETASSET;
			static const QString USABLE;
			static const QString RATIO;
			static const QString UNREALISEDSWAP;
			static const QString UNREALISEDPL;
			static const QString UNCLEARAMOUNT;
			static const QString CREDITAMOUNT;
			static const QString FROZENFUND;
			static const QString TOTALPAIDAMOUNT;
			static const QString TOTALUNPAIDAMOUNT;
			static const QString REMARKS;
		};

		class TRADERBUSINESS_EXPORT AccountListItemConstants
		{
		public:
			static const QString CURRENCY;
			static const QString BALANCE;
			static const QString NECESSARY;
			static const QString PARTIALPAYMENTPHYSICALNECESSARY;
			static const QString FLOATING;
			static const QString ESTIMATECLOSECOMMISSION;
			static const QString ESTIMATECLOSELEVY;
			static const QString UNREALISEDSWAP;
			static const QString UNREALISEDPL;
			static const QString UNCLEARAMOUNT;
			static const QString EQUITY;
			static const QString NETASSET;
			static const QString USABLE;
			static const QString PLEDGEAMOUNT;
			static const QString FROZENFUND;
			static const QString TOTALPAIDAMOUNT;
			static const QString TOTALUNPAIDAMOUNT;
		};

		class TRADERBUSINESS_EXPORT OpenOrderConstants
		{
		public:
			static const QString LMTSTOP;
			static const QString LIQUIDATION;
			static const QString OFFICIALFIXINGDATE;
			static const QString CODE;
			static const QString ACCOUNT;
			static const QString INSTRUMENT;
			static const QString EXECUTETIME;
			static const QString LOT;
			static const QString BS;
			static const QString PRICE;
			static const QString LIMITORDERS;
			static const QString STOPORDERS;
			static const QString REFERENCEPRICE;
			static const QString AUTOLIMITPRICE;
			static const QString AUTOSTOPPRICE;
			static const QString FLOATING;
			static const QString UNREALIZEDSWAP;
			static const QString INTEREST;
			static const QString STORAGE;
			static const QString COMMISSION;
			static const QString REBATE;
			static const QString INTERESTRATE;
			static const QString CURRENCYRATE;

		};

		class TRADERBUSINESS_EXPORT PendingCloseOrderItemConstants
		{
		public:
			static const QString PHASE;
			static const QString SUBMITTIME;
			static const QString EXPIRETIME;
			static const QString ACCOUNT;
			static const QString INSTRUMENT;
			static const QString CLOSELOT;
			static const QString LOT;
			static const QString NC;
			static const QString BS;
			static const QString PRICE;
			static const QString TYPE;

		};

		class TRADERBUSINESS_EXPORT WorkingOrderConstants
		{
		public:
			static const QString ID;
			static const QString STATUS;
			static const QString SUBMITTIME;
			static const QString EXPIRETIME;
			static const QString ACCOUNT;
			static const QString INSTRUMENT;
			static const QString LOT;
			static const QString NC;
			static const QString BS;
			static const QString PRICE;
			static const QString MPS;
			static const QString EXECUTEDPRICE;
			static const QString STOPPRICE;
			static const QString TYPE;
			static const QString TRADEPL;
			static const QString FEE;
			static const QString REBATE;
			static const QString LEVY;
			static const QString REMARKS;

		};

		class TRADERBUSINESS_EXPORT BinaryOptionConstants
		{
		public:
			static const QString ID;
			static const QString STATUS;
			static const QString CODE;
			static const QString BOBEGINTIME;
			static const QString BOENDTIME;
			static const QString ACCOUNT;
			static const QString INSTRUMENT;
			static const QString BOBETAMOUNT;
			static const QString BOBETOPTION;
			static const QString EXECUTEPRICE;
			static const QString HITPRICE;
			static const QString BOODDS;
			static const QString TRADEPL;
			static const QString ORDERCOMMISSIONSUM;
			static const QString BOHITCOUNT;
			static const QString BOFREQUENCY;
			static const QString BOCOUNTDOWN;
			static const QString REMARKS;
		};
				
		class TRADERBUSINESS_EXPORT InstrumentSummaryConstants
		{
		public:
			static const QString INSTRUMENT;
			static const QString BUY;
			static const QString AVERAGEBUYPRICE;
			static const QString SELL;
			static const QString AVERAGESELLPRICE;
			static const QString NET;
		};

		class TRADERBUSINESS_EXPORT InstrumentSummaryItemConstants
		{
		public:
			static const QString ACCOUNT;
			static const QString BUY;
			static const QString AVERAGEBUYPRICE;
			static const QString SELL;
			static const QString AVERAGESELLPRICE;
			static const QString NET;

		};

		class TRADERBUSINESS_EXPORT InventoryConstants
		{
		public:
			static const QString DELIVERY;
			static const QString CLOSE;
			static const QString INSTRUMENT;
			static const QString ACCOUNT;
			static const QString WEIGHT;
			static const QString UNIT;
			static const QString CURRENCY;
			static const QString AVERAGEPRICE;
			static const QString REFERENCEPRICE;
			static const QString MARKETVALUE;
		};
		
		class TRADERBUSINESS_EXPORT InventoryItemConstants
		{
		public:
			static const QString ID;
			static const QString DELIVERY;
			static const QString CLOSE;
			static const QString INSTALMENT;
			static const QString PREPAYMENT;
			static const QString CODE;
			static const QString TIME;
			static const QString WEIGHT;
			static const QString PRICE;
			static const QString REFERENCEPRICE;
			static const QString MARKETVALUE;
			static const QString PAIDPLEDGE;
			static const QString FLOATING;
			static const QString REMAINAMOUNT;
			static const QString PLEDGEVALUE;
			static const QString PAYMENTDISCOUNT;
		};

		class TRADERBUSINESS_EXPORT PendingInventoryConstants
		{
		public:
			static const QString ID;
			static const QString CODE;
			static const QString ACCOUNT;
			static const QString INSTRUMENT;
			static const QString SUBMITTIME;
			static const QString WEIGHT;
			static const QString UNIT;
			static const QString STATUS;
			static const QString SETTLEMENTTIME;
		};

		class TRADERBUSINESS_EXPORT ShortSellConstants
		{
		public:
			static const QString ID;
			static const QString CLOSE;
			static const QString ACCOUNT;
			static const QString INSTRUMENT;
			static const QString SHORTSELLDATE;
			static const QString WEIGHT;
			static const QString UNIT;
			static const QString CURRENCY;
			static const QString SHORTSELLPRICE;
			static const QString MARKETPRICE;
			static const QString FLOATING;
			static const QString PAIDPLEDGE;
		};


		class TRADERBUSINESS_EXPORT UnConfirmPendingOrderConstants
		{
		public:
			static const QString ID;
			static const QString STATUS;
			static const QString SUBMITTIME;
			static const QString EXPIRETIME;
			static const QString ACCOUNT;
			static const QString INSTRUMENT;
			static const QString LOT;
			static const QString NC;
			static const QString BS;
			static const QString PRICE;
			static const QString STOPPRICE;
			static const QString TYPE;
			static const QString TRADEPL;
			static const QString FEE;
			static const QString LEVY;
		};

		class TRADERBUSINESS_EXPORT OrderQueryConstants
		{
		public:
			static const QString ID;
			static const QString CODE;
			static const QString ACCOUNT;
			static const QString SUBMITTIME;
			static const QString EXPIRETIME;
			static const QString EXECUTE_CANCEL_TIME;
			static const QString INSTRUMENT;
			static const QString TYPE;
			static const QString LOT;
			static const QString PRICE;
			static const QString NC;
			static const QString BS;
			static const QString BOBETOPTION;
			static const QString PHASE;
			static const QString TRADEPL;
			static const QString REMARKS;
		};

		class TRADERBUSINESS_EXPORT NewsOrChatConstants
		{
		public:
			static const QString ID;
			static const QString PUBLISHTIME;
			static const QString TITLE;
		};

		class TRADERBUSINESS_EXPORT PriceAlertConstants
		{
		public:
			static const QString ID;
			static const QString MODIFY;
			static const QString DELETEBUTTON;
			static const QString INSTRUMENT;
			static const QString CONDITION;
			static const QString PRICE;
			static const QString EXPIRATIONTIME;
			static const QString COLOR;
		};

		class TRADERBUSINESS_EXPORT LogConstants
		{
		public:
			static const QString ID;
			static const QString TIME;
			static const QString ACTION;
		};

	}
}
#endif


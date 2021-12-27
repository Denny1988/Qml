#include "Common/Constants/ViewColumnsConstants.h"
namespace Common 
{
	namespace Constants
	{
		//Instrument List Header
		const QString  InstrumentListConstants::ID = "ID";
		const QString  InstrumentListConstants::CODE = "CODE";
		const QString  InstrumentListConstants::TIMESTAMP = "TIMESTAMP";
		const QString  InstrumentListConstants::BID = "BID";
		const QString  InstrumentListConstants::ASK = "ASK";
		const QString  InstrumentListConstants::FASTTRADE = "FASTTRADE";
		const QString  InstrumentListConstants::SINGLEPRICE = "SINGLEPRICE";
		const QString  InstrumentListConstants::HIGHT = "HIGHT";
		const QString  InstrumentListConstants::LOW = "LOW";
		const QString  InstrumentListConstants::OPEN = "OPEN";
		const QString  InstrumentListConstants::LAST = "LAST";
		const QString  InstrumentListConstants::PCLOSE = "PCLOSE";
		const QString  InstrumentListConstants::VOLUME = "VOLUME";
		const QString  InstrumentListConstants::TOTALVOLUME = "TOTALVOLUME";
		const QString  InstrumentListConstants::CHANGE = "CHANGE";
		const QString  InstrumentListConstants::BUYRATE = "BUYRATE";
		const QString  InstrumentListConstants::SELLRATE = "SELLRATE";
		const QString  InstrumentListConstants::FIFO = "FIFO";
		const QString  InstrumentListConstants::LOT = "LOT";
		const QString  InstrumentListConstants::MOVEPIPS = "MOVEPIPS";
		
		//Account List  Header
		const QString  AccountListConstants::CHECK = "CHECK";
		const QString  AccountListConstants::ACCOUNTCODE = "ACCOUNTCODE";
		const QString  AccountListConstants::ACCOUNTNAME = "ACCOUNTNAME";
		const QString  AccountListConstants::SALESCODE = "SALESCODE";
		const QString  AccountListConstants::CURRENCY = "CURRENCY";
		const QString  AccountListConstants::BALANCE = "BALANCE";
		const QString  AccountListConstants::NECESSARY = "NECESSARY";
		const QString  AccountListConstants::PARTIALPAYMENTPHYSICALNECESSARY = "PARTIALPAYMENTPHYSICALNECESSARY";
		const QString  AccountListConstants::PLEDGEAMOUNT = "PLEDGEAMOUNT";
		const QString  AccountListConstants::FLOATING = "FLOATING";
		const QString  AccountListConstants::ESTIMATECLOSECOMMISSION = "ESTIMATECLOSECOMMISSION";
		const QString  AccountListConstants::ESTIMATECLOSELEVY = "ESTIMATECLOSELEVY";
		const QString  AccountListConstants::EQUITY = "EQUITY";
		const QString  AccountListConstants::NETASSET = "NETASSET";
		const QString  AccountListConstants::USABLE = "USABLE";
		const QString  AccountListConstants::RATIO = "RATIO";
		const QString  AccountListConstants::UNREALISEDSWAP = "UNREALISEDSWAP";
		const QString  AccountListConstants::UNREALISEDPL = "UNREALISEDPL";
		const QString  AccountListConstants::UNCLEARAMOUNT = "UNCLEARAMOUNT";
		const QString  AccountListConstants::CREDITAMOUNT = "CREDITAMOUNT";
		const QString  AccountListConstants::FROZENFUND = "FROZENFUND";
		const QString  AccountListConstants::TOTALPAIDAMOUNT = "TOTALPAIDAMOUNT";
		const QString  AccountListConstants::TOTALUNPAIDAMOUNT = "TOTALUNPAIDAMOUNT";
		const QString  AccountListConstants::REMARKS = "REMARKS";

		////Account List Item   Header
		const QString  AccountListItemConstants::CURRENCY = "CURRENCY";
		const QString  AccountListItemConstants::BALANCE = "BALANCE";
		const QString  AccountListItemConstants::NECESSARY = "NECESSARY";
		const QString  AccountListItemConstants::PARTIALPAYMENTPHYSICALNECESSARY = "PARTIALPAYMENTPHYSICALNECESSARY";
		const QString  AccountListItemConstants::FLOATING = "FLOATING";
		const QString  AccountListItemConstants::ESTIMATECLOSECOMMISSION = "ESTIMATECLOSECOMMISSION";
		const QString  AccountListItemConstants::ESTIMATECLOSELEVY = "ESTIMATECLOSELEVY";
		const QString  AccountListItemConstants::UNREALISEDSWAP = "UNREALISEDSWAP";
		const QString  AccountListItemConstants::UNREALISEDPL = "UNREALISEDPL";
		const QString  AccountListItemConstants::UNCLEARAMOUNT = "UNCLEARAMOUNT";
		const QString  AccountListItemConstants::EQUITY = "EQUITY";
		const QString  AccountListItemConstants::NETASSET = "NETASSET";
		const QString  AccountListItemConstants::USABLE = "USABLE";
		const QString  AccountListItemConstants::PLEDGEAMOUNT = "PLEDGEAMOUNT";
		const QString  AccountListItemConstants::FROZENFUND = "FROZENFUND";
		const QString  AccountListItemConstants::TOTALPAIDAMOUNT = "TOTALPAIDAMOUNT";
		const QString  AccountListItemConstants::TOTALUNPAIDAMOUNT = "TOTALUNPAIDAMOUNT";


		//Open Order Header
		const QString  OpenOrderConstants::LMTSTOP = "LMTSTOP";
		const QString  OpenOrderConstants::LIQUIDATION = "LIQUIDATION";
		const QString  OpenOrderConstants::OFFICIALFIXINGDATE = "OFFICIALFIXINGDATE";
		const QString  OpenOrderConstants::CODE = "CODE";
		const QString  OpenOrderConstants::ACCOUNT = "ACCOUNT";
		const QString  OpenOrderConstants::INSTRUMENT = "INSTRUMENT";
		const QString  OpenOrderConstants::EXECUTETIME = "EXECUTETIME";
		const QString  OpenOrderConstants::LOT = "LOT";
		const QString  OpenOrderConstants::BS = "BS";
		const QString  OpenOrderConstants::PRICE = "PRICE";
		const QString  OpenOrderConstants::LIMITORDERS = "LIMITORDERS";
		const QString  OpenOrderConstants::STOPORDERS = "STOPORDERS";
		const QString  OpenOrderConstants::REFERENCEPRICE = "REFERENCEPRICE";
		const QString  OpenOrderConstants::AUTOLIMITPRICE = "AUTOLIMITPRICE";
		const QString  OpenOrderConstants::AUTOSTOPPRICE = "AUTOSTOPPRICE";
		const QString  OpenOrderConstants::FLOATING = "FLOATING";
		const QString  OpenOrderConstants::UNREALIZEDSWAP = "UNREALIZEDSWAP";
		const QString  OpenOrderConstants::INTEREST = "INTEREST";
		const QString  OpenOrderConstants::STORAGE = "STORAGE";
		const QString  OpenOrderConstants::COMMISSION = "COMMISSION";
		const QString  OpenOrderConstants::REBATE = "REBATE";
		const QString  OpenOrderConstants::INTERESTRATE = "INTERESTRATE";
		const QString  OpenOrderConstants::CURRENCYRATE = "CURRENCYRATE";

		//Open Order Item Header
		const QString  PendingCloseOrderItemConstants::PHASE = "PHASE";
		const QString  PendingCloseOrderItemConstants::SUBMITTIME = "SUBMITTIME";
		const QString  PendingCloseOrderItemConstants::EXPIRETIME = "EXPIRETIME";
		const QString  PendingCloseOrderItemConstants::ACCOUNT = "ACCOUNT";
		const QString  PendingCloseOrderItemConstants::INSTRUMENT = "INSTRUMENT";
		const QString  PendingCloseOrderItemConstants::CLOSELOT = "CLOSELOT";
		const QString  PendingCloseOrderItemConstants::LOT = "LOT";
		const QString  PendingCloseOrderItemConstants::NC = "NC";
		const QString  PendingCloseOrderItemConstants::BS = "BS";
		const QString  PendingCloseOrderItemConstants::PRICE = "PRICE";
		const QString  PendingCloseOrderItemConstants::TYPE = "TYPE";

		////Working Order Header
		const QString  WorkingOrderConstants::ID = "ID";
		const QString  WorkingOrderConstants::STATUS = "STATUS";
		const QString  WorkingOrderConstants::SUBMITTIME = "SUBMITTIME";
		const QString  WorkingOrderConstants::EXPIRETIME = "EXPIRETIME";
		const QString  WorkingOrderConstants::ACCOUNT = "ACCOUNT";
		const QString  WorkingOrderConstants::INSTRUMENT = "INSTRUMENT";
		const QString  WorkingOrderConstants::LOT = "LOT";
		const QString  WorkingOrderConstants::NC = "NC";
		const QString  WorkingOrderConstants::BS = "BS";
		const QString  WorkingOrderConstants::PRICE = "PRICE";
		const QString  WorkingOrderConstants::MPS = "MPS";
		const QString  WorkingOrderConstants::EXECUTEDPRICE = "EXECUTEDPRICE";
		const QString  WorkingOrderConstants::STOPPRICE = "STOPPRICE";
		const QString  WorkingOrderConstants::TYPE = "TYPE";
		const QString  WorkingOrderConstants::TRADEPL = "TRADEPL";
		const QString  WorkingOrderConstants::FEE = "FEE";
		const QString  WorkingOrderConstants::REBATE = "REBATE";
		const QString  WorkingOrderConstants::LEVY = "LEVY";
		const QString  WorkingOrderConstants::REMARKS = "REMARKS";

		////BO Order Header
		const QString  BinaryOptionConstants::ID = "ID";
		const QString  BinaryOptionConstants::STATUS = "STATUS";
		const QString  BinaryOptionConstants::CODE = "CODE";
		const QString  BinaryOptionConstants::BOBEGINTIME = "BOBEGINTIME";
		const QString  BinaryOptionConstants::BOENDTIME = "BOENDTIME";
		const QString  BinaryOptionConstants::ACCOUNT = "ACCOUNT";
		const QString  BinaryOptionConstants::INSTRUMENT = "INSTRUMENT";
		const QString  BinaryOptionConstants::BOBETAMOUNT = "BOBETAMOUNT";
		const QString  BinaryOptionConstants::BOBETOPTION = "BOBETOPTION";
		const QString  BinaryOptionConstants::EXECUTEPRICE = "EXECUTEPRICE";
		const QString  BinaryOptionConstants::HITPRICE = "HITPRICE";
		const QString  BinaryOptionConstants::BOODDS = "BOODDS";
		const QString  BinaryOptionConstants::TRADEPL = "TRADEPL";
		const QString  BinaryOptionConstants::ORDERCOMMISSIONSUM = "ORDERCOMMISSIONSUM";
		const QString  BinaryOptionConstants::BOHITCOUNT = "BOHITCOUNT";
		const QString  BinaryOptionConstants::BOFREQUENCY = "BOFREQUENCY";
		const QString  BinaryOptionConstants::BOCOUNTDOWN = "BOCOUNTDOWN";
		const QString  BinaryOptionConstants::REMARKS = "REMARKS";


		////Instrument Summary Header
		const QString  InstrumentSummaryConstants::INSTRUMENT = "INSTRUMENT";
		const QString  InstrumentSummaryConstants::BUY = "BUY";
		const QString  InstrumentSummaryConstants::AVERAGEBUYPRICE = "AVERAGEBUYPRICE";
		const QString  InstrumentSummaryConstants::SELL = "SELL";
		const QString  InstrumentSummaryConstants::AVERAGESELLPRICE = "AVERAGESELLPRICE";
		const QString  InstrumentSummaryConstants::NET = "NET";
		
		////Instrument Summary Item Header
		const QString  InstrumentSummaryItemConstants::ACCOUNT = "ACCOUNT";
		const QString  InstrumentSummaryItemConstants::BUY = "BUY";
		const QString  InstrumentSummaryItemConstants::AVERAGEBUYPRICE = "AVERAGEBUYPRICE";
		const QString  InstrumentSummaryItemConstants::SELL = "SELL";
		const QString  InstrumentSummaryItemConstants::AVERAGESELLPRICE = "AVERAGESELLPRICE";
		const QString  InstrumentSummaryItemConstants::NET = "NET";
		
		//Inventory Header
		const QString  InventoryConstants::DELIVERY = "DELIVERY";
		const QString  InventoryConstants::CLOSE = "CLOSE";
		const QString  InventoryConstants::INSTRUMENT = "INSTRUMENT";
		const QString  InventoryConstants::ACCOUNT = "ACCOUNT";
		const QString  InventoryConstants::WEIGHT = "WEIGHT";
		const QString  InventoryConstants::UNIT = "UNIT";
		const QString  InventoryConstants::CURRENCY = "CURRENCY";
		const QString  InventoryConstants::AVERAGEPRICE = "AVERAGEPRICE";
		const QString  InventoryConstants::REFERENCEPRICE = "REFERENCEPRICE";
		const QString  InventoryConstants::MARKETVALUE = "MARKETVALUE";

		//Inventory Item Header
		const QString  InventoryItemConstants::ID = "ID";
		const QString  InventoryItemConstants::DELIVERY = "DELIVERY";
		const QString  InventoryItemConstants::CLOSE = "CLOSE";
		const QString  InventoryItemConstants::INSTALMENT = "INSTALMENT";
		const QString  InventoryItemConstants::PREPAYMENT = "PREPAYMENT";
		const QString  InventoryItemConstants::CODE = "CODE";
		const QString  InventoryItemConstants::TIME = "TIME";
		const QString  InventoryItemConstants::WEIGHT = "WEIGHT";
		const QString  InventoryItemConstants::PRICE = "PRICE";
		const QString  InventoryItemConstants::REFERENCEPRICE = "REFERENCEPRICE";
		const QString  InventoryItemConstants::MARKETVALUE = "MARKETVALUE";
		const QString  InventoryItemConstants::PAIDPLEDGE = "PAIDPLEDGE";
		const QString  InventoryItemConstants::FLOATING = "FLOATING";
		const QString  InventoryItemConstants::REMAINAMOUNT = "REMAINAMOUNT";
		const QString  InventoryItemConstants::PLEDGEVALUE = "PLEDGEVALUE";
		const QString  InventoryItemConstants::PAYMENTDISCOUNT = "PAYMENTDISCOUNT";

		////Pending Inventory Header
		const QString  PendingInventoryConstants::ID = "ID";
		const QString  PendingInventoryConstants::CODE = "CODE";
		
		const QString  PendingInventoryConstants::ACCOUNT = "ACCOUNT";
		const QString  PendingInventoryConstants::INSTRUMENT = "INSTRUMENT";
		const QString  PendingInventoryConstants::SUBMITTIME = "SUBMITTIME";
		const QString  PendingInventoryConstants::WEIGHT = "WEIGHT";
		const QString  PendingInventoryConstants::UNIT = "UNIT";
		const QString  PendingInventoryConstants::STATUS = "STATUS";
		const QString  PendingInventoryConstants::SETTLEMENTTIME = "SETTLEMENTTIME";

		//Short Sell Header
		const QString  ShortSellConstants::ID = "ID";
		const QString  ShortSellConstants::CLOSE = "CLOSE";
		const QString  ShortSellConstants::ACCOUNT = "ACCOUNT";
		const QString  ShortSellConstants::INSTRUMENT = "INSTRUMENT";
		const QString  ShortSellConstants::SHORTSELLDATE = "SHORTSELLDATE";
		const QString  ShortSellConstants::WEIGHT = "WEIGHT";
		const QString  ShortSellConstants::UNIT = "UNIT";
		const QString  ShortSellConstants::CURRENCY = "CURRENCY";
		const QString  ShortSellConstants::SHORTSELLPRICE = "SHORTSELLPRICE";
		const QString  ShortSellConstants::MARKETPRICE = "MARKETPRICE";
		const QString  ShortSellConstants::FLOATING = "FLOATING";
		const QString  ShortSellConstants::PAIDPLEDGE = "PAIDPLEDGE";
		
		////UnConfirm PendingOrder Header
		const QString  UnConfirmPendingOrderConstants::ID = "ID";
		const QString  UnConfirmPendingOrderConstants::STATUS = "STATUS";
		const QString  UnConfirmPendingOrderConstants::SUBMITTIME = "SUBMITTIME";
		const QString  UnConfirmPendingOrderConstants::EXPIRETIME = "EXPIRETIME";
		const QString  UnConfirmPendingOrderConstants::ACCOUNT = "ACCOUNT";
		const QString  UnConfirmPendingOrderConstants::INSTRUMENT = "INSTRUMENT";
		const QString  UnConfirmPendingOrderConstants::LOT = "LOT";
		const QString  UnConfirmPendingOrderConstants::NC = "NC";
		const QString  UnConfirmPendingOrderConstants::BS = "BS";
		const QString  UnConfirmPendingOrderConstants::PRICE = "PRICE";
		const QString  UnConfirmPendingOrderConstants::STOPPRICE = "STOPPRICE";
		const QString  UnConfirmPendingOrderConstants::TYPE = "TYPE";
		const QString  UnConfirmPendingOrderConstants::TRADEPL = "TRADEPL";
		const QString  UnConfirmPendingOrderConstants::FEE = "FEE";
		const QString  UnConfirmPendingOrderConstants::LEVY = "LEVY";
		
		////Order Query Header
		const QString  OrderQueryConstants::ID = "ID";
		const QString  OrderQueryConstants::CODE = "CODE";
		const QString  OrderQueryConstants::ACCOUNT = "ACCOUNT";
		const QString  OrderQueryConstants::SUBMITTIME = "SUBMITTIME";
		const QString  OrderQueryConstants::EXPIRETIME = "EXPIRETIME";
		const QString  OrderQueryConstants::EXECUTE_CANCEL_TIME = "EXECUTE_CANCEL_TIME";
		const QString  OrderQueryConstants::INSTRUMENT = "INSTRUMENT";
		const QString  OrderQueryConstants::TYPE = "TYPE";
		const QString  OrderQueryConstants::LOT = "LOT";
		const QString  OrderQueryConstants::PRICE = "PRICE";
		const QString  OrderQueryConstants::NC = "NC";
		const QString  OrderQueryConstants::BS = "BS";
		const QString  OrderQueryConstants::BOBETOPTION = "BOBETOPTION";
		const QString  OrderQueryConstants::PHASE = "PHASE";
		const QString  OrderQueryConstants::TRADEPL = "TRADEPL";
		const QString  OrderQueryConstants::REMARKS = "REMARKS";

		////NewsOrChat Header
		const QString  NewsOrChatConstants::ID = "ID";
		const QString  NewsOrChatConstants::PUBLISHTIME = "PUBLISHTIME";
		const QString  NewsOrChatConstants::TITLE = "TITLE";
		
		//Price Alert Header
		const QString  PriceAlertConstants::ID = "ID";
		const QString  PriceAlertConstants::MODIFY = "MODIFY";
		const QString  PriceAlertConstants::DELETEBUTTON = "DELETE";
		const QString  PriceAlertConstants::INSTRUMENT = "INSTRUMENT";
		const QString  PriceAlertConstants::CONDITION = "CONDITION";
		const QString  PriceAlertConstants::PRICE = "PRICE";
		const QString  PriceAlertConstants::EXPIRATIONTIME = "EXPIRATIONTIME";
		const QString  PriceAlertConstants::COLOR = "COLOR";
		
		////Log Header
		const QString  LogConstants::ID = "ID";
		const QString  LogConstants::TIME = "TIME";
		const QString  LogConstants::ACTION = "ACTION";


	}
}
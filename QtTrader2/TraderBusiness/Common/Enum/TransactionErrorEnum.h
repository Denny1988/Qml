#ifndef TRANSACTIONERRORENUM_H
#define TRANSACTIONERRORENUM_H
namespace Common
{
	namespace Enum
	{
		namespace TransactionError
		{
			enum TransactionError
			{
				NoLinkedServer = -1,
				OK = 0,
				RuntimeError = 1,

				DbOperationFailed = 2,

				TransactionAlreadyExists = 3,
				HasNoOrders = 4,
				InvalidRelation = 5,
				InvalidLotBalance = 6,
				ExceedOpenLotBalance = 7,
				InvalidPrice = 8,

				AccountIsNotTrading = 9,
				AccountResetFailed = 10,
				InstrumentIsNotAccepting = 11,
				TimingIsNotAcceptable = 12,
				OrderTypeIsNotAcceptable = 13,
				HasUnassignedOvernightOrders = 14,
				HasNoAccountsLocked = 15,
				IsLockedByAgent = 16,
				IsNotLockedByAgent = 17,
				ExceedAssigningLotBalance = 18,

				LossExecutedOrderInOco = 19,
				OrderLotExceedMaxLot = 20,
				OpenOrderNotExists = 21,
				AssigningOrderNotExists = 22,
				TransactionNotExists = 23,
				TransactionCannotBeCanceled = 24,
				TransactionCannotBeExecuted = 25,
				OrderCannotBeDeleted = 26,
				NecessaryIsNotWithinThreshold = 27,
				MarginIsNotEnough = 28,
				IsNotAccountOwner = 29,
				InvalidOrderRelation = 30,
				TradePolicyIsNotActive = 31,
				SetPriceTooCloseToMarket = 32,
				HasNoQuotationExists = 33,
				AccountIsInAlerting = 34,
				DailyQuotationIsNotIntegrated = 35,
				LimitStopAddPositionNotAllowed = 36,
				MooMocNewPositionNotAllowed = 37,
				TransactionCannotBeBooked = 38,
				OnlySptMktIsAllowedForPreCheck = 39,
				InvalidTransactionPhase = 40,
				ExecuteTimeMustBeInTradingTime = 41,

				DatabaseDataIntegralityViolated = 50,

				PriceIsOutOfDate = 60,
				ShortSellNotAllowed = 61,
				InvalidInstalmentTrade = 62,
				PrepaymentIsNotAllowed = 63,
				HitIsReseted = 64,
				ExistPendingLimitCloseOrder = 65,

				//More accuracy error
				TransactionExpired = 100,
				FillOnMarketCloseNotAllowed = 101,
				InstrumentNotInTradePolicy = 102,
				AmendedOrderNotFound = 103,
				InitialOrderCanNotBeAmended = 104,
				InvalidResetStatusWhenAssign = 105,  //Check [P_AssignTran] 
				AlreadyValued = 106,
				OutOfAcceptDQVariation = 107,
				PriceIsDisabled = 108,
				TransactionStateViolated = 109,
				PriceChangedSincePlace = 110,
				ExceedMaxOpenLot = 111,
				ReplacedWithMaxLot = 112,
				ExceedMaxPhysicalValue = 113,
				BalanceOrEquityIsShort = 114,
				SplittedForHasShortSell = 115,
				AdjustedToFullPaidOrderForHasShortSell = 116,
				NotPaidoffInstalmentOrderCanNotBeClosed = 117,

				MultipleCloseOrderNotFound = 200,
				MultipleCloseOnlyExecutedOrderAllowed = 201,
				MultipleCloseOnlyOpenOrderAllowed = 202,
				MultipleCloseHasNoLotBalance = 203,
				MultipleCloseOnlySameContractSizeAllowed = 204,
				MultipleCloseOnlySameAccountAllowed = 205,
				MultipleCloseOnlySameInstrumentAllowed = 206,
				MultipleCloseOppositeNotFound = 207,
				MultipleCloseNotSortByCode = 208,
				MultipleCloseNotAllowed = 209,

				IsCuttingByRemoteSystem = 300,

				//Used to extend function. it is not conform to the code specification
				Action_ShouldAutoFill = 10000,
				Action_NeedDealerConfirmCanceling = 10001,

				RedoLimitOrder_PartialSuccess = 20001,
				RedoLimitOrder_AllFailed = 20002,
				AccountIsIntializing = 20003,
				AccountIsInReset = 20004,
				InterestRecalculateFailed = 20005,
				UpdateInstalmentFailed = 20006,
				ExceedHighLowPrice = 20007,
				ResetDataNotSyncToDB = 20008,
				OpenContractsWithDifferentSizes = 20009
			};
		}
	}
}
#endif
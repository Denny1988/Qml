#ifndef BUSINESS_MODEL_COMPUTING_CALCULATEORDER_H
#define BUSINESS_MODEL_COMPUTING_CALCULATEORDER_H
#include <quuid.h>
#include "DemandComputing.h"
#include "ComputingResult.h"
#include "Business/Model/Setting/BusinessAccount.h"
#include "Business/Model/Setting/CurrencyRate.h"
#include "Business/Model/Trading/TradingOrder.h"
#include "common/Enum/InstrumentEnum.h"

using namespace Common::Enum;
namespace Business
{
	namespace Model
	{
		namespace Computing
		{
			class ComputingOrder : public DemandComputing
			{
			public:
				QUuid _accountId;
				QUuid _instrumentId;
				QUuid _orderId;

			private:
				void calculateMargin(QSharedPointer<OrderComputingResult>& orderComputingResult,
					QSharedPointer<OverridedQuotationComputingResult>& overridedQuotationComputingResult,
					QSharedPointer<Model::Trading::TradingOrder>& tradingOrder,
					QSharedPointer<Model::Setting::BusinessInstrument>& settingInstrument,
					QSharedPointer<Model::Setting::BusinessAccount>& settingAccount);
				QDecNumber calculateMargin(int marginFormula, QDecNumber lotBalance, QDecNumber contractSize, QDecNumber price,
					QSharedPointer<Model::Setting::CurrencyRate>& currencyRate, int decimals);
				void calculatePLNotValued();
				void calculatePLFloat(QSharedPointer<OrderComputingResult>& orderComputingResult,
					QSharedPointer<OverridedQuotationComputingResult>& overridedQuotationComputingResult,
					QSharedPointer<Model::Trading::TradingOrder>& tradingOrder,
					QSharedPointer<Model::Setting::BusinessInstrument>& settingInstrument,
					QSharedPointer<Model::Setting::BusinessAccount>& settingAccount, bool isInit = false);
				QDecNumber calculateTradePL(int tradePLFormula, QDecNumber lotBalance, QDecNumber contractSize, QDecNumber buy, QDecNumber sell, QDecNumber close);
				void calculateMarketValue(QSharedPointer<OrderComputingResult>& orderComputingResult,
					QSharedPointer<OverridedQuotationComputingResult>& overridedQuotationComputingResult,
					QSharedPointer<Model::Trading::TradingOrder>& tradingOrder,
					QSharedPointer<Model::Setting::BusinessInstrument>& settingInstrument,
					QSharedPointer<Model::Setting::BusinessAccount>& settingAccount);
				void calculateCommission(QSharedPointer<OrderComputingResult>& orderComputingResult,
					QSharedPointer<Model::Trading::TradingOrder>& tradingOrder,
					QSharedPointer<Model::Setting::BusinessInstrument>& settingInstrument,
					QSharedPointer<Model::Setting::BusinessAccount>& settingAccount,
					FeeCalculateType::FeeCalculateType feeCalculateType, bool isInit = false);
				
			protected:
				QSharedPointer<DemandComputingResult> doCompute();

			public:
				void calculate();
			};
		}
	}
}
#endif

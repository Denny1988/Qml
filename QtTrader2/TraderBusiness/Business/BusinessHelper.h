#ifndef BUSINESS_BUSINESSHELPER_H
#define BUSINESS_BUSINESSHELPER_H

#include <QSharedPointer>
#include "QDecNumber.h"
#include "Common/Enum/PlaceEnum.h"
#include "Common/Enum/BOEnum.h"
#include "traderbusiness_global.h"

using namespace Common::Enum;

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class BusinessAccount;
			class BusinessInstrument;
		}

		namespace Trading
		{
			class TradingOrder;
		}
	}
}

namespace Business
{
	class SettingManager;
	class TRADERBUSINESS_EXPORT BusinessHelper
	{
	public:
		static BusinessHelper* getInstance();
		
		int getDecimals(QSharedPointer<Business::Model::Setting::BusinessAccount> account, QSharedPointer<Business::Model::Setting::BusinessInstrument> instrument);
		void setSettingManager(QSharedPointer<SettingManager> settingManager);
		QDecNumber calculateValue(int tradePLFormula, QDecNumber contractSize, QDecNumber lotInteger, QDecNumber lotRemainder, QDecNumber livePrice, QDecNumber discountOfOdd);
		bool canChangeToOCO(QSharedPointer<Business::Model::Trading::TradingOrder> order);
		bool canChangeToIfdone(QSharedPointer<Business::Model::Trading::TradingOrder> order);
		bool canModifySetPrice(QSharedPointer<Business::Model::Trading::TradingOrder> order);
		QDecNumber calculatePLNotValued(QSharedPointer<Business::Model::Setting::BusinessAccount> account,
			QSharedPointer<Business::Model::Setting::BusinessInstrument> instrument,
			QList<QDecNumber> interestPLNotValueds,
			QList<QDecNumber> storagePLNotValueds);
		QString getCurrencyRate(QSharedPointer<Business::Model::Setting::BusinessAccount> account,
			QSharedPointer<Business::Model::Setting::BusinessInstrument> instrument,
			QDecNumber tradePLFloat);
		bool getBOIncludePrincipal();
		QDateTime getSettleTime(QDateTime now, int seconds, BOBetOption::BOBetOption boOption, int lastOrderTimeSpan);

	private:
		BusinessHelper();

	private:
		static BusinessHelper* m_Instance;
		QSharedPointer<SettingManager> _settingManager;
	};
}

#endif


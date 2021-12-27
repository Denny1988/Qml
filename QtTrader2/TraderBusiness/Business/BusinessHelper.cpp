#include "BusinessHelper.h"
#include "SettingManager.h"
#include "Business/Model/Setting/BusinessAccount.h"
#include "Business/Model/Setting/BusinessInstrument.h"
#include "Business/Model/Trading/Transaction.h"
#include "Business/Model/Trading/TradingOrder.h"
#include "BusinessManager.h"

#include "qDebug.h"

namespace Business
{
	BusinessHelper* BusinessHelper:: m_Instance = Q_NULLPTR;

	BusinessHelper* BusinessHelper::getInstance()
	{
		if(!m_Instance)
		{
			m_Instance = new BusinessHelper;
		}
		return m_Instance;
	}

	BusinessHelper::BusinessHelper()
	{

	}

	void BusinessHelper::setSettingManager(QSharedPointer<SettingManager> settingManager)
	{
		this->_settingManager = settingManager;
	}

	int BusinessHelper::getDecimals(QSharedPointer<Business::Model::Setting::BusinessAccount> account, QSharedPointer<Business::Model::Setting::BusinessInstrument> instrument)
	{	
#ifdef RUNTIME_OS_MACX //add for debug EXC_BAD_ACCESS
        if(account.isNull() || instrument.isNull())
        {
            qDebug() << "ERROR: BusinessHelper::getDecimals Account is NULL";
            return 0;
        }
#endif
		if (!account->getIsMultiCurrency())
		{
			QSharedPointer<Model::Setting::Currency> currency = this->_settingManager->getCurrency(account->getCurrencyId());
			return currency->getDecimals();
		}
		else
		{
			QSharedPointer<Model::Setting::Currency> currency = this->_settingManager->getCurrency(instrument->getCurrencyId());
			return currency->getDecimals();
		}
	}

	QString BusinessHelper::getCurrencyRate(QSharedPointer<Business::Model::Setting::BusinessAccount> account,
		QSharedPointer<Business::Model::Setting::BusinessInstrument> instrument,
		QDecNumber tradePLFloat)
	{
		QUuid sourceCurrencyId = instrument->getCurrencyId();
		QUuid targetCurrencyId = account->getCurrencyId();
		auto currencyRate = this->_settingManager->getCurrencyRate(sourceCurrencyId, targetCurrencyId);
		QDecNumber currencyRateValue = tradePLFloat > 0.0 ? currencyRate->getRateIn() : currencyRate->getRateOut();
		if (currencyRateValue < 1.0 && currencyRateValue > 0.0) 
		{
			currencyRateValue = QDecNumber(1.0) / currencyRateValue;
		}
		return Common::roundToString2(currencyRateValue.toDouble(), 4);
	}

	QDecNumber BusinessHelper::calculatePLNotValued(QSharedPointer<Business::Model::Setting::BusinessAccount> account,
		QSharedPointer<Business::Model::Setting::BusinessInstrument> instrument,
		QList<QDecNumber> interestPLNotValueds,
		QList<QDecNumber> storagePLNotValueds)
	{
		QDecNumber interest = 0.0;
		QDecNumber storage = 0.0; 

		if (!account->getIsMultiCurrency())
		{
			QUuid sourceCurrencyId = instrument->getCurrencyId();
			QUuid targetCurrencyId = account->getCurrencyId();

			auto currencyRate = this->_settingManager->getCurrencyRate(sourceCurrencyId, targetCurrencyId);
			foreach (QDecNumber item, interestPLNotValueds)
			{
				interest += currencyRate->exchange(item);
			}
			foreach (QDecNumber item, interestPLNotValueds)
			{
				storage += currencyRate->exchange(item);
			}
		}
		else
		{
			foreach (QDecNumber item, interestPLNotValueds)
			{
				interest += item;
			}
			foreach (QDecNumber item, interestPLNotValueds)
			{
				storage += item;
			}
		}
		return interest+storage;
	}


	QDecNumber BusinessHelper::calculateValue(int tradePLFormula, QDecNumber contractSize, QDecNumber lotInteger, QDecNumber lotRemainder, QDecNumber livePrice, QDecNumber discountOfOdd)
	{
//        qDebug()<< " tradePLFormula:" << tradePLFormula << " lotInteger:" << lotInteger.toDouble()
//                << " contractSize:" << contractSize.toDouble() << " livePrice:" << livePrice.toDouble()
//                << " lotRemainder:" << lotRemainder.toDouble() << " discountOfOdd:" << discountOfOdd.toDouble();
		QDecNumber value = 0;
		QDecNumber divisor = 1;
		switch (tradePLFormula)
		{
		case 0:
			value = lotInteger * contractSize * livePrice
				+ lotRemainder * contractSize * livePrice * discountOfOdd;            
			break;
		case 1:
		case 3:
			value = lotInteger * contractSize
				+ lotRemainder * contractSize * discountOfOdd;
			break;
		case 2:
			livePrice = divisor / livePrice;
			value = lotInteger * contractSize * livePrice
				+ lotRemainder * contractSize * livePrice * discountOfOdd;
			break;
		}
		return value;
	}


	bool BusinessHelper::canChangeToOCO(QSharedPointer<Business::Model::Trading::TradingOrder> order)
	{
		auto placeSettingDetail = order->getTransaction()->getBusinessInstrument()->getPlaceSetting()->getPlaceSettingDetail(OrderType::Limit);  
		auto transaction = order->getTransaction();
		if (!order->getIsOpen())
			return (order->getPhase() == Phase::Placing || order->getPhase() == Phase::Placed)
			&& (transaction->getOrderType() == OrderType::Limit || transaction->getOrderType() == OrderType::Stop)
			&& placeSettingDetail->getAllowTrade()
			&& placeSettingDetail->getAllowOneCancelOther();

		else
		{
			return (order->getPhase() == Phase::Placing || order->getPhase() == Phase::Placed)
				&& (transaction->getOrderType() == OrderType::Limit || transaction->getOrderType() == OrderType::Stop)
				&& placeSettingDetail->getAllowTrade()
				&& placeSettingDetail->getAllowNewOCO();
		}
	}

	bool BusinessHelper::canChangeToIfdone(QSharedPointer<Business::Model::Trading::TradingOrder> order)
	{
		auto placeSettingDetail = order->getTransaction()->getBusinessInstrument()->getPlaceSetting()->getPlaceSettingDetail(OrderType::Limit);  
		auto transaction = order->getTransaction();
		if (!order->getIsOpen())
			return false;
		else
		{
			return (order->getPhase() == Phase::Placing || order->getPhase() == Phase::Placed)
				&& (transaction->getOrderType() == OrderType::Limit || transaction->getOrderType() == OrderType::Stop)
				&& placeSettingDetail->getAllowTrade()
				&& placeSettingDetail->getAllowIfDone();
		}
	}

	bool BusinessHelper::canModifySetPrice(QSharedPointer<Business::Model::Trading::TradingOrder> order)
	{
		auto placeSettingDetail = order->getTransaction()->getBusinessInstrument()->getPlaceSetting()->getPlaceSettingDetail(OrderType::Limit); 
		return order->getPhase() == Phase::Placed       
			&& placeSettingDetail->getAllowChangePlacedOrder()
			&& (order->getTransaction()->getOrderType() == OrderType::Limit || order->getTransaction()->getOrderType() == OrderType::Stop) 
			&& order->getPhase() != Phase::Placing
			&& placeSettingDetail->getAllowTrade();
	}
	

	bool BusinessHelper::getBOIncludePrincipal()
	{
		return this->_settingManager->getSystemSetting()->getBOIncludePrincipal();
	}

	QDateTime BusinessHelper::getSettleTime(QDateTime now, int seconds, BOBetOption::BOBetOption boOption, int lastOrderTimeSpan)
	{            
		if (boOption == BOBetOption::Instance)
		{
			return now.addSecs(seconds);
		}
		else 
		{
			int year = now.date().year();
			int month = now.date().month();
			int day = now.date().day();
			int hour = now.time().hour();
			int minute = now.time().minute();
			if (boOption == BOBetOption::IntegralMinute)
			{
				QDateTime result(QDate(year, month, day), QTime(hour, minute, 0));
				result = result.addSecs((seconds + 1)*60);       //frequency is minute in option 1,2
				return result;
			}
			else
			{
				QDateTime settleTime(QDate(year, month, day), QTime(hour, seconds, 0));          //frequency is minute in option 1,2
				if (settleTime.addSecs(-lastOrderTimeSpan) <= now)
				{
					settleTime = settleTime.addSecs(60*60);
				}
				return settleTime;
			}
		}
	}
}

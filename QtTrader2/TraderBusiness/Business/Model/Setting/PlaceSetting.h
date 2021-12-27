#ifndef BUSINESS_MODEL_SETTING_PLACESETTING_H
#define BUSINESS_MODEL_SETTING_PLACESETTING_H
#include "PlaceSettingDetail.h"
#include <QMap>
#include <QSharedPointer>
#include <QDomNode>
#include "SettingDataBase.h"
#include "traderbusiness_global.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class TRADERBUSINESS_EXPORT PlaceSetting: public SettingDataBase
			{
			public:
				PlaceSetting();
				~PlaceSetting();
			private:
				bool _isAutoClose;				
				bool _allowNewOpenBuy;
				bool _allowNewOpenSell;
				int _priceValidTime;
				int _quoteTimeout;
				bool _allowFullPayment;
				bool _allowAdvancePayment;
				bool _allowInstalment;
				bool _allowDelivery;
				bool _allowShortSell;
				bool _closingUseCustomerQuotePolicy;
				QDecNumber	_contractSize;

				QMap<Common::Enum::OrderType::OrderType, QSharedPointer<PlaceSettingDetail>> _placeSettingDetails;

			public:
				QSharedPointer<PlaceSettingDetail>& getOrAddPlaceSettingDetail(Common::Enum::OrderType::OrderType orderType);
				QSharedPointer<PlaceSettingDetail> getPlaceSettingDetail(Common::Enum::OrderType::OrderType orderType);
				void addPlaceSettingDetail(Common::Enum::OrderType::OrderType orderType, const QSharedPointer<PlaceSettingDetail>& instrument);
				bool containsPlaceSettingDetail(Common::Enum::OrderType::OrderType orderType);
				QList<QSharedPointer<PlaceSettingDetail>> getPlaceSettingDetails();

				void update(const QDomNode& xmlPlaceSetting);
                QSharedPointer<PlaceSetting> copy();
			public:	
				bool getIsAutoClose() const
				{
					return _isAutoClose;
				}
				
				bool getAllowNewOpenBuy() const
				{
					return _allowNewOpenBuy;
				}

				bool getAllowNewOpenSell() const
				{
					return _allowNewOpenSell;
				}

				int getPriceValidTime() const
				{
					return _priceValidTime;
				}

				int getQuoteTimeout() const
				{
					return _quoteTimeout;
				}

				bool getAllowFullPayment() const
				{
					return _allowFullPayment;
				}

				bool getAllowAdvancePayment() const
				{
					return _allowAdvancePayment;
				}

				bool getAllowInstalment() const
				{
					return _allowInstalment;
				}

				bool getAllowDelivery() const
				{
					return _allowDelivery;
				}

				bool getAllowShortSell() const
				{
					return _allowShortSell;
				}
				
				bool getClosingUseCustomerQuotePolicy() const
				{
					return _closingUseCustomerQuotePolicy;
				}

				QDecNumber getContractSize() const
				{
					return _contractSize;
				}



				void setAllowAdvancePayment(const bool& value)
				{
					_allowAdvancePayment = value;
				}

				void setAllowDelivery(const bool& value)
				{
					_allowDelivery = value;
				}

				void setAllowFullPayment(const bool& value)
				{
					_allowFullPayment = value;
				}

				void setAllowInstalment(const bool& value)
				{
					_allowInstalment = value;
				}

				void setAllowNewOpenBuy(const bool& value)
				{
					_allowNewOpenBuy = value;
				}

				void setAllowNewOpenSell(const bool& value)
				{
					_allowNewOpenSell = value;
				}

				void setAllowShortSell(const bool& value)
				{
					_allowShortSell = value;
				}

				void setClosingUseCustomerQuotePolicy(const bool& value)
				{
					_closingUseCustomerQuotePolicy = value;
				}

				void setContractSize(const QDecNumber& value)
				{
					_contractSize = value;
				}

				void setIsAutoClose(const bool& value)
				{
					_isAutoClose = value;
				}

				void setPriceValidTime(const int& value)
				{
					_priceValidTime = value;
				}

				void setQuoteTimeout(const int& value)
				{
					_quoteTimeout = value;
				}
			};
		}
	}
}
#endif


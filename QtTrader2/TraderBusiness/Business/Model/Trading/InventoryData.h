#ifndef BUSINESS_MODEL_TRADING_INVENTORYDATA_H
#define BUSINESS_MODEL_TRADING_INVENTORYDATA_H

#include "Common/Struct/PlacingStruct.h"
#include "Common/Struct/PhysicalStruct.h"
#include <QList>
#include <QMap>
#include "Common/Enum/PlaceEnum.h"
#include "TradingOrder.h"
#include "Business/Model/Computing/ComputingResult.h"
#include "traderbusiness_global.h"

using namespace Common::Enum;
using namespace Common::Struct;
using namespace Business::Model::Computing;

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class BusinessAccount;
			class BusinessInstrument;
		}
	}
}


namespace Business
{
	class SettingManager;
	class ComputingManager;
	namespace Model
	{
		namespace Trading
		{			
			class TRADERBUSINESS_EXPORT InventoryItemData
			{	
			public:
				InventoryItemData()
				{
					_canDelivery = false;
					_canInstalment = false;
					_canPrepayment = false;
					_isShowUnpaidAmount = false;
					_isShowPhysicalPaymentDiscount = false;
					_isVisibility = false;
					_isSelect = false;
				};
				~InventoryItemData(){};

			public:
				QSharedPointer<OrderComputingResult> _orderComputingResult;
				QSharedPointer<TradingOrder> _tradingOrder;
				bool _canDelivery;
				bool _canInstalment;
				bool _canPrepayment;
				bool _isShowUnpaidAmount;
				bool _isShowPhysicalPaymentDiscount;
				bool _isVisibility;
				bool _isSelect;
			};

            struct OpenOrderStruct
            {
                QUuid _OrderId;
                QDecNumber _Orderlot;
                QDecNumber _OrderQuantity;
                QDecNumber _RequireLot;
                QDecNumber _RequireQuantity;
                QDecNumber _ContractSize;
                QDateTime _ExecuteTime;
                bool _IsChecked;
            };

			class TRADERBUSINESS_EXPORT InventoryData
			{				
			public:
				InventoryData(QSharedPointer<SettingManager> settingManager,
					QSharedPointer<ComputingManager> computingManager,
					QSharedPointer<Business::Model::Setting::BusinessAccount> businessAccount,
				QSharedPointer<Business::Model::Setting::BusinessInstrument> businessInstrument);

			public:
				void addOrder(QSharedPointer<TradingOrder>& tradingOrder, QSharedPointer<OrderComputingResult> orderComputingResult);
				void calculate();
				void setButtonIsEnabled(QSharedPointer<Business::Model::Computing::OverridedQuotationComputingResult> overridedQuotationComputingResult);
				bool isShowQuoteDescription();
				QString getRequireLotString();
				QString getRequireQuantityString();
				QString getRequireQuantityUIString();
                QString getAvailableQuantityString();
                QString getOriginalTotalQuantityString();
				bool reCalculateQuantity();
				void reCalculateCharge();
				bool checkRequireLot(QDecNumber requireLot);
                bool setAvailableQuantity(QDecNumber requireQuantity);
                void setAvailableQuantityByDeliveryLot(QDecNumber deliveryLot);
                QDecNumber getMaxAvailableQuantity(QDecNumber requireQuantityLock);
				QString getDeliveryChargeString();
				QDate checkDeliveryTime(QDate date);
				double getTotalLotBalance();
				void updateItems();
				bool isRemove();
				void setDeliveryCharge(QDecNumber value);
				int getDecimal(){ return _decimal;} 
				bool existInstalmentOrder();
				bool existRepaymentOrder();
				bool existInventoryItemData(QUuid id);
				void updateAccountInstrumentByUpdateCommand();
                void setOpenOrderSort();
                void fastSortForOpenOrder(QList<QSharedPointer<OpenOrderStruct>>& ptr, int begin, int end, Qt::SortOrder order);
                void initDeliveryTime();

			private:
				void setCanPrepayment(QSharedPointer<InventoryItemData> inventoryItemData);
                void setCanClose(QSharedPointer<InventoryItemData> inventoryItemData);
				QDate isDeliveryHolidaysDay(QDate date);
				QDate isDeliveryWeekDay(QDate date);
				QDate getVaildDeliveryDays(QDate date);
				QDate getBeginDateTime(int deliveryTimeBeginDay, QDate now);
                void reAddOpenOrderStructs();

			private:
				QSharedPointer<SettingManager> _settingManager;
				QSharedPointer<ComputingManager> _computingManager;
				QSharedPointer<Business::Model::Setting::BusinessAccount> _businessAccount;
				QSharedPointer<Business::Model::Setting::BusinessInstrument> _businessInstrument;

				bool _canDelivery;
				bool _canClose;
                QDecNumber _totalQuantity;
                QString _totalQuantityStr;
				QString _unit;
				QString _chargeCurrency;
				QString _averagePrice;
				QString _referencePrice;
				QString _marketValue;
				QDecNumber _requireQuantity;
				QDecNumber _requireLot;				
				QDecNumber _quantityLock;								
				QDecNumber _deliveryCharge;
				QString _deliveryChargeFormat;				
				QUuid _currencyId;
				bool _isShowDeliveryCharge;
				bool _isShowDeliveryAddress;
				bool _isSelectDeliveryQuantity;
				bool _isSelectDeliverySpecification;
				QList<QDate> _deliveryTimes;
				QDate _deliveryTime;
				QDate _beginDeliveryTime;
				QDate _endDeliveryTime;
				QMap<QUuid, QSharedPointer<InventoryItemData>> _inventoryItemDatas;

				int _decimal;
                QDecNumber _originalDeliveryTotalQuantity;
                QDecNumber _originalDeliveryTotalLot;
                QDecNumber _availableQuantity;
                QList<QSharedPointer<OpenOrderStruct>> _openOrderStructs;

			public:
				bool getCanDelivery()
				{
					return _canDelivery;
				}

				bool getCanClose()
				{
					return _canClose;
				}

							
				QString getTotalQuantity()
				{
                    return _totalQuantityStr;
				}

				QString getUnit()
				{
					return _unit;
				}

				QString getChargeCurrency()
				{
					return _chargeCurrency;
				}

				QString getAveragePrice()
				{
					return _averagePrice;
				}

				QString getReferencePrice()
				{
					return _referencePrice;
				}
				
				QString getMarketValue()
				{
					return _marketValue;
				}

				QMap<QUuid, QSharedPointer<InventoryItemData>> getInventoryItemDatas()
				{
					return _inventoryItemDatas;
				}

                QList<QSharedPointer<OpenOrderStruct>> getOpenOrderStructs()
                {
                    return _openOrderStructs;
                }

				bool canDeliverySubmit()
				{
					return _isSelectDeliveryQuantity && _isSelectDeliverySpecification;
				}
								
				QDate getDeliveryTime()
				{
					return _deliveryTime;
				}

				QDate getBeginDeliveryTime()
				{
					return _beginDeliveryTime;
				}
				
				QDate getEndDeliveryTime()
				{
					return _endDeliveryTime;				
				}

				QSharedPointer<Business::Model::Setting::BusinessAccount> getBusinessAccount()
				{
					return _businessAccount;
				}

				QSharedPointer<Business::Model::Setting::BusinessInstrument> getBusinessInstrument()
				{
					return _businessInstrument;
				}

				bool getIsShowDeliveryCharge()
				{
					return _isShowDeliveryCharge;
				}

				bool getIsShowDeliveryAddress()
				{
					return _isShowDeliveryAddress;
				}

				QString getDeliveryChargeFormat()
				{
					return _deliveryChargeFormat;
				}
								
				bool getIsSelectDeliverySpecification()
				{
					return _isSelectDeliverySpecification;
				}

				void setIsSelectDeliverySpecification(bool value)
				{
					_isSelectDeliverySpecification = value;
				}

				QDecNumber getRequireQuantity()
				{
					return _requireQuantity;
				}

				QDecNumber getDeliveryCharge()
				{
					return _deliveryCharge;
				}

				QUuid getCurrencyId()
				{
					return _currencyId;
				}

				QDecNumber getRequireLot()
				{
					return _requireLot;
				}

                QDecNumber getOriginalTotalQuantity()
                {
                    return _originalDeliveryTotalQuantity;
                }


				void setIsShowDeliveryCharge(bool value)
				{
					_isShowDeliveryCharge = value;
				}

				void setIsShowDeliveryAddress(bool value)
				{
					_isShowDeliveryAddress = value;
				}

				void setIsSelectDeliveryQuantity(bool value)
				{
					_isSelectDeliveryQuantity = value;
				}

				void setDeliveryTime(QDate value)
				{
					_deliveryTime = value;
				}

				

				
			};
		}
	}
}

#endif

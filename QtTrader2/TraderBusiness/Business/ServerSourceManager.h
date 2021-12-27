#ifndef BUSINESS_SERVERSOURCEMANAGER_H
#define BUSINESS_SERVERSOURCEMANAGER_H
#include <QDomNode>
#include <QSharedPointer>
#include "Business/Model/Server/Customer.h"
#include "Business/Model/Server/SystemParameter.h"
#include "Business/Model/Server/InstrumentUnit.h"
#include "Business/Model/Server/DealingPolicyDetail.h"
#include "Business/Model/Server/Instrument.h"
#include "Business/Model/Server/Account.h"
#include "Business/Model/Server/TradePolicyDetail.h"
#include "Business/Model/Server/QuotePolicy.h"
#include "Business/Model/Server/QuotePolicyDetail.h"
#include "Business/Model/Server/InstalmentPolicy.h"
#include "Business/Model/Server/InstalmentPolicyDetail.h"
#include "Business/Model/Server/DeliveryCharge.h"
#include "Business/Model/Server/VolumeNecessary.h"
#include "Business/Model/Server/VolumeNecessaryDetail.h"
#include "Business/Model/Server/DeliverySpecificationDetail.h"
#include "Business/Model/Server/PhysicalPaymentDiscount.h"
#include "Business/Model/Server/PhysicalPaymentDiscountDetail.h"
#include "Business/Model/Server/BOPolicy.h"
#include "Business/Model/Server/BOPolicyDetail.h"
#include "Business/Model/Server/BOBetType.h"
#include "Business/Model/Server/Organization.h"
#include "Business/Model/Server/CounterParty.h"
#include "Business/Model/Server/AdministrativeCharge.h"
#include "Business/Model/Server/AdministrativeChargeDetail.h"
#include "Business/Model/Server/PricingPolicyDetail.h"
#include <QXmlStreamReader>
#include "Common/Enum/CommonEnum.h"
#include "traderbusiness_global.h"
#include "Business/Model/Setting/BusinessAccount.h"
#include "QDecNumber.h"

using namespace Common::Enum;
using namespace Business::Model;


namespace Business
{
	class SettingManager;
	class InitializeManager;
	class TRADERBUSINESS_EXPORT ServerSourceManager
	{		
	private:
		QSharedPointer<SettingManager> _settingManager;
		QSharedPointer<InitializeManager> _initializeManager;
        QSharedPointer<Business::Model::Server::Customer>  _customer;
        QSharedPointer<Business::Model::Server::SystemParameter>  _systemParameter;
        QMap<QUuid, QSharedPointer<Business::Model::Server::InstrumentUnit>> _instrumentUnits;
        QMap<QString, QSharedPointer<Business::Model::Server::DealingPolicyDetail>> _dealingPolicyDetails;//DealingPolicyId+InstrumentId
        QMap<QUuid, QSharedPointer<Business::Model::Server::Instrument>> _instruments;
        QMap<QUuid, QSharedPointer<Business::Model::Server::Account>> _accounts;
        QMap<QString, QSharedPointer<Business::Model::Server::TradePolicyDetail>> _tradePolicyDetails;//TradePolicyId+InstrumentId
        QMap<QUuid, QSharedPointer<Business::Model::Server::QuotePolicy>> _quotePolicies;
        QMap<QString, QSharedPointer<Business::Model::Server::QuotePolicyDetail>> _quotePolicyDetails;//QuotePolicyId+InstrumentId
        QMap<QUuid, QSharedPointer<Business::Model::Server::InstalmentPolicy>> _instalmentPolicies;
        QMap<QString, QSharedPointer<Business::Model::Server::InstalmentPolicyDetail>>_instalmentPolicyDetails;//InstalmentPolicyId+Period+Frequence
        QMap<QUuid, QSharedPointer<Business::Model::Server::DeliveryCharge>> _deliveryCharges;
        QMap<QUuid, QSharedPointer<Business::Model::Server::VolumeNecessary>> _volumeNecessaries;
        QMap<QUuid, QSharedPointer<Business::Model::Server::VolumeNecessaryDetail>> _volumeNecessaryDetails;
        QMap<QString, QSharedPointer<Business::Model::Server::DeliverySpecificationDetail>>_deliverySpecificationDetails;//DeliverySpecificationId+Size
        QMap<QUuid, QSharedPointer<Business::Model::Server::PhysicalPaymentDiscount>> _physicalPaymentDiscounts;
        QMap<QUuid, QSharedPointer<Business::Model::Server::PhysicalPaymentDiscountDetail>> _physicalPaymentDiscountDetails;
        QMap<QUuid, QSharedPointer<Business::Model::Server::BOPolicy>> _BOPolicies;
        QMap<QString, QSharedPointer<Business::Model::Server::BOPolicyDetail>>_BOPolicyDetails;//BOPolicyId+BOBetTypeId+Frequency
        QMap<QUuid, QSharedPointer<Business::Model::Server::BOBetType>> _BOBetTypes;
        QMap<QUuid, QSharedPointer<Business::Model::Server::Organization>> _organizations;
        QMap<int, QSharedPointer<Business::Model::Server::CounterParty>> _counterParties;
        QMap<QUuid, QSharedPointer<Business::Model::Server::AdministrativeCharge>> _AdministrativeCharges;
        QMap<QString, QSharedPointer<Business::Model::Server::AdministrativeChargeDetail>>_AdministrativeChargeDetails;//AdministrativeChargeId+Quantity
        QStringList _instrumentAndQuotePolicyIDs;
        QMap<QUuid, QSharedPointer<Business::Model::Server::PricingPolicyDetail>>_PricingPolicyDetails;
        QList<QString> _reportGroupDetails;

	public:
		ServerSourceManager(QSharedPointer<SettingManager>& settingManager, QSharedPointer<InitializeManager>& initializeManager);
		bool initSettingSourceData(const QByteArray&  dataByteArray);
		void updateSettingSourceData(const QDomNode& xmlSettingData);
		void calculate();
		QStringList getInstrumentAndQuotePolicyIDs() { return _instrumentAndQuotePolicyIDs;  }
		int getRearPrice(const QUuid& id);
		bool isBVI(const QUuid& id);
		QSharedPointer<Server::CounterParty> getCounterParty(const int& id);
		int getCounterPartyId(const QUuid& instrumnetId);
		bool getAllowEmployeeTrading(const QUuid& accountId);
		bool getAllowEmployeeTrading();
        QSharedPointer<Business::Model::Server::InstrumentUnit> getInstrumentUnit(const QUuid& id);
        void setIsSptReconfirm(bool value);
        QSharedPointer<Business::Model::Server::PricingPolicyDetail> getPricingPolicyDetail(const QUuid& instrumentId, QDecNumber marketPrice);
        QSharedPointer<Business::Model::Server::Customer>  getCustomer() { return _customer; }
        QList<QString> getReportGroupDetails() { return _reportGroupDetails;}

	private:
        QSharedPointer<Business::Model::Server::InstrumentUnit> getOrAddInstrumentUnit(const QUuid& id);        
        QSharedPointer<Business::Model::Server::DealingPolicyDetail> getOrAddDealingPolicyDetail(const QString& key);
        QSharedPointer<Business::Model::Server::Instrument> getOrAddInstrument(const QUuid& id);
        QSharedPointer<Business::Model::Server::Account> getOrAddAccount(const QUuid& id);
        QSharedPointer<Business::Model::Server::TradePolicyDetail> getOrAddTradePolicyDetail(const QString& key);
        QSharedPointer<Business::Model::Server::QuotePolicy> getOrAddQuotePolicy(const QUuid& id);
        QSharedPointer<Business::Model::Server::QuotePolicyDetail> getOrAddQuotePolicyDetail(const QString& key);
        QSharedPointer<Business::Model::Server::InstalmentPolicy> getOrAddInstalmentPolicy(const QUuid& id);
        QSharedPointer<Business::Model::Server::InstalmentPolicyDetail> getOrAddInstalmentPolicyDetail(const QString& key);
        QSharedPointer<Business::Model::Server::DeliveryCharge> getOrAddDeliveryCharge(const QUuid& id);
        QSharedPointer<Business::Model::Server::VolumeNecessary> getOrAddVolumeNecessary(const QUuid& id);
        QSharedPointer<Business::Model::Server::VolumeNecessaryDetail> getOrAddVolumeNecessaryDetail(const QUuid& id);
        QSharedPointer<Business::Model::Server::DeliverySpecificationDetail> getOrAddDeliverySpecificationDetail(const QString& key);
        QSharedPointer<Business::Model::Server::PhysicalPaymentDiscount> getOrAddPhysicalPaymentDiscount(const QUuid& id);
        QSharedPointer<Business::Model::Server::PhysicalPaymentDiscountDetail> getOrAddPhysicalPaymentDiscountDetail(const QUuid& id);
        QSharedPointer<Business::Model::Server::BOPolicy> getOrAddBOPolicy(const QUuid& id);
        QSharedPointer<Business::Model::Server::BOPolicyDetail> getOrAddBOPolicyDetail(const QString& key);
        QSharedPointer<Business::Model::Server::BOBetType> getOrAddBOBetType(const QUuid& id);
        QSharedPointer<Business::Model::Server::Organization> getOrAddOrganization(const QUuid& id);
        QSharedPointer<Business::Model::Server::CounterParty> getOrAddCounterParty(const int& id);
        QSharedPointer<Business::Model::Server::AdministrativeCharge> getOrAddAdministrativeCharge(const QUuid& id);
        QSharedPointer<Business::Model::Server::AdministrativeChargeDetail> getOrAddAdministrativeChargeDetail(const QString& key);
        QSharedPointer<Business::Model::Server::PricingPolicyDetail> getOrAddPricingPolicyDetail(const QUuid& key);

		void readDoubleStructureElement(QXmlStreamReader& xmlStreamReader, QString name);
		void updateDoubleStructureElement(QDomNode& xmlData, QString name, UpdateAction::UpdateAction updateAction);
		void setSystemSetting();
		void setAccountSetting();
		void setInstrumentSettings(QSharedPointer<Setting::BusinessAccount>& account, QSharedPointer<Server::Account>& accountSource);
		void setInstrumentSettingItem(
			QSharedPointer<Setting::BusinessInstrument>& instrument,
            QSharedPointer<Business::Model::Server::Account>& accountSource,
            QSharedPointer<Business::Model::Server::Instrument>& instrumentSource,
            QSharedPointer<Business::Model::Server::TradePolicyDetail>& tradePolicyDetailSource,
            QSharedPointer<Business::Model::Server::DealingPolicyDetail>& dealingPolicyDetailSource,
            QSharedPointer<Business::Model::Server::QuotePolicyDetail>& quotePolicyDetailSource);
		void setCalculateSetting(QSharedPointer<Setting::CalculateSetting>& calculateSetting,
            QSharedPointer<Business::Model::Server::Account>& accountSource,
            QSharedPointer<Business::Model::Server::Instrument>& instrumentSource,
            QSharedPointer<Business::Model::Server::TradePolicyDetail>& tradePolicyDetailSource);
		void setMovePipsInfoSetting(QSharedPointer<Setting::MovePipsInfoSetting>& movePipsInfoSetting, 
            QSharedPointer<Business::Model::Server::Instrument>& instrumentSource,
            QSharedPointer<Business::Model::Server::TradePolicyDetail>& tradePolicyDetailSource);
		void setPlaceSettingItem(
			QSharedPointer<Setting::PlaceSetting>& placeSetting,
            QSharedPointer<Business::Model::Server::Account>& accountSource,
            QSharedPointer<Business::Model::Server::Instrument>& instrumentSource,
            QSharedPointer<Business::Model::Server::TradePolicyDetail>& tradePolicyDetailSource,
            QSharedPointer<Business::Model::Server::DealingPolicyDetail>& dealingPolicyDetailSource);
		void setLotSetting(QSharedPointer<Setting::LotSetting>& lotSetting,
            QSharedPointer<Business::Model::Server::Account>& accountSource,
            QSharedPointer<Business::Model::Server::Instrument>& instrumentSource,
            QSharedPointer<Business::Model::Server::TradePolicyDetail>& tradePolicyDetailSource,
            QSharedPointer<Business::Model::Server::DealingPolicyDetail>& dealingPolicyDetailSource);

		void setInstrumentQuotePolicyMappings(QXmlStreamReader& xmlStreamReader);
		void setAccountForMarketingGroups(QXmlStreamReader& xmlStreamReader);
        void setPricingPolicyDetails(QXmlStreamReader& xmlStreamReader);

        QSharedPointer<Business::Model::Server::DealingPolicyDetail> getDealingPolicyDetail(const QUuid& dealingPolicyId, const QUuid& instrumentId);        

	};

}
#endif

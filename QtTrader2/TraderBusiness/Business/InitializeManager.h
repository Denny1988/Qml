#ifndef BUSINESS_INITIALIZEMANAGER_H
#define BUSINESS_INITIALIZEMANAGER_H

#include <QDomNode>
#include "Common/Struct/SettingStruct.h"
#include <QList>
#include <QUuid>
#include <QMap>
#include <QSharedPointer>
#include "Business/Model/Setting/TradingListColumnsVisibility.h"
#include "traderbusiness_global.h"

using namespace Common::Enum;
using namespace Business::Model::Setting;
namespace Business
{
	class SettingManager;
	class TRADERBUSINESS_EXPORT InitializeManager
	{
	private:
		QMap<QUuid, QSharedPointer<Common::Struct::SettingAccount>> _accounts;			
		QMap<QUuid, QSharedPointer<Common::Struct::SettingInstrument>> _instruments;	
		QSharedPointer<SettingManager> _settingManager;
		QMap<TradingListNameEnum::TradingListNameEnum, QList<QSharedPointer<TradingListColumnsVisibility>>> _tradingListColumnsHides;


	public:
		InitializeManager();
		QList<QSharedPointer<Common::Struct::SettingAccount>> getAccounts();
		QList<QSharedPointer<Common::Struct::SettingAccount>> getAccountsBySortAccountCode();
		QList<QSharedPointer<Common::Struct::SettingInstrument>> getInstruments();	
		QList<QSharedPointer<Common::Struct::SettingInstrument>> getInstrumentsBySortCode();
        QList<QSharedPointer<Common::Struct::SettingInstrument>> getInstrumentsBySortSequenceAndCode();
        QList<QSharedPointer<Common::Struct::SettingInstrument>> getLayoutInstruments(QList<QUuid> layoutInstrumentIds);
        QList<QSharedPointer<Common::Struct::SettingInstrument>> getAllInstrumentsBySortSequenceAndCode(QList<QUuid> layoutInstrumentIds);
        QList<QSharedPointer<Common::Struct::SettingInstrument>> getNewAddInstruments(QList<QUuid> oldInstrumentIds);
		QList<QSharedPointer<Common::Struct::SettingInstrument>> getUnCheckInstruments(QList<QString> notContainInstrumentIds);		
		QSharedPointer<Common::Struct::SettingInstrument> getInstrument(QUuid instrumentId);
		QSharedPointer<Common::Struct::SettingAccount> getAccount(QUuid accountId);
		int getPhysicalCount();
		bool isContainsAccount(QUuid id);	
		bool isExistAccountOrInstrument();
		bool isExistInstrument();
		int getBOInstrumentCount();
		int getInstrumentCount();
		int getNotBOInstrumentCount();
		QList<QString> getTradingListColumnsHides(TradingListNameEnum::TradingListNameEnum tradingListNameEnum);
		void removeNoActiveInstrument(QUuid instrumentId);
		bool isExistInstrument(QUuid instrumentId);
        bool isShowOfficialFixing();
        bool isShowOfficialFixingByInstrument(QUuid instrumentId);
        QString getFixingDateByInstrument(QUuid instrumentId);
        QList<QString> getInstrumentIds();
        QMap<QUuid, bool> getEnableClickTradeList();
        QList<QUuid> getInstrumentKeyIds();

	public:
        void updateInitializeData(const QDomNode& xmlInitializeData, QSharedPointer<SettingManager>& settingManager);
		
	};

}
#endif

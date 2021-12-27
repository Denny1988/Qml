#ifndef COMMON_APPSETTING_H
#define COMMON_APPSETTING_H
#include <QSettings>
#include <QSharedPointer>
#include "Struct/LoginSettingStruct.h"
#include "traderbusiness_global.h"

namespace Common
{
	class TRADERBUSINESS_EXPORT AppSetting
	{	
	public:
		static AppSetting* getInstance();
        void createSettingByPath(QString path);
		void setValue(const QString & type, const QString & key, const QVariant & value);
        QVariant getValue(QString key);
        QVariant getValue(const QString & type, const QString & key);
		void removeValue(const QString & type, const QString & key);
		int getBeatInterval();
		int getBeatIntervalOnNotReceiveCommand();
		int getBeatIntervalOnReceiveCommand();
		int getHeartbeaterTimeout();
		int getContinuityHeartbeaterTimeoutCount();
		int getConnectTimeout();
		int getRecoverTimeout();
		int getRecoverConnectionMaxCount();
		int getAsyncTimeoutCheckTimerInterval();
		int getReGetTradingDataMaxCount();
		bool getContainsMilSInQuotationTime();
		bool getWriteLog();
		int getBORealtimeCount();
		int getSynchronizeTime();
		int getRefreshViewTimerInterval();
		int getWatchWorlds();
		int getWatchWorldsLessthanCount();
		int getTimeOutFromProvisional();
		int getProvisionalTimerInterval();
		bool getEnableSystemPictureButton();
		bool getIsNeedLoadMultiTradingData();
        void setIsNeedLoadMultiTradingData(bool isNeedLoadMultiTradingData);
        //by default false,if value is true modify the system style Chart Toolbar style to change, else not change
        bool getIsChangeChartToolbarByChangeSystemTheme();
        bool getAllUserSetChartColorSign();
        int getNoQuotationChangeSingleChannelTime();

	private:
		AppSetting();
		static AppSetting* _instance;
		QSettings * _settings;
		bool _isNeedLoadMultiTradingData;
	};
}
#endif

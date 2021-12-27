#ifndef COMMON_CUSTOMERSETTING_H
#define COMMON_CUSTOMERSETTING_H
#include <QSettings>
#include <QMap>
#include <QUuid>
#include <QSettings> 
#include "traderbusiness_global.h"

namespace Common
{
	struct TRADERBUSINESS_EXPORT BORealTimePriceSettingStruct
	{
		bool bDrawGrid;
		bool bDrawBarStroke;
		bool bDrawCrosshair;
		bool isShowXaxis;
		bool isShowBOText;
		QString colorBarStroke;
		QString colorBackground;
		QString coordinateBackground;
		QString colorBOText;
		QString colorForeground;
		QString colorGrid;
		QString colorCrosshair;
		QString colorBullish;
		QString colorBearish;
	};

	class TRADERBUSINESS_EXPORT CustomerSetting
	{	
	public:
		static CustomerSetting* getInstance();
		void setValue(const QString & type, const QString & key, const QVariant & value);
		void removeValue(const QString & type, const QString & key);
        void createSettingByPath(QString path);
        void createSetting(QString userName);
		QString getValue(QString key);
		QString getValue(const QString & type, const QString & key);
		double getLot(QUuid accountId, QUuid instrumentId);
		int getMovePips(QUuid accountId, QUuid instrumentId);
		bool getIsSaveLot(QUuid accountId, QUuid instrumentId);
		bool getIsSaveMovePips(QUuid accountId, QUuid instrumentId);
		void saveLot(QUuid accountId, QUuid instrumentId, bool isSave, double lot);
		void saveMovePips(QUuid accountId, QUuid instrumentId, bool isSave, int movePips);
		int getDisclaimerOption();
		void setDisclaimerOption(int option);
		bool isShowDisclaimerDialog();
		bool getCheckAccountStatus(QUuid accountId);
		void setCheckAccountStatus(QUuid accountId, bool checkValue);
		void setCheckAccountsStatus(QList<QUuid> accountIds, bool checkValue);
		void saveBORealTimePriceSettings(BORealTimePriceSettingStruct BORealTimePriceSettings);
		BORealTimePriceSettingStruct getBORealTimePriceSettings();
        //fix bug: show reference price Column and hide auto limit/auto stop/Commission/Rebate Column for Open Order List
        //value is true mean used default layout for system, value is false mean used myself layout for system
        //if value is null set value for true
        bool getIsUseDefaultLayout();
        //By default, this property is false; return false change chart bgColor/fontColor when change system theme, else do not change
        void saveUserSetChartColorSign(bool value);
        bool getUserSetChartColorSign();
        void saveUserSetRealTimeChartColorSign(bool value);
        bool getUserSetRealTimeChartColorSign();


	private:
		CustomerSetting();
		static CustomerSetting* _instance;
		QSettings * _settings;
		QString _userName;
		QMap<QString, QString> _settingMap;
	};
}
#endif

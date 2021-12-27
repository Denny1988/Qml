#ifndef COMMON_LOGINSETTING_H
#define COMMON_LOGINSETTING_H
#include <QSettings>
#include <QSharedPointer>
#include "Struct/LoginSettingStruct.h"
#include "traderbusiness_global.h"

namespace Common
{
	class TRADERBUSINESS_EXPORT LoginSetting
	{	
	public:
		static LoginSetting* getInstance();
		bool saveLoginSetting(QSharedPointer<Struct::LoginSettingStruct> loginSetting);
		QSharedPointer<Struct::LoginSettingStruct> getLoginSetting();
		void savePasswordSetting(QString pwd, int length);
		void clearUserAndPasswordSetting();
		QString getLanguage();
		void setCurrentLanguage(QString language);
		void setValue(const QString & type, const QString & key, const QVariant & value);
        QString getValue(const QString & type, const QString & key);
		void removeValue(const QString & type, const QString & key);
		bool getUseTraderQuotationServer();
		void saveAutoUpdateSetting(QSharedPointer<Struct::AutoUpdateSettingStruct> loginSetting);
		QSharedPointer<Struct::AutoUpdateSettingStruct> getAutoUpdateSetting();		
		bool getFromTraderStartUp();
		void setFromTraderStartUp();
        void createSettingByPath(QString path);

	private:
		LoginSetting();
		static LoginSetting* _instance;
		QSettings * _settings;
	};
}
#endif

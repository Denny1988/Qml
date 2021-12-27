#ifndef LOGINSETTINGSTRUCT_H
#define LOGINSETTINGSTRUCT_H
#include <QString>

namespace Common
{
	namespace Struct
	{
		struct LoginSettingStruct
		{
			QString		_loginName;
			QString		_loginPassword;
			bool		_isUserNameRemembered;
			bool		_isPasswordRemembered;
			QString		_currentLanguage;
			QString		_currentServer;
			QString		_currentConnection;
            QString		_currentConnectionValue;
			QString		_useDefinedConnection;
			bool        _isUseDefined;
                        QString     _LastFaildServer;
		};

		struct AutoUpdateSettingStruct
		{
			QString _autoUpdateServer;
			QString _autoUpdateServerPort;
			QString _reTranslateTimes;
			QString _organization;
			QString _appName;
			QString _organization_NewVersion;
		};
	}
}
#endif

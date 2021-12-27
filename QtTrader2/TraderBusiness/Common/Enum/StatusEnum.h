#ifndef LOGINENUM_H
#define LOGINENUM_H

#include <QObject>
#include <QMetaType> 
namespace Common
{
	namespace Enum
	{
		namespace StatusEnum
		{
			enum StatusEnum
			{
				CONNECT_START,
				CONNECT_SUCCESS,
				CONNECT_FAILED,
				LOGIN_START,
				LOGIN_SUCCESS,
				LOGIN_FAILED,
				DISCONNECTION
			};
			enum InitializeDataStatus
			{
				None,
				BeginInitData,
				EndInitData,
				BeginInitQuotation,
				BeginGetTradingAndSettingData,
				EndTradingData,
				EndSettingData,
			};
		}
	}
}
Q_DECLARE_METATYPE(Common::Enum::StatusEnum::StatusEnum); 
Q_DECLARE_METATYPE(Common::Enum::StatusEnum::InitializeDataStatus); 
#endif	   //LOGINENUM_H

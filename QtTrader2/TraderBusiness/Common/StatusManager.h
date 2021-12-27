#ifndef COMMON_STATUSMANAGER_H
#define COMMON_STATUSMANAGER_H

#include <QObject>
#include "Common/Enum/StatusEnum.h"
#include "traderbusiness_global.h"

namespace Common
{
	class TRADERBUSINESS_EXPORT StatusManager : public QObject
	{
		Q_OBJECT
		private:
			StatusManager();
			~StatusManager();

		signals:
			void statusSignal(Common::Enum::StatusEnum::StatusEnum status);
			void initTradingOrderCompletedSignal();
			void initializeDataCompletedSignal();
			void initSettingDataCompletedSignal();
			void computingCompletedSignal();
			void showAccountOrdersSignal(const QString&,bool);
			void changeThemeSignal();
			void initFailSignal();
			void initSettingDataFailSignal();
            void initTradingDataFailSignal();

		public:
			static StatusManager*	getInstance();
			void SetStatus(Common::Enum::StatusEnum::StatusEnum status);
			void setInitDataStatus(bool isCompleted);
			void setInitSettingDataStatus(bool isCompleted);
			void setInitTradingDataStatus(bool isCompleted);
			void setComputingStatus(bool isCompleted);
			bool isInitDataCompleted();
			bool isSettingDataCompleted();
			bool isTradingDataCompleted();
			bool isComputingCompleted();
			void changeFIFOState(bool isFiFo);
			void clearStatus();
			bool isCustomerNoAccountStatus();
			bool isNeedSelectAccountStatus();
			bool isNeedSelectInstrumentStatus();
			void setIsCustomerNoAccountStatus(bool isCustomerNoAccountStatus);
			void setIsNeedSelectAccountStatus(bool isNeedSelectAccountStatus);
			void setIsNeedSelectInstrumentStatus(bool isNeedSelectInstrumentStatus);
			bool isInitFailStatus();
			bool isInitSettingDataFailStatus();
			bool isInitTradingDataFailStatus();
			void setIsInitFailStatus(bool isInitFailStatus);
			void setIsInitSettingDataFailStatus(bool isInitSettingDataFailStatus);
			void setIsInitTradingDataFailStatus(bool isInitTradingDataFailStatus);
			bool isLoadLayoutStatus() { return _IsLoadLayoutStatus;}
            void setIsLoadLayoutStatus(bool value) { _IsLoadLayoutStatus = value;}

		public slots:
			void appAboutToQuit();

		private:
			static	StatusManager*	_instance; 
			bool     _IsInitDataCompleted;
			bool     _IsSettingDataCompleted;
			bool     _IsTradingDataCompleted;
			bool     _IsComputingCompleted;
			bool     _IsCustomerNoAccount;
			bool     _IsNeedSelectAccount;
			bool     _IsNeedSelectInstrument;
			bool	_IsInitFailStatus;
			bool	_IsInitSettingDataFailStatus;
			bool	_IsInitTradingDataFailStatus;
			bool	_IsLoadLayoutStatus;
	};
}
#endif	  //LOGINFORMSTATUS_H

#include "Common/StatusManager.h"
#include "../../Common/TimerManager.h"
#include "Service/ServiceManager.h"
#include "ThreadManager.h"
#include <QCoreApplication>
#include "Business/BusinessManager.h"
namespace Common
{
	StatusManager* StatusManager::_instance=Q_NULLPTR;
	StatusManager* StatusManager::getInstance()
	{
			if (Q_NULLPTR==_instance)
			{
				_instance = new StatusManager;
			}
			return _instance;
	}

	StatusManager::StatusManager()
	{
		this->clearStatus();
	}

	StatusManager::~StatusManager()
	{

	}

	void StatusManager::appAboutToQuit()
	{
		Service::ServiceManager::getInstance()->stop();
		ThreadManager::getInstance()->stop();
	}

	void StatusManager::SetStatus(Common::Enum::StatusEnum::StatusEnum status)
	{
		emit statusSignal(status);
	}

	void StatusManager::setInitDataStatus(bool isCompleted)
	{
		this->_IsInitDataCompleted = isCompleted;
		if(isCompleted) emit initializeDataCompletedSignal();
	}
			
	void StatusManager::setInitSettingDataStatus(bool isCompleted)
	{
		this->_IsSettingDataCompleted = isCompleted;
		if (isCompleted) emit initSettingDataCompletedSignal();
		
		Common::TimerManager::getInstance()->setUpdateInterval(Common::AppSetting::getInstance()->getRefreshViewTimerInterval());
		Common::TimerManager::getInstance()->start();
	}
			
	void StatusManager::setInitTradingDataStatus(bool isCompleted)
	{
		this->_IsTradingDataCompleted = isCompleted;
		if(isCompleted) emit initTradingOrderCompletedSignal(); 
	}

	void StatusManager::setComputingStatus(bool isCompleted)
	{
		this->_IsComputingCompleted = isCompleted;
		if(isCompleted) emit computingCompletedSignal();
	}
			
	bool StatusManager::isInitDataCompleted()
	{
		return this->_IsInitDataCompleted;
	}
			
	bool StatusManager::isSettingDataCompleted()
	{
		return this->_IsSettingDataCompleted;
	}
			
	bool StatusManager::isTradingDataCompleted()
	{
		return this->_IsTradingDataCompleted;
	}

	bool StatusManager::isComputingCompleted()
	{
		return this->_IsComputingCompleted;
	}
	
	void StatusManager::setIsCustomerNoAccountStatus(bool isCustomerNoAccountStatus)
	{
		this->_IsCustomerNoAccount = isCustomerNoAccountStatus;
	}
			
	void StatusManager::setIsNeedSelectAccountStatus(bool isNeedSelectAccountStatus)
	{
		this->_IsNeedSelectAccount = isNeedSelectAccountStatus;
	}
	
	void StatusManager::setIsNeedSelectInstrumentStatus(bool isNeedSelectInstrumentStatus)
	{
		this->_IsNeedSelectInstrument = isNeedSelectInstrumentStatus;
	}

	
	bool StatusManager::isInitFailStatus()
	{
		return this->_IsInitFailStatus;
	}
	
	bool StatusManager::isInitSettingDataFailStatus()
	{
		return this->_IsInitSettingDataFailStatus;
	}

	bool StatusManager::isInitTradingDataFailStatus()
	{
		return this->_IsInitTradingDataFailStatus;
	}
	
	void StatusManager::setIsInitFailStatus(bool isInitFailStatus)
	{
		this->_IsInitFailStatus = isInitFailStatus;
		if(isInitFailStatus)
		{
			emit initFailSignal();
		}
	}
	
	void StatusManager::setIsInitSettingDataFailStatus(bool isInitSettingDataFailStatus)
	{
		this->_IsInitSettingDataFailStatus = isInitSettingDataFailStatus;
		if(isInitSettingDataFailStatus)
		{
			emit initSettingDataFailSignal();
		}
	}

	void StatusManager::setIsInitTradingDataFailStatus(bool isInitTradingDataFailStatus)
	{
		this->_IsInitTradingDataFailStatus = isInitTradingDataFailStatus;
		if(isInitTradingDataFailStatus)
		{
			emit initTradingDataFailSignal();
		}
	}

	bool StatusManager::isCustomerNoAccountStatus()
	{
		return this->_IsCustomerNoAccount;
	}
			
	bool StatusManager::isNeedSelectAccountStatus()
	{
		return this->_IsNeedSelectAccount;
	}
	
	bool StatusManager::isNeedSelectInstrumentStatus()
	{
		return this->_IsNeedSelectInstrument;
	}
	
	void StatusManager::clearStatus()
	{
		this->_IsInitDataCompleted = false;
		this->_IsSettingDataCompleted = false;
		this->_IsTradingDataCompleted = false;
		this->_IsComputingCompleted = false;
		this->_IsCustomerNoAccount = false;
		this->_IsNeedSelectAccount = false;
		this->_IsNeedSelectInstrument = false;
		this->_IsInitFailStatus = false;
		this->_IsInitSettingDataFailStatus = false;
		this->_IsInitTradingDataFailStatus = false;
		this->_IsLoadLayoutStatus = false;
	}
}
#include "moc_StatusManager.cpp"
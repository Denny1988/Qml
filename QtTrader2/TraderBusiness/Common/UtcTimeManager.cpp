#include "UtcTimeManager.h"
#include "ThreadManager.h"
#include "Service/ServiceManager.h"
#include "Common/Constants/DateTimeConstants.h"
#include <qDebug>

namespace Common
{
	UtcTimeManager* UtcTimeManager::getInstance()
	{
		if (!_instance)
		{
			_instance = new UtcTimeManager;
		}
		return _instance;
	}

	UtcTimeManager* UtcTimeManager::_instance = Q_NULLPTR;

	UtcTimeManager::UtcTimeManager()
	{
	}

	QDateTime UtcTimeManager::getUtcNow()
	{
		return QDateTime::currentDateTime().addMSecs(this->_serverToClientMilliseconds);
	}

	UtcTimeManager::~UtcTimeManager()
	{
	}

	void UtcTimeManager::startServerDateTime(qint64 serverToClientMilliseconds)
	{
		this->_serverToClientMilliseconds = serverToClientMilliseconds;
		_timer = new QTimer();
		_timer->setInterval(1000 * 60 * AppSetting::getInstance()->getSynchronizeTime());
		_timer->start();
		connect(_timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
	}

	void UtcTimeManager::stop()
	{
		disconnect(_timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
		_timer->stop();
	}

	void UtcTimeManager::onTimerOut()
	{
		if (!Service::ServiceManager::getInstance()->isStop())
		{
			this->_startNow = QDateTime::currentDateTime();
			this->m_ServerDateTimeResult = Service::ServiceManager::getInstance()->getTradeService()->getServerDateTime();
			connect(this->m_ServerDateTimeResult.data(), SIGNAL(completed()), this, SLOT(getServerDateTimeResultCompletedSlot()));
		}
	}

	void UtcTimeManager::getServerDateTimeResultCompletedSlot()
	{
		if (!this->m_ServerDateTimeResult->hasError())
		{
			QDateTime endNow = QDateTime::currentDateTime();
			this->_startNow = this->_startNow.addMSecs(this->_startNow.msecsTo(endNow));
			QDomElement xmlResult = this->m_ServerDateTimeResult->getXmlResult();
			QString serverDateStr = xmlResult.text();
			QDateTime serverDate = QDateTime::fromString(serverDateStr.mid(0, serverDateStr.length() - 4), Common::Constants::DateTimeConstants::DateTimeFormat);
			this->_serverToClientMilliseconds = this->_startNow.msecsTo(serverDate);
		}
	}

}
#include "moc_UtcTimeManager.cpp"
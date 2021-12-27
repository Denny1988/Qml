#include "TimerManager.h"
#include "ThreadManager.h"

namespace Common
{
	TimerManager* TimerManager::getInstance()
	{
		if(!_instance)
		{
			_instance = new TimerManager;
		}
		return _instance;
	}

	TimerManager* TimerManager::_instance = Q_NULLPTR;

	TimerManager::TimerManager()
	{		
		this->moveToThread(ThreadManager::getInstance()->getTimerThread());
		_viewModeltimer.moveToThread(ThreadManager::getInstance()->getTimerThread());
		QObject::connect(this,SIGNAL(startTiemrSignal()),this,SLOT(startTimerSlot()),Qt::BlockingQueuedConnection);
		QObject::connect(this,SIGNAL(stopTimerSignal()),this,SLOT(stopTimerSlot()),Qt::BlockingQueuedConnection);
	}

	TimerManager::~TimerManager()
	{
		disconnect(this,SIGNAL(startTiemrSignal()),this,SLOT(startTimerSlot()));
		disconnect(this,SIGNAL(stopTimerSignal()),this,SLOT(stopTimerSlot()));
	}

	void TimerManager::setUpdateInterval(int updateInterval)
	{
		this->_viewModeltimer.setInterval(updateInterval);
	}

	void TimerManager::start()
	{	
		emit  startTiemrSignal();
	}


	void TimerManager::clear()
	{
            _instance = Q_NULLPTR;
	}

	void TimerManager::startTimerSlot()
	{		
		this->_viewModeltimer.start();
	}
		
	void TimerManager::stopTimerSlot()
	{	
		this->_viewModeltimer.stop();
	}
	
	QTimer* TimerManager::getViewModelTimer()
	{
		return &this->_viewModeltimer; 
	}
}
#include "moc_TimerManager.cpp"

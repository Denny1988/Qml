#include "ThreadManager.h"

namespace Common
{
	ThreadManager* ThreadManager::getInstance()
	{
		if(!_instance)
		{
			_instance = new ThreadManager;
		}
		return _instance;
	}

	ThreadManager* ThreadManager::_instance = Q_NULLPTR;

	ThreadManager::ThreadManager()
	{
		this->_timerThread.start();
		this->_viewModelThread.start();
		this->_businessThread.start();
		this->_chartThread.start();
	}

	void ThreadManager::stop()
	{
		//if(this->_timerThread.isRunning())
		//{
		//	this->_timerThread.quit();
		//	this->stopThread(this->_timerThread);
		//}

		//if(this->_viewModelThread.isRunning())
		//{
		//	this->_viewModelThread.quit();
		//	this->stopThread(this->_viewModelThread);
		//}

		//if(this->_businessThread.isRunning())
		//{
		//	this->_businessThread.quit();
		//	/*this->stopThread(this->_businessThread);*/
		//}
	}

	void ThreadManager::stopThread(QThread& thread)
	{
		//bool isok = thread.wait(3000);
		//if(!isok)
		//	thread.terminate();
	}

	QThread* ThreadManager::getViewModelThread() 
	{
		return &_viewModelThread; 
	}

	QThread* ThreadManager::getBusinessThread() 
	{
		return &_businessThread; 
	}

	QThread* ThreadManager::getTimerThread() 
	{
		return &_timerThread;
	}

	QThread* ThreadManager::getChartThread()
	{
		return &_chartThread;
	}

	QThread* ThreadManager::getTimeThread()
	{
		return &_timeThread;
	}
}

#include "moc_ThreadManager.cpp"
#ifndef COMMON_THREADMANAGER_H
#define COMMON_THREADMANAGER_H
#include <QThread>
#include "traderbusiness_global.h"

namespace Common
{
	class TRADERBUSINESS_EXPORT ThreadManager: public QObject
	{
		Q_OBJECT
	public:
		static ThreadManager* getInstance();
		QThread* getViewModelThread();
		QThread* getTimerThread();
		QThread* getBusinessThread();
		QThread* getChartThread();
		QThread* getTimeThread();

		void stop();

	private:
		void stopThread(QThread& thread);
	private:
		ThreadManager();
		static ThreadManager* _instance;
		QThread _businessThread;	
		QThread _viewModelThread;
		QThread _timerThread;
		QThread _chartThread;
		QThread _timeThread;
	};
}
#endif

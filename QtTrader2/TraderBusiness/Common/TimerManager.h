#ifndef COMMON_TIMERMANAGER_H
#define COMMON_TIMERMANAGER_H
#include <QMap>
#include <QUuid>
#include <QSharedPointer>
#include <QTimer>
#include <qdebug.h>
#include "traderbusiness_global.h"

namespace Common
{
	class TRADERBUSINESS_EXPORT TimerManager: public QObject
	{	
		Q_OBJECT
	public:
		static TimerManager* getInstance();
		void setUpdateInterval(int updateInterval);
                void start();
		void clear();
		QTimer* getViewModelTimer();

   signals:
		void startTiemrSignal();
		void stopTimerSignal();

	public slots:
		void startTimerSlot();
		void stopTimerSlot();
	private:
		TimerManager();
		~TimerManager();
		static TimerManager* _instance;
		QTimer _viewModeltimer;
	};
}
#endif

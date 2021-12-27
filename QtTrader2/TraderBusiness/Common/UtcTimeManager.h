#ifndef COMMON_UtcTimeManager_H
#define COMMON_UtcTimeManager_H
#include <QSharedPointer>
#include <QTimer>
#include <QDateTime>
#include "Common/Result/Result.h"
#include "traderbusiness_global.h"

namespace Common
{
	class TRADERBUSINESS_EXPORT UtcTimeManager: public QObject
	{	
		Q_OBJECT
	public:
		static UtcTimeManager* getInstance();
		QDateTime getUtcNow();
		void addTime();
		void startServerDateTime(qint64 serverToClientMilliseconds);
		void stop();

	private:
		UtcTimeManager();
		~UtcTimeManager();
		static UtcTimeManager* _instance;
		QDateTime _startNow;
		QTimer* _timer;
		qint64 _serverToClientMilliseconds;
		QSharedPointer<Common::Result::InvokeResult> m_ServerDateTimeResult;

	public slots:
		void onTimerOut();
		void getServerDateTimeResultCompletedSlot();

	};
}
#endif

#ifndef COMMUNICATION_HEARTBEATER_H
#define COMMUNICATION_HEARTBEATER_H

#include <QObject>
#include <QSharedPointer>

#include "Package.h"
#include "Common/Constants/CommunicationConstants.h"
#include "Common/Enum/CommunicationEnum.h"
#include <QDateTime>
#include <QMap>
#include <QTimer>

namespace Communication
{
    class SslChannel;
	class Heartbeater : public QObject
	{
		friend class SslChannel;

		Q_OBJECT		

	private:
        Heartbeater(SslChannel* ownerSslChannel, bool sendHeartbeat = true);        
		void stop();
		void checkTimeout(QHash<QString, QDateTime>& asyncHeartbeaterTimeRecord, int beatInterval, qint64 currentMSecs);
		void innerProcess(QSharedPointer<Package>& package);
		QString beatRequest();

    public:
        ~Heartbeater();

	private slots:
		void process(QSharedPointer<Package>& package);
		void beat();
		void checkTimeoutSlot();

	signals:
		void statusChanged(Common::Enum::BeatStatus::BeatStatus status);
        void stopBeatTimerSignal();
        void stopCheckTimeoutTimerSignal();

	private:
		QTimer* beatTimer;
		QTimer* checkTimeoutTimer;
        SslChannel* _ownerSslChannel;

		int beatInterval;
		int beatIntervalOnNotReceiveCommand;
		int beatIntervalOnReceiveCommand;
		int beatTimeoutInterval;
		int continuityHeartbeaterTimeoutCount;
		int waitTime;
		int failedCount;
		QDateTime _lastHeartbeaterSuccessTimeRecord;
		QHash<QString, QDateTime> _asyncHeartbeaterTimeRecord;
		QHash<QString, QDateTime> _asyncHeartbeaterLongTimeRecord;
		bool _sendHeartbeat;
	};
}
#endif

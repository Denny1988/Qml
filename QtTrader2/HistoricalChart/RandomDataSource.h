#pragma once;
#include "RealtimeDataSource.h"
#include "BORealtimeDataSource.h"
#include "HistoricalDataSource.h"
#include "historicalchart_global.h"
#include "Common.h"

class QTimer;
namespace HistoricalChartSpace
{
	class HISTORICALCHART_EXPORT RandomDataSource : public HistoricalDataSource
	{
		Q_OBJECT
	public:
		RandomDataSource();

		virtual void beginGetHistoricalData(const ChartProperty& chartProperty, double endTime, int size, QSharedPointer<GetChartDataCallback> callback) override;
		virtual void beginGetFixPeriodData(const ChartProperty& chartProperty, double beginTime, double endTime, QSharedPointer<GetChartDataCallback> callback) override;
		virtual void beginGetFixPeriodData(const ChartProperty& chartProperty, int periodType, QSharedPointer<GetChartDataCallback> callback) override;
		virtual void beginGetLastBars(const ChartProperty& chartProperty, double lastDataDate, QSharedPointer<GetLastBarCallback> callback) override;

		virtual void getRealTimeData(QList<PrimRealTimeData> *datas);
		virtual void beginGetCurrentTime(QSharedPointer<GetSystemTimeCallback> callback) override;
		virtual double GetPeriodBeginTime(int rangeType) override;
		virtual double GetPeriodBeginTime(int periodType, const QString& instrumentId) override;

        #if defined(RUNTIME_OS_IPHONE) || defined(RUNTIME_OS_ANDROID)
        //mobile
        virtual MobileMarketQuotation getMobileMarketQuotation(QString instumentIdStr) override;
        #endif

		void setTimeInterval(int interval);
		void setBasePrice(int basePrice);
		void setVolatilityRange(int volatilityRange);

	public slots:
		void NewDataComeSlot();

	private:
		void init();
		void genHistoricalData(PrimHistoricalData* data) const;
		void genRealTimeData(PrimRealTimeData& data) const;
		void adjustBegin(double& begin, const Frequency& frequency);

	private slots:
		//void getCurrentTimeSlot(int callId);

		virtual void beginGetQuotePolicies(QString instumentId, QSharedPointer<GetQuotePoliciesCallback> callback) override;

	private:
		QTimer* m_pTimer;
		int m_basePrice;
		int m_volatilityRange;
		PrimHistoricalData m_lastBar;
	};

	HISTORICALCHART_EXPORT RandomDataSource*  GetDataSource();

	class RandomRealtimeDataSource : public RealtimeDataSource
	{
		Q_OBJECT
	public:
		RandomRealtimeDataSource();
		virtual void beginGetInitialData();

	public slots:
		void NewDataComeSlot();

	private:
		void genRealTimeData(PrimRealTimeData& data) const;

	private:
		QTimer* m_pTimer;

	};

	HISTORICALCHART_EXPORT RealtimeDataSource*  GetRealtimeDataSource();

	class RandomBORealtimeDataSource : public BORealtimeDataSource
	{
		Q_OBJECT
	public:
		RandomBORealtimeDataSource();
		virtual void beginGetInitialData();

	public slots:
		void NewDataComeSlot();

	private:
		void genRealTimeData(PrimBORealTimeData& data) const;

	private:
		QTimer* m_pTimer;

	};

	HISTORICALCHART_EXPORT BORealtimeDataSource*  GetBORealtimeDataSource();
}


 

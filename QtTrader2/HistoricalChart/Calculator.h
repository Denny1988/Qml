#pragma  once
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QQueue>

namespace HistoricalChartSpace
{
	class Indicator;
	class HistoricalChart;
	struct CalculatorTask
	{
		Indicator* indicator;
		int beginIndex;
		int endIndex;
	};

	class Calculator : public QThread
	{
	public:
		Calculator(HistoricalChart* pView);
		~Calculator();

	public:
		void addEndTask();
		void beginCalculate(Indicator *indicator, int beginIndex, int endIndex);
		void beginCalculateAll(Indicator *indicator);
		void removeTasks(Indicator* indicator);
		void stop();

	protected:
		virtual void run() override;

	private:
		QMutex m_queueMutex;
		QWaitCondition m_waitCondition;
		QQueue<CalculatorTask>	m_taskQueue;
		HistoricalChart* m_pView;
		bool m_isRunning;
	};
}
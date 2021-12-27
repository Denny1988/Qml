#include "Calculator.h"
#include "Indicator/Indicator.h"
#include "historicalchart.h"
#include <QCoreApplication>
#include <QDebug>
namespace HistoricalChartSpace
{
	Calculator::Calculator(HistoricalChart* pView) : m_pView(pView), m_isRunning(true)
	{
		QThread::start();
        qDebug() << "HistoricalChartSpace::Calculator:  QThread::start();";
	}

	Calculator::~Calculator()
	{
		QThread::exit();
	}

	void Calculator::stop()
	{
		m_isRunning = false;
#ifndef RUNTIME_OS_MACX
        m_waitCondition.wakeOne();
        QThread::exit();
        QThread::wait();
#else
        try
        {
            qDebug() << "HistoricalChartSpace::Calculator::stop() begin....";
            m_waitCondition.wakeOne();
            m_waitCondition.notify_all();
            QThread::exit();
            QThread::wait();
            qDebug() << "HistoricalChartSpace::Calculator::stop() finish.";
        }
        catch(...)
        {
            qDebug() << "HistoricalChartSpace::Calculator::stop() Error.";
        }
#endif
	}

	void Calculator::run()
	{
		while (m_isRunning)
		{
			CalculatorTask task;
			{
				QMutexLocker locker(&m_queueMutex);
				if (m_taskQueue.isEmpty())
				{
					m_waitCondition.wait(&m_queueMutex);
					if (!m_isRunning)
					{
						break;
					}
				}
				task = m_taskQueue.dequeue();
			}
			if (task.indicator)
			{
				try
				{
					if (task.endIndex > 0)
					{
						if (task.beginIndex == task.endIndex && task.beginIndex > 0)
						{
                            // because bug of TASDK(Found by testing SMA).
                            task.beginIndex--;
						}

						if (task.endIndex >= task.indicator->m_baseData.size())
						{
                            // This may happen when switching Instrument.
							task.endIndex = task.indicator->m_baseData.size() - 1;
							if (task.beginIndex > task.endIndex)
							{
								task.beginIndex = task.endIndex;
							}
						}
						task.indicator->calculate(task.beginIndex, task.endIndex);
					}
					else
					{
						task.indicator->calculateAll();
					}
				}
				catch (...)
				{

				}
			}
			else
			{
				// task.indicator == null express end of a batch tasks, we should notify UI to refresh now.
				QCoreApplication::postEvent(m_pView, new QKeyEvent(QEvent::KeyPress, Qt::Key_F5, Qt::NoModifier));
				//QCoreApplication::postEvent(task.pView, new QEvent(QEvent::Paint));
				//qDebug() << "sent Paint event to pView.";

				// sleep to control performance
				this->msleep(500);
			}
		}
	}

	void Calculator::beginCalculate(Indicator* indicator, int beginIndex, int endIndex)
	{
		QMutexLocker locker(&m_queueMutex);

		// Merge task
		bool isMerged = false;
		if (indicator)
		{
			for (int i = 0; i < m_taskQueue.count(); i++)
			{
				if (m_taskQueue[i].indicator == indicator)
				{
					if (m_taskQueue[i].beginIndex > beginIndex)
					{
						m_taskQueue[i].beginIndex = beginIndex;
					}
					if (m_taskQueue[i].endIndex < endIndex)
					{
						m_taskQueue[i].endIndex = endIndex;
					}
					isMerged = true;
					break;
				}
			}
		}
		else
		{
			// Merge EndTask
			if (m_taskQueue.count() > 0 && m_taskQueue.last().indicator == nullptr)
			{
				isMerged = true;
			}
		}


		if (!isMerged)
		{
			CalculatorTask task = { indicator, beginIndex, endIndex };
			m_taskQueue.enqueue(task);
			m_waitCondition.wakeOne();
		}
	}

	void Calculator::beginCalculateAll(Indicator *indicator)
	{
		this->beginCalculate(indicator, 0, -1);
	}

	void Calculator::addEndTask()
	{
		this->beginCalculate(nullptr, 0, 0);
	}

	void Calculator::removeTasks(Indicator* indicator)
	{
		QMutexLocker locker(&m_queueMutex);
		for (int i = m_taskQueue.count() - 1; i >= 0; i--)
		{
			if (m_taskQueue[i].indicator == indicator)
			{
				m_taskQueue.removeAt(i);
			}
		}
	}
}

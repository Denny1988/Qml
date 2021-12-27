#include "PriceAlertAdapter.h"
#include "AlertLine.h"
#include "historicalchart.h"
#include "PricePanel.h"
#include <QCoreApplication>

namespace HistoricalChartSpace
{

	PriceAlertAdapter::PriceAlertAdapter(HistoricalChart* pView) : m_pView(pView)
	{
	}

	PriceAlertAdapter::~PriceAlertAdapter()
	{
	}

	void PriceAlertAdapter::appendAlert(const PriceAlertStruct& alert)
	{
        //_ASSERT(!m_alertLines.contains(alert.ID));
		QSharedPointer<AlertLine> alertLine(new AlertLine(m_pView->getPricePanel(), this, alert));
		m_alertLines.insert(alert.ID, alertLine);
	}

	void PriceAlertAdapter::udpateAlert(const PriceAlertStruct& alert)
	{
        //_ASSERT(m_alertLines.contains(alert.ID));
		m_alertLines[alert.ID]->updateParameter(alert);
	}

	void PriceAlertAdapter::removeAlert(const QUuid& alertId)
	{
		if (m_alertLines.contains(alertId))
		{
			m_alertLines.remove(alertId);
			m_pView->getPricePanel()->getMainPainterPanel()->update();
		}
	}

	void PriceAlertAdapter::removeAlerts(const QList<QUuid>& alertIds)
	{
        foreach (QUuid alertId, alertIds)
		{
			this->removeAlert(alertId);
		}
		m_pView->getPricePanel()->getMainPainterPanel()->update();
	}

	void PriceAlertAdapter::clearAlerts()
	{
		m_alertLines.clear();
	}

	void PriceAlertAdapter::updateUI()
	{
        foreach (QSharedPointer<AlertLine> line, m_alertLines.values())
		{
			line->ensureInView();
		}
	}
}

#include "moc_PriceAlertAdapter.cpp"

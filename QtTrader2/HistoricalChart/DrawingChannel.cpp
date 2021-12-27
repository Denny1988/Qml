#include "DrawingChannel.h"
#include "ChartPanel.h"

namespace HistoricalChartSpace
{

	DrawingChannel::DrawingChannel(const DrawingObjectSettings& settings, ChartPanel* parent, DrawStage state)
		: DrawingTrend(settings, parent, state), m_secondLine(nullptr)
	{
		m_secondLine = new QGraphicsLineItem(m_parentPanel->getMainPainterPanel());
	}

	DrawingChannel::~DrawingChannel()
	{
		if (m_secondLine)
		{
			delete m_secondLine;
		}
	}

    void DrawingChannel::setSecondLineIsVisible(bool isVisible)
    {
        m_secondLine->setVisible(isVisible);
    }

	void DrawingChannel::drawLine()
	{
        //_ASSERT(m_vecNode.size() == 3);
		DrawingTrend::drawLine();

		QPointF beginPos = m_vecNode[2]->pos();
		QPointF endPos = beginPos - m_vecNode[0]->pos() + m_vecNode[1]->pos();

		this->computeRayPos(beginPos, endPos);

		QPen pen(m_settings.m_colorLine, m_settings.m_lfLineWidth, (Qt::PenStyle)m_settings.m_intLineType);
		m_secondLine->setPen(pen);
		m_secondLine->setLine(QLineF(beginPos, endPos));
	}

	void DrawingChannel::updateCenterNode()
	{
        //_ASSERT(m_vecNode.size() == 3);
		const QPointF& beginPos = m_vecNode[0]->pos();
		const QPointF& endPos = m_vecNode[1]->pos();

		QPointF centerPos = (beginPos + endPos) / 2;
		m_centerNode->setPos(centerPos);
	}

	void DrawingChannel::hindToPos(const QPointF& scenePosPoint)
	{
		if (m_stage == DrawFirstStep)
		{
			QPointF localPoint = m_parentPanel->mapFromScene(scenePosPoint);
			m_vecNode[1]->setPos(localPoint);
			DrawingTrend::drawLine();
			m_vecNode[1]->hide();
		}
		else if (m_stage == DrawSecondStep)
		{
			m_vecNode[1]->show();
			QPointF localPoint = m_parentPanel->mapFromScene(scenePosPoint);
			m_vecNode[2]->setPos(localPoint);
			this->drawLine();
			m_vecNode[2]->hide();
		}
		else if (m_stage >= DrawThirdStep)
		{
			m_vecNode[2]->show();
			m_parentPanel->getMainPainterPanel()->setAcceptHoverEvents(false);
		}
	}

}

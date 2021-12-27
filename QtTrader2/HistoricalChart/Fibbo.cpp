#include "Fibbo.h"
#include "ChartPanel.h"

namespace HistoricalChartSpace
{
	Fibbo::Fibbo(const DrawingObjectSettings& settings, ChartPanel* parent, DrawStage state) : DrawingTrend(settings, parent, state)
	{
	}

	Fibbo::~Fibbo()
	{
		foreach(QGraphicsLineItem* line, m_listLine)
		{
			delete line;
		}
		foreach(QGraphicsSimpleTextItem* text, m_listText)
		{
			delete text;
		}

		m_listText.clear();
		m_listLine.clear();
	}

	void Fibbo::drawLine()
	{
		if (m_vecNode.size() != 2) return;
		const QPointF& beginPos = m_vecNode[0]->pos();
		const QPointF& endPos = m_vecNode[1]->pos();
		const QRectF rect = m_parentPanel->getMainPainterPanel()->boundingRect();

		int mapSize = m_settings.m_mapLevels.size();
		int listSize = m_listLine.size();

		if (!m_vecNode.at(0)->isVisible())
		{
			for (int i = 0; i < listSize; ++i)
			{
				if (m_listLine[i]->isVisible())
				{
					m_listLine[i]->setVisible(false);
				}

				if (m_listText[i]->isVisible())
				{
					m_listText[i]->setVisible(false);
				}
			}
			return;
		}

        // Draw control points
		m_lineElement->setLine(QLineF(beginPos, endPos));

        // To draw a Fibbo line, first determine whether the line in the container is enough, and then not create it, otherwise create a new line

		if (listSize < mapSize)
		{
			for (int i = 0; i < mapSize - listSize; ++i)
			{
				QGraphicsSimpleTextItem* text = new QGraphicsSimpleTextItem(m_parentPanel->getMainPainterPanel());
				QGraphicsLineItem* line = new QGraphicsLineItem(m_parentPanel->getMainPainterPanel());
				m_listLine.append(line);
				m_listText.append(text);

			}
		}

        // Draw specific Fibbo lines
        //Calculate the ratio
		double yScale = endPos.y() - beginPos.y();

		QMapIterator<double, QString> it(m_settings.m_mapLevels);
		for (int i = 0; it.hasNext(); ++i)
		{
			it.next();
			double y = yScale * it.key() + beginPos.y();
            //Configure the brush
			QPen pen(m_settings.m_levelsColorLine, m_settings.m_levelsLineWidth, (Qt::PenStyle)m_settings.m_levelsLineType);
			m_listLine[i]->setPen(pen);
			m_listText[i]->setBrush(m_settings.m_levelsColorLine);
            //Draw a line
			m_listLine[i]->setLine(beginPos.x(), y, endPos.x(), y);

            //Draw text
			double val = m_parentPanel->Y2Value(y);
			QString text = QString("%1 (%2)").arg(val).arg(it.value());
			QFont font;
			font.setFamily(font.defaultFamily());
			font.setPointSize(9);
			m_listText[i]->setFont(font);
			m_listText[i]->setText(text);
			qreal textPos = endPos.x() > beginPos.x() ? endPos.x() : beginPos.x();
			m_listText[i]->setPos(textPos - 15, y - 15);

            // Set visibility
			m_listLine[i]->setVisible(true);
			m_listText[i]->setVisible(true);
		}

		for (int i = mapSize; i < listSize; ++i)
		{
			m_listLine[i]->setVisible(false);
			m_listText[i]->setVisible(false);
		}
	}

    void Fibbo::setLineAndTextIsVisible(bool isVisible)
    {
        foreach(QGraphicsLineItem* line, m_listLine)
        {
            line->setVisible(isVisible);
        }
        foreach(QGraphicsSimpleTextItem* text, m_listText)
        {
            text->setVisible(isVisible);
        }
    }
}

#include "IndicatorPanel.h"
#include "SplitPanel.h"
#include "Config.h"
#include "JulianDate.h"
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>
#include <QStyleOptionGraphicsItem>
#include "MainPainterPanel.h"
#include "YCoorPanel.h"
#include "PricePanel.h"
#include "Helper.h"
#include "../QtFramework/QsLog.h"

namespace HistoricalChartSpace
{
	// create from config
	IndicatorPanel::IndicatorPanel(HistoricalChart* pView, Config& config, BaseData& baseData) :ChartPanel(pView, config, baseData)
	{
		m_PanelType = enPanelIndicator;
	}

	// create from menu
	IndicatorPanel::IndicatorPanel(HistoricalChart* pView, Config& config, BaseData& baseData, Indicator* indicator) :ChartPanel(pView, config, baseData)
	{
		m_PanelType = enPanelIndicator;

		// add default indicator
		m_indicators.append(indicator);
	}

	IndicatorPanel::~IndicatorPanel()
	{
	}

	void IndicatorPanel::paintMainPanel(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
	{
		if (m_baseData.size() == 0) return;

		const QRectF& rectFull = option->exposedRect;
		QRectF rect = rectFull;

        painter->setClipRect(rect);

//#if defined(RUNTIME_OS_IPHONE) || defined(RUNTIME_OS_ANDROID)
//		QPen penText(m_commonSettings.m_foreground);
//		painter->setPen(penText);
//		const double leftMargin = Helper::GetMarginSpaceValue(enMarginTextInfoToLeft);
//		const double topMargin = Helper::GetMarginSpaceValue(enMarginTextInfoToTop);
//		const double textHight = Helper::GetMarginSpaceValue(enMarginIndicatorTextInfoH);
//		const double textWidth = Helper::GetMarginSpaceValue(enMarginIndicatorTextInfoW);
//		painter->drawText(rect.left() + leftMargin, rect.top() + topMargin, textWidth, textHight, Qt::AlignLeft | Qt::AlignTop, m_indicators[0]->getTitle());
//#endif
		this->paintIndicators(painter);

		updateAllDrawingObject();
	}

	void IndicatorPanel::paintYCoorPanel(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
	{
		const QRectF& rect = option->exposedRect;
		const double width = Helper::GetMarginSpaceValue(enMarginYCoordinate);
		const double outGrid = Helper::GetMarginSpaceValue(enMarginOutGridStandardSpace);
		const double yTextH = Helper::GetMarginSpaceValue(enMarginYTextHSpace);
		const double yTextW = Helper::GetMarginSpaceValue(enMarginYTextWSpace);
		const double yTexpSpace = Helper::GetMarginSpaceValue(enMarginSpaceTextToCoordinate);

		bool isRsi = m_indicators[0]->getIndicatorSettings().m_type == enIndicatorRelativeStrengthIndex;
		int decimalPlaces = m_indicators[0]->getIndicatorSettings().m_decimalPlaces;
		double tinyShit = isRsi ? 0.0 : Helper::GetMarginSpaceValue(enMarginYcoodinateTinyShift);
		double valueShit = (m_bottomMargin - tinyShit) / (m_PaintArgs.scaleY == 0 ? 1.0 : m_PaintArgs.scaleY);

		double overMax = m_maxValue + valueShit;
		double belowMin = m_PaintArgs.minValue - valueShit;

		QPointF pointTopBegin(rect.left(), rect.top() + tinyShit);
		QPointF pointTopEnd(rect.left() + outGrid, rect.top() + tinyShit);
		QPointF pointLowBegin(rect.left(), rect.top() + rect.height() - tinyShit);
		QPointF pointLowEnd(rect.left() + outGrid, rect.top() + rect.height() - tinyShit);

		QPen pen(m_commonSettings.m_foreground);
		painter->setPen(pen);

		painter->drawLine(pointTopBegin, pointTopEnd);
		painter->drawText(pointTopEnd.x() + yTexpSpace, pointTopEnd.y(), yTextW, yTextH, Qt::AlignLeft | Qt::AlignTop, QString::number(overMax, 'f', decimalPlaces));

		bool isMacd = m_indicators[0]->getIndicatorSettings().m_type == enIndicatorMACD;
		if (isMacd)
		{
			double y = this->Value2Y(0);
			QPointF pointZeroBegin(rect.left(), y);
			QPointF pointZeroEnd(rect.left() + outGrid, y);
			painter->drawLine(pointZeroBegin, pointZeroEnd);
			painter->drawText(pointZeroEnd.x() + yTexpSpace, y, yTextW, yTextH, Qt::AlignLeft | Qt::AlignTop, QString::number(0.0, 'f', decimalPlaces));
		}

		painter->drawLine(pointLowBegin, pointLowEnd);
		painter->drawText(pointLowEnd.x() + yTexpSpace, pointLowEnd.y() - yTextH, yTextW, yTextH, Qt::AlignLeft | Qt::AlignBottom, QString::number(belowMin, 'f', decimalPlaces));
	}

	void IndicatorPanel::refresh()
	{
		const QRectF& rect = m_mainPainterPanel->boundingRect();
		if (m_baseData.size() > 0 && m_indicators.size() > 0)
		{
			bool isRsi = m_indicators[0]->getIndicatorSettings().m_type == enIndicatorRelativeStrengthIndex;
			m_maxValue = isRsi ? 100.0 : calculateMaxValue();
            if (!std::isnan(m_maxValue))
			{
				m_PaintArgs.minValue = isRsi ? 0.0 : calculateMinValue();
				m_bottomMargin = isRsi ? 0.0 : Helper::GetMarginSpaceValue(enMarginValueToBondSpaceScale) * rect.height();
				m_PaintArgs.minY = rect.top() + rect.height() - m_bottomMargin;
				m_PaintArgs.scaleY = 1;
				if (m_maxValue != m_PaintArgs.minValue)
				{
					m_PaintArgs.scaleY = (rect.height() - 2 * m_bottomMargin) / (m_maxValue - m_PaintArgs.minValue);
				}
				else
				{
					m_PaintArgs.scaleY = std::numeric_limits<double>::max();
				}
				if (m_PaintArgs.scaleY <= 0)
				{
					m_PaintArgs.scaleY = 1;
				}
			}
		}
		ChartPanel::updateUI();
	}
}


#include "moc_IndicatorPanel.cpp"

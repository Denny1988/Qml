#include "PriceStyle.h"
#include "PricePanel.h"
#include <QPainter>
#include <QDebug>

using namespace HistoricalChartSpace;

PriceBarStyle::PriceBarStyle(PricePanel* parent) :PriceStyle(parent)
{
	m_type = enPriceBar;
}

void PriceBarStyle::drawPrice(QPainter* painter, QRectF& rect, double barDistance)
{
	int begin = m_pricePanel->m_baseData.getBeginIndex();
	double rectY = rect.top() + rect.height();
	double bar = Helper::GetMarginSpaceValue(enMarginPriceBarLength);
	double penWidth = m_pricePanel->m_commonSettings.m_zoomLevel >= zoom_normal ? 2.0 : 1.0;

	QPen penUp(m_pricePanel->m_priceSettings.m_barUpColor, penWidth);
	QPen penDown(m_pricePanel->m_priceSettings.m_barDownColor, penWidth);

	int endIndex = m_pricePanel->m_baseData.getEndIndex();
	for (int i = begin; i <= endIndex; ++i)
	{
		double  X = rect.left() + (i - begin)  * barDistance;

		double openY = m_pricePanel->Value2Y(m_pricePanel->m_baseData.getOpens()[i]);
		double closeY = m_pricePanel->Value2Y(m_pricePanel->m_baseData.getCloses()[i]);
		double highY = m_pricePanel->Value2Y(m_pricePanel->m_baseData.getHighs()[i]);
		double lowY = m_pricePanel->Value2Y(m_pricePanel->m_baseData.getLows()[i]);

		//Open shift left and Close shift right
		QPointF pointOpenLeft(X - bar, openY);
		QPointF pointOpen(X, openY);
		QPointF pointClose(X, closeY);
		QPointF pointCloseRight(X + bar, closeY);

		if (closeY > openY)
		{
			painter->setPen(penDown);
		}
		else
		{
			painter->setPen(penUp);
		}

		painter->drawLine(pointOpenLeft, pointOpen);
		painter->drawLine(X, highY, X, lowY);
		painter->drawLine(pointClose, pointCloseRight);
	}
}

PriceLineStyle::PriceLineStyle(PricePanel* parent) :PriceStyle(parent)
{
	m_type = enPriceLine;
}

void PriceLineStyle::drawPrice(QPainter* painter, QRectF& rect, double barDistance)
{
	/*painter->setRenderHint(QPainter::Antialiasing);*/
	double penWidth = m_pricePanel->m_commonSettings.m_zoomLevel >= zoom_normal ? 2.0 : 1.0;
	QPen pen(m_pricePanel->m_priceSettings.m_lineChartColor, penWidth);
	painter->setPen(pen);

	QPointF lastPoint;
	int begin = m_pricePanel->m_baseData.getBeginIndex();
	int endIndex = m_pricePanel->m_baseData.getEndIndex();
	for (int i = begin; i <= endIndex; ++i)
	{
		double  X = rect.left() + (i - begin)  * barDistance;
		double  Y = m_pricePanel->Value2Y(m_pricePanel->m_baseData.getCloses()[i]);
		QPointF point(X, Y);
		if (i > begin)
		{
			painter->drawLine(lastPoint, point);
		}
		lastPoint = point;
	}
}

PriceCandlestickStyle::PriceCandlestickStyle(PricePanel* parent):PriceStyle(parent)
{
	m_type = enPriceCandlesticks;
}

/*
  rect:       rectangle for paint
  barDistance:     width(pixels) per bar
*/
void PriceCandlestickStyle::drawPrice(QPainter* painter, QRectF& rect, double barDistance)
{
	int begin = m_pricePanel->m_baseData.getBeginIndex();
	const double shiftScale = Helper::GetMarginSpaceValue(enMarginCandlestickSpaceScale);

	double rectY = rect.top() + rect.height();
	double barWidth = shiftScale * barDistance;

	//static int si = 0;
	//qDebug() << "Begin:" << begin << "  End:" << m_pricePanel->m_PaintArgs.endIndex << ". -" << si++;
	// draw test horizontal lines:
	//double y20 = m_pricePanel->Value2Y(20);
	//double y30 = m_pricePanel->Value2Y(30);
	//painter->drawLine(rect.x(), y20, rect.x() + rect.width(), y20);
	//painter->drawLine(rect.x(), y30, rect.x() + rect.width(), y30);

	int endIndex = m_pricePanel->m_baseData.getEndIndex();
	for (int i = begin; i <= endIndex; ++i)
	{
		double  X = rect.left() + (i - begin)  * barDistance;

		double openY = m_pricePanel->Value2Y(m_pricePanel->m_baseData.getOpens()[i]);
		double closeY = m_pricePanel->Value2Y(m_pricePanel->m_baseData.getCloses()[i]);
		double highY = m_pricePanel->Value2Y(m_pricePanel->m_baseData.getHighs()[i]);
		double lowY = m_pricePanel->Value2Y(m_pricePanel->m_baseData.getLows()[i]);

		if (closeY > openY)
		{
            // Draw shadow line
			QPen pen(m_pricePanel->m_priceSettings.m_bearCandleColor);
			double penWidth = pen.widthF();
			painter->setPen(pen);
			if (openY != highY) painter->drawLine(X, openY, X, highY);
			if (closeY != lowY) painter->drawLine(X, closeY, X, lowY);
			painter->setBrush(m_pricePanel->m_priceSettings.m_bearCandleColor);
			painter->drawRect(X - (barWidth - penWidth) / 2, openY, barWidth, closeY - openY);
		}
		else if (openY == closeY)
		{
            // Draw doji
			QPen pen(m_pricePanel->m_priceSettings.m_crossStarColor);
			double penWidth = pen.widthF();
			painter->setPen(pen);
			painter->drawLine(X, lowY, X, highY);
			painter->drawLine(X - (barWidth - penWidth) / 2, openY, X + barWidth / 2, openY);
		}
		else
		{
            // Draw a sun line
			QPen pen(m_pricePanel->m_priceSettings.m_bullCandleColor);
			double penWidth = pen.widthF();
			painter->setPen(pen);
			if (closeY != highY) painter->drawLine(X, closeY, X, highY);
			if (openY != lowY) painter->drawLine(X, openY, X, lowY);
			painter->setBrush(m_pricePanel->m_priceSettings.m_bullCandleColor);
			painter->drawRect(X - (barWidth - penWidth) / 2, closeY, barWidth, openY - closeY); 
		}
	}
}


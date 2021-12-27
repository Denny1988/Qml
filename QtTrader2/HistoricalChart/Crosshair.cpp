#include "Crosshair.h"
#include "historicalchart.h"
#include "CursorPosition.h"
#include "PricePanel.h"
#include <QDebug>

namespace HistoricalChartSpace
{
	Crosshair::Crosshair(HistoricalChart* pView, CursorPosition* cursorPosition) : m_pView(pView), m_cursorPosition(cursorPosition)
	{
		pView->scene()->addItem(this);
		this->setZValue(100);
	}

	QRectF Crosshair::boundingRect() const
	{
		return m_pView->getViewPortRect();
	}

	QPainterPath Crosshair::shape() const
	{
		QPainterPath path;
		path.addRect(boundingRect());
		return path;
	}

	void Crosshair::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
	{
		const double marginYCoor = Helper::GetMarginSpaceValue(enMarginYCoordinate);
		const double marginXCoor = Helper::GetMarginSpaceValue(enMarginXCoordinate);
		const double outMargin = Helper::GetMarginSpaceValue(enMarginOutGridStandardSpace);

		QRectF rect = boundingRect();
		rect.setHeight(rect.height() - marginXCoor);
		rect.setWidth(rect.width() - marginYCoor);

		// QPoint tempPoint = this->cursor().pos();

		QPointF point = m_cursorPosition->getScenePos(); //m_pView->mapToScene(m_pView->mapFromGlobal(tempPoint));
		//const QPointF& point2 = m_cursorPosition->getScenePos();
		//qDebug() << QString("[%1,%2] [%3,%4]").arg(point.x()).arg(point.y()).arg(point2.x()).arg(point2.y());
		//QPointF pointOnPricePanel = m_pView->m_pricePanel->mapFromScene(point);
		point = this->mapFromScene(point);

		if (!rect.contains(point))
		{
			this->setCursor(Qt::ArrowCursor);
			return;
		}

		//this->setCursor(Qt::BlankCursor);
		const CommonSettings& settings = m_pView->m_config.getCommonSettings();

		painter->setPen(settings.m_foreground);

        // TODO: A cross cursor with reverse color can be drawn at the cursor position (similar to MT4)
        // 1. Draw a false cross cursor (real cursor movement will cause a delay)

        // 2.Draw crosshair
        //  2 horizontal lines with +-2 4 pixels in the middle
		painter->drawLine(rect.left(), point.y(), point.x() - 2, point.y());
		painter->drawLine(point.x() + 2, point.y(), rect.left() + rect.width() + outMargin, point.y());

        //  2 vertical lines with +-2 4 pixels in the middle
		painter->drawLine(point.x(), rect.top(), point.x(), point.y() - 2);
		painter->drawLine(point.x(), point.y() + 2, point.x(), rect.top() + rect.height() + outMargin);

        // 3.Draw display data background
		QFontMetrics fontMetircs(painter->font());
		const double textHeight = fontMetircs.height();
		painter->setBrush(settings.m_foreground);

        // 4.Calculation data and display
		QString date = m_cursorPosition->getDateText();

		painter->setPen(settings.m_background);

		double textWidth = fontMetircs.width(date) + 2;
		QRect textRectX(point.x() - textWidth / 2, rect.top() + rect.height() + outMargin, textWidth, textHeight);
		if (textRectX.left() < 0) textRectX.moveLeft(0);
		painter->drawRect(textRectX);
		painter->drawText(textRectX, Qt::AlignCenter, date);

		QString YValue = m_cursorPosition->getYValueText();
		textWidth = fontMetircs.width(YValue) + 2;
		QRect textRectY(rect.left() + rect.width() + outMargin, point.y() - textHeight / 2, textWidth, textHeight);
		painter->drawRect(textRectY);
		painter->drawText(textRectY, Qt::AlignCenter, YValue);
		//qDebug() << "Xtext: " << date << "Ytext: " << YValue;
	}
}

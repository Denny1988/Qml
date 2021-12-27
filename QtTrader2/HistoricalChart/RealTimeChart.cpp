#include "RealTimeChart.h"
#include "SR.h"
#include <QPainter>
#include <QMoveEvent>
#include "JulianDate.h"
#include "RealTimeDialog.h"
#include <QMenu>
#include <QGuiApplication>
#include "Helper.h"
#include "Cursors.h"
namespace HistoricalChartSpace
{
	RealTimeChart::RealTimeChart(RealtimeDataSource* dataSource, RealTimePriceSettings settings, QWidget* parent)
		:QWidget(parent),
		m_dataSource(dataSource),
		m_gridUnitXLength(96),
		m_maxDataCount(5000),
		m_settings(settings),
		m_zoomIndex(1),
		m_adjustYScale(1),
		m_XAxisHeight(settings.m_isShowXaxis ? Helper::GetMarginSpaceValue(enMarginXCoordinate) : 1.0),
		m_YAxisWidth(Helper::GetMarginSpaceValue(enMarginYCoordinate))
	{
		if (m_settings.m_bDrawCrosshair)
		{
			this->setCursor(Qt::BlankCursor);
		}
		this->setMouseTracking(true);
		m_dataSource->OnRealTimeChartStarted(this);
		m_actionHandleSetting = new QAction(SR::get("FavoriteSettings"), this);
		connect(m_actionHandleSetting, SIGNAL(triggered()), SLOT(HandleSettingsSlot()));
		m_decimalPlaces = dataSource->getDecimalPlaces();
		m_dataUnitLengths.append(1);
		m_dataUnitLengths.append(2);
		m_dataUnitLengths.append(4);
		m_dataUnitLengths.append(6);
		m_dataUnitLengths.append(8);
	}

	RealTimeChart::~RealTimeChart()
	{
	}

	void RealTimeChart::showAskLineOnly()
	{
		m_settings.m_bDrawAsk = true;
		m_settings.m_bDrawbid = false;
		update();
	}

	void RealTimeChart::showBidLineOnly()
	{
		m_settings.m_bDrawAsk = false;
		m_settings.m_bDrawbid = true;
		update();
	}

	void RealTimeChart::showBidAskLines()
	{
		m_settings.m_bDrawAsk = true;
		m_settings.m_bDrawbid = true;
		update();
	}
	const RealTimePriceSettings& RealTimeChart::getSettings()
	{
		return m_settings;
	}

    void RealTimeChart::OnThemeChanged(bool userSetRealTimeChartColorSign, QColor bgColor, QColor fontColor)
    {
        if(!userSetRealTimeChartColorSign)
        {
            m_settings.m_colorBackground = bgColor;
            m_settings.m_coordinateBackground = bgColor;
            m_settings.m_colorForeground = fontColor;
            update();
        }
    }


	void RealTimeChart::RealTimeDataComeSlot(const PrimRealTimeData& data)
	{
        // 2016-03-16 (Liansheng: Only the changed price is displayed on the real-time chart)
		int lastIndex = m_listRealTimeData.count() - 1;
		if (lastIndex >= 0)
		{
			const PrimRealTimeData& lastData = m_listRealTimeData[lastIndex];
			if (lastData.bid == data.bid && lastData.ask == data.ask) return;
		}

		if (m_listRealTimeData.size() == m_maxDataCount)
		{
			m_listRealTimeData.pop_front();
		}
		m_listRealTimeData.append(data);
		update();
	}

	void RealTimeChart::UpdateRealTimeDataSlot(const QList<PrimRealTimeData> & listData)
	{
		m_listRealTimeData = listData;
		update();
	}

	void RealTimeChart::paintEvent(QPaintEvent * event)
	{
        // Initialize the background layer
		QRectF rect = this->rect();
		//const double m_XAxisHeight = m_settings.m_isShowXaxis ? Helper::GetMarginSpaceValue(enMarginXCoordinate) : 1.0;
		QPainter painter(this);
		painter.setPen(m_settings.m_coordinateBackground);
		painter.setBrush(m_settings.m_coordinateBackground);
		painter.drawRect(rect);

		rect.setWidth(rect.width() - m_YAxisWidth);
		rect.setHeight(rect.height() - m_XAxisHeight);
		painter.setPen(m_settings.m_colorForeground);
		painter.setBrush(m_settings.m_colorBackground);
		painter.drawRect(rect);

        // Draw grid
		if (m_settings.m_bDrawGrid)
		{
			this->drawGrid(painter, rect);
		}

        // If you have data, proceed to the next step of painting
		if (m_listRealTimeData.size() == 0) return;

        // Draw coordinates
        // 1. Calculate the number of elements
		m_verticalGridLineCount = rect.width() / m_gridUnitXLength;
		m_drawDataCount = m_verticalGridLineCount * m_gridUnitXLength / m_dataUnitLengths[m_zoomIndex] + 1;
		int listSize = m_listRealTimeData.size();
		if (m_drawDataCount > listSize) m_drawDataCount = listSize;

        // 2. Calculate maximum and minimum
		double maxValue;
		double minValue;
		this->getMaxMinValue(maxValue, minValue);

        // 3.Calculate the coordinate corresponding to the minimum value of the Y axis, and the conversion ratio between the Y coordinate and the value
		double differ = maxValue - minValue;
		differ = differ > 0 ? differ : 1;
		double reserveMargin = Helper::GetMarginSpaceValue(enMarginValueToBondSpaceScale) * rect.height();
		double height = rect.height() - 2 * reserveMargin;
		double scaleY = height * m_adjustYScale / differ;
		double minY = rect.top() + rect.height() - reserveMargin;
		minY = minY - height * (1 - m_adjustYScale) / 2;

        // Draw Y axis
		QRectF rectYCoor(rect.left() + rect.width(), rect.top(), m_YAxisWidth, rect.height());
		drawYCoordinate(painter, rectYCoor, minValue, minY, scaleY);
        // Draw X axis
		if (m_settings.m_isShowXaxis)
		{
			QRectF rectXCoor(rect.left(), rect.top() + rect.height(), rect.width(), m_XAxisHeight);
			this->drawXCoordinate(painter, rectXCoor);
		}
        // Draw ask line
		if (m_settings.m_bDrawAsk)
		{
			drawDataLine(painter, rect, minValue, minY, scaleY, drawAsk);
		}
        // Draw bid line
		if (m_settings.m_bDrawbid)
		{
			drawDataLine(painter, rect, minValue, minY, scaleY, drawBid);
		}
        // Draw crosshair
		if (m_settings.m_bDrawCrosshair)
		{
			drawCrosshair(painter, rect, minValue, minY, scaleY);
		}

		painter.setPen(m_settings.m_colorForeground);
		painter.setBrush(Qt::NoBrush);
		painter.drawRect(rect);
	}

	void RealTimeChart::mousePressEvent(QMouseEvent* event)
	{
		if (m_settings.m_isShowXaxis && event->buttons() == Qt::LeftButton && this->isInXCoorPanel(event->pos()) || this->isInYCoorPanel(event->pos()))
		{
			m_startMovePos = event->pos();
		}
	}

	void RealTimeChart::mouseMoveEvent(QMouseEvent *event)
	{
		const double YCoor = Helper::GetMarginSpaceValue(enMarginYCoordinate);
		const double XCoor = Helper::GetMarginSpaceValue(enMarginXCoordinate);
		QRect mainRect = this->rect();
		mainRect.setHeight(mainRect.height() - XCoor);
		mainRect.setWidth(mainRect.width() - YCoor);
		QPoint pos = event->pos();
		if (m_settings.m_bDrawCrosshair && mainRect.contains(pos))
		{
			QWidget::setCursor(Qt::BlankCursor);
			update();
		}
		else if (this->isInXCoorPanel(pos))
		{
			QPixmap pixmap(adjust_horizontal);
			QCursor cursor(pixmap, 0, 0);
			QWidget::setCursor(cursor);
		}
		else
		{
			QWidget::setCursor(Qt::ArrowCursor);
		}

		if (event->buttons() == Qt::LeftButton)
		{
			if (this->isInYCoorPanel(pos))
			{
				QRectF rect = this->rect();
				double moveRatio = (event->pos().y() - m_startMovePos.y()) / rect.height();
				double temp = m_adjustYScale - moveRatio;
				if (temp < 0)
				{
					m_adjustYScale = 0.01;
				}
				else if (temp > 1.0)
				{
					m_adjustYScale = 1.0;
				}
				else
				{
					m_adjustYScale = temp;
				}
				this->update();
				m_startMovePos = pos;
			}
			else if (m_settings.m_isShowXaxis && this->isInXCoorPanel(pos))
			{
				if (pos.x() > m_startMovePos.x())
				{
					this->zoomOut();
				}
				else
				{
					this->zoomIn();
				}
				m_startMovePos = pos;
			}
		}
		
		return QWidget::mouseMoveEvent(event);
	}

	void RealTimeChart::drawGrid(QPainter& painter, QRectF rect)
	{
		const double gridy = Helper::GetMarginSpaceValue(enMarginGridYStandardSpace);
		QPen pen(m_settings.m_colorGrid, 1, Qt::DotLine);
		painter.setPen(pen);
        // Draw horizontal lines
		double pos = gridy;
		for (int i = 1; pos < rect.height(); ++i)
		{

			double y = rect.top() + rect.height() - pos;
			QPoint p1(rect.left(), y);
			QPoint p2(rect.left() + rect.width(), y);
			painter.drawLine(p1, p2);
			pos = (i + 1) * gridy;
		}
        // Draw vertical lines
		pos = m_gridUnitXLength;
		for (int i = 1; pos < rect.width(); ++i)
		{
			double x = rect.left() + pos;
			QPoint p1(x, rect.top());
			QPoint p2(x, rect.top() + rect.height());
			painter.drawLine(p1, p2);
			pos = (i + 1)* m_gridUnitXLength;
		}
	}

	void RealTimeChart::drawXCoordinate(QPainter& painter, QRectF rect)
	{
		const double outMargin = Helper::GetMarginSpaceValue(enMarginOutGridStandardSpace);
		const double textH = Helper::GetMarginSpaceValue(enMarginXTextHSpace);
		const double textW = Helper::GetMarginSpaceValue(enMarginXTextWSpace);
		QPen pen(m_settings.m_colorForeground);
		painter.setPen(pen);

        // The first value is used to display the year, month and day, and the subsequent values are used to display the time
		int dataSize = m_listRealTimeData.size();
		double date = m_listRealTimeData[dataSize - m_drawDataCount].date;
		QString timeLabel = JulianDate::JDateItemToString(date, enShowYear | enShowMonth | enShowDay, 3);
		double x = rect.left();
		QPoint p1(x, rect.top());
		QPoint p2(x, rect.top() + outMargin);
		painter.drawLine(p1, p2);
		painter.drawText(x, rect.top() + outMargin, textW, textH, Qt::AlignLeft | Qt::AlignTop, timeLabel);

        // The latter is only used to display the time
		int verticalGridLineIndex;
		for (verticalGridLineIndex = 2; verticalGridLineIndex < m_verticalGridLineCount; verticalGridLineIndex++)
		{
			x = verticalGridLineIndex * m_gridUnitXLength;
			p1.setX(x);
			p2.setX(x);
			painter.drawLine(p1, p2);

			int dataIndex = dataSize - m_drawDataCount + (m_gridUnitXLength / m_dataUnitLengths[m_zoomIndex]) * verticalGridLineIndex;

			if (dataIndex >= dataSize) break;
			date = m_listRealTimeData[dataIndex].date;
            #if defined (RUNTIME_OS_WIN32) || defined (RUNTIME_OS_MACX)
                timeLabel = JulianDate::JDateItemToString(date, enShowTime | enShowSecond);
            #else
                timeLabel = JulianDate::JDateItemToString(date, enShowTime);
            #endif
			painter.drawText(x, rect.top() + outMargin, textW, textH, Qt::AlignLeft | Qt::AlignTop, timeLabel);
		}

		// draw last label
		x = verticalGridLineIndex * m_gridUnitXLength;
		p1.setX(x);
		p2.setX(x);
		painter.drawLine(p1, p2);

		date = m_listRealTimeData[dataSize - 1].date;
        #if defined (RUNTIME_OS_WIN32) || defined (RUNTIME_OS_MACX)
            timeLabel = JulianDate::JDateItemToString(date, enShowTime | enShowSecond);
        #else
            timeLabel = JulianDate::JDateItemToString(date, enShowTime);
        #endif
		painter.drawText(x, rect.top() + outMargin, textW, textH, Qt::AlignLeft | Qt::AlignTop, timeLabel);
	}

	void RealTimeChart::drawYCoordinate(QPainter& painter, QRectF rect, double minValue, double minY, double scaleY)
	{
		if (scaleY == 0) return;
		double scaleValue = 1.0 / scaleY;
		const double gridy = Helper::GetMarginSpaceValue(enMarginGridYStandardSpace);
		const double outMargin = Helper::GetMarginSpaceValue(enMarginOutGridStandardSpace);
		const double textH = Helper::GetMarginSpaceValue(enMarginYTextHSpace);
		const double textW = Helper::GetMarginSpaceValue(enMarginYTextWSpace);
		QPen pen(m_settings.m_colorForeground);
		painter.setPen(pen);

		double pos = gridy;
		for (int i = 1; pos < rect.height(); ++i)
		{
			double y = rect.top() + rect.height() - pos;
			double yValue = minValue + (minY - y) * scaleValue;
			QString priceText;
			priceText.setNum(yValue, 'f', m_decimalPlaces);
			QPoint p1(rect.left(), y);
			QPoint p2(rect.left() + outMargin, y);
			painter.drawLine(p1, p2);
			painter.drawText(rect.left() + outMargin, y - textH / 2, textW, textH, Qt::AlignVCenter | Qt::AlignLeft, priceText);
			pos = (i + 1) * gridy;
		}
	}

	void RealTimeChart::drawDataLine(QPainter& painter, QRectF rect, double minValue, double minY, double scaleY, drawDataType type)
	{
		const double outMargin = Helper::GetMarginSpaceValue(enMarginOutGridStandardSpace);
		int listSize = m_listRealTimeData.size();

		QPen pen(type == drawAsk ? m_settings.m_colorAsk : m_settings.m_colorBid);
		painter.setPen(pen);

		QPoint last;
		double value;
		for (int i = 0; i < m_drawDataCount; ++i)
		{
			value = type == drawAsk ? m_listRealTimeData[listSize - m_drawDataCount + i].ask : m_listRealTimeData[listSize - m_drawDataCount + i].bid;

			double y = minY - (value - minValue) * scaleY;
			double x = i * m_dataUnitLengths[m_zoomIndex];

			if (i != 0)
			{
				painter.drawLine(last.x(), last.y(), x, y);
			}
			last.setX(x);
			last.setY(y);
		}
		painter.drawLine(last.x(), last.y(), rect.left() + rect.width() + outMargin, last.y());

		QString priceText;
		priceText.setNum(value, 'f', m_decimalPlaces);

		QFontMetrics fontMetircs(painter.font());
		double textW = fontMetircs.width(priceText);
		double textH = fontMetircs.height();
		QRectF textRect(rect.left() + rect.width() + outMargin, last.y() - textH / 2, textW, textH);
		painter.setBrush(type == drawAsk ? m_settings.m_colorAsk : m_settings.m_colorBid);
		painter.drawRect(textRect);

		QPen textPen(m_settings.m_colorBackground);
		painter.setPen(textPen);
		painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, priceText);
	}

	void RealTimeChart::drawCrosshair(QPainter& painter, QRectF rect, double minValue, double minY, double scaleY)
	{
		QPointF mousePos = this->mapFromGlobal(this->cursor().pos());
		if (!rect.contains(mousePos)) return;

		QPen pen(m_settings.m_colorCrosshair);
		painter.setPen(pen);

        // 1. Draw a false cross cursor (real cursor movement will cause a delay)
		const double margin = Helper::GetMarginSpaceValue(enMarginRealTimeCustomCrossCursorMargin);
		const double marginLen = Helper::GetMarginSpaceValue(enMarginRealTimeCustomCrossCursorMarginLen);
		const double outMargin = Helper::GetMarginSpaceValue(enMarginOutGridStandardSpace);

		painter.drawLine(mousePos.x() - marginLen, mousePos.y() - margin, mousePos.x() - margin, mousePos.y() - margin);
		painter.drawLine(mousePos.x() - marginLen, mousePos.y() + margin, mousePos.x() - margin, mousePos.y() + margin);
		painter.drawLine(mousePos.x() + marginLen, mousePos.y() - margin, mousePos.x() + margin, mousePos.y() - margin);
		painter.drawLine(mousePos.x() + marginLen, mousePos.y() + margin, mousePos.x() + margin, mousePos.y() + margin);

		painter.drawLine(mousePos.x() - margin, mousePos.y() - marginLen, mousePos.x() - margin, mousePos.y() - margin);
		painter.drawLine(mousePos.x() - margin, mousePos.y() + marginLen, mousePos.x() - margin, mousePos.y() + margin);
		painter.drawLine(mousePos.x() + margin, mousePos.y() - marginLen, mousePos.x() + margin, mousePos.y() - margin);
		painter.drawLine(mousePos.x() + margin, mousePos.y() + marginLen, mousePos.x() + margin, mousePos.y() + margin);

        // 2.Draw crosshair
		painter.drawLine(rect.left(), mousePos.y(), rect.left() + rect.width() + outMargin, mousePos.y());
		painter.drawLine(mousePos.x(), rect.top(), mousePos.x(), rect.top() + rect.height() + outMargin);

        // 3. Draw coordinates and display data
		QFontMetrics fontMetircs(painter.font());
		const double textH = fontMetircs.height();

		double scaleValue = scaleY == 0 ? 1.0 : 1.0 / scaleY;
		double yValue = minValue + (minY - mousePos.y()) * scaleValue;
		int index = mousePos.x() / m_dataUnitLengths[m_zoomIndex];
		if (index >= m_drawDataCount) index = m_drawDataCount - 1;
		int sizeList = m_listRealTimeData.size();
		double xValue = m_listRealTimeData[sizeList - m_drawDataCount + index].date;
		double askValue = m_listRealTimeData[sizeList - m_drawDataCount + index].ask;
		double bidValue = m_listRealTimeData[sizeList - m_drawDataCount + index].bid;

		painter.setBrush(m_settings.m_colorCrosshair);

		QPen textPen(m_settings.m_colorBackground);
		painter.setPen(textPen);

        // Y axis text
		QString priceText;
		double textWidth;
		priceText.setNum(yValue, 'f', m_decimalPlaces);
		textWidth = fontMetircs.width(priceText) + 2;
		QRect textRectY(rect.left() + rect.width() + outMargin, mousePos.y() - textH / 2, textWidth, textH);
		painter.drawRect(textRectY);
		painter.drawText(textRectY, Qt::AlignHCenter | Qt::AlignVCenter, priceText);

        // X axis text
		priceText = JulianDate::JDateItemToString(xValue, enShowYear | enShowMonth | enShowDay | enShowTime | enShowSecond, 3);
		textWidth = fontMetircs.width(priceText) + 2;
		QRect textRectX(mousePos.x() - textWidth / 2, rect.top() + rect.height() + outMargin, textWidth, textH);
		if (textRectX.x() < 0)
		{
			textRectX.moveLeft(0);
		}
		painter.drawRect(textRectX);
		painter.drawText(textRectX, Qt::AlignCenter, priceText);

        // Middle text
		QString askText, bidText;
		priceText = askText.setNum(askValue, 'f', m_decimalPlaces) + '/' + bidText.setNum(bidValue, 'f', m_decimalPlaces);
		textWidth = fontMetircs.width(priceText) + 2;
		QRect textRectM(mousePos.x() + margin + outMargin, mousePos.y() - margin - outMargin - textH, textWidth, textH);
		if (textRectM.x() + textWidth > rect.x() + rect.width())
		{
			textRectM.moveLeft(textRectM.x() - textWidth - 2 * (margin + outMargin));
		}
		painter.drawRect(textRectM);
		painter.drawText(textRectM, Qt::AlignCenter, priceText);
	}

	void RealTimeChart::getMaxMinValue(double& max, double& min)
	{
		const PrimRealTimeData& lastData = m_listRealTimeData.last();
		if (lastData.ask < lastData.bid)
		{
			min = lastData.ask; max = lastData.bid;
		}
		else
		{
			min = lastData.bid; max = lastData.ask;
		}
		
		int dataSize = m_listRealTimeData.size();
		for (int i = 1; i < m_drawDataCount; ++i)
		{
			const PrimRealTimeData& data = m_listRealTimeData.at(dataSize - i - 1);
			double newMax, newMin;
			if (data.ask < data.bid)
			{
				newMax = data.bid; newMin = data.ask;
			}
			else
			{
				newMax = data.ask; newMin = data.bid;
			}
			if (max < newMax) max = newMax;
			if (min > newMin) min = newMin;
		}
	}

	void RealTimeChart::keyPressEvent(QKeyEvent* event)
	{
		Qt::Key key = (Qt::Key)event->key();
		switch (key)
		{
		case Qt::Key_Plus:
			this->zoomIn();
			break;
		case Qt::Key_Minus:
			this->zoomOut();
			break;
		default:
			break;
		}
		return QWidget::keyPressEvent(event);
	}

	void RealTimeChart::zoomIn()
	{
		if (m_zoomIndex < m_dataUnitLengths.count() - 1)
		{
			m_zoomIndex++;
			this->update();
		}
	}

	void RealTimeChart::zoomOut()
	{
		if (m_zoomIndex > 0)
		{
			m_zoomIndex--;
			this->update();
		}
	}

	bool RealTimeChart::isInXCoorPanel(QPoint pos)
	{
		QRect xAxisRect = this->rect();
		xAxisRect.moveTop(xAxisRect.height() - m_XAxisHeight);
		xAxisRect.setHeight(m_XAxisHeight);
		return xAxisRect.contains(pos);
	}

	bool RealTimeChart::isInYCoorPanel(QPoint pos)
	{
		QRect yAxisRect = this->rect();
		yAxisRect.moveLeft(yAxisRect.width() - m_YAxisWidth);
		yAxisRect.setWidth(m_YAxisWidth);
		return yAxisRect.contains(pos);
	}

	void RealTimeChart::HandleSettingsSlot()
	{
		RealTimeDialog dialog(m_settings, this);
        connect(&dialog, SIGNAL(UserSetRealTimeChartColorSignSignal()), this, SIGNAL(UserSetRealTimeChartColorSignSignal()));
		dialog.exec();

		if (dialog.result() == QDialog::Accepted)
		{
			m_settings = dialog.getSettings();
			update();
		}
	}

	void RealTimeChart::contextMenuEvent(QContextMenuEvent *event)
	{
		QMenu menu;
		menu.addAction(m_actionHandleSetting);
		menu.exec(event->globalPos());
	}
}

#include "moc_RealTimeChart.cpp"

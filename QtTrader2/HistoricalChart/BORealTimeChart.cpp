#include "BORealTimeChart.h"
#include "SR.h"
#include <QPainter>
#include <QMoveEvent>
#include "JulianDate.h"
#include "BORealTimeDialog.h"
#include <QMenu>
#include <QGuiApplication>
#include "Helper.h"
#include "Cursors.h"
#include "../QtFramework/QsLog.h"
#include <QTimer>

namespace HistoricalChartSpace
{
	BORealTimeChart::BORealTimeChart(BORealtimeDataSource* dataSource, BORealTimePriceSettings settings, QWidget* parent)
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

	BORealTimeChart::~BORealTimeChart()
	{
	}

	void BORealTimeChart::RealTimeDataComeSlot(const PrimBORealTimeData& data)
	{
        // 2016-03-16 (Liansheng: Only the changed price is displayed on the real-time chart)
		int lastIndex = m_listRealTimeData.count() - 1;
		if (lastIndex >= 0)
		{
			const PrimBORealTimeData& lastData = m_listRealTimeData[lastIndex];
			if (lastData.barStroke == data.barStroke) return;
		}

		if (m_listRealTimeData.size() == m_maxDataCount)
		{
			m_listRealTimeData.pop_front();
		}
		m_listRealTimeData.append(data);
		update();
	}

	void BORealTimeChart::UpdateRealTimeDataSlot(const QList<PrimBORealTimeData> & listData)
	{
		m_listRealTimeData = listData;
		m_decimalPlaces = m_dataSource->getDecimalPlaces();
		update();
	}

	void BORealTimeChart::paintEvent(QPaintEvent * event)
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
        // Draw Midline
		if (m_settings.m_bDrawBarStroke)
		{
			drawDataLine(painter, rect, minValue, minY, scaleY);
		}

        // Draw crosshair
		if (m_settings.m_bDrawCrosshair)
		{
			drawCrosshair(painter, rect, minValue, minY, scaleY);
		}

		painter.setPen(m_settings.m_colorForeground);
		painter.setBrush(Qt::NoBrush);
		painter.drawRect(rect);

		if(m_BOExecutePrice!="")
		{
			drawBOExecutePrice(painter, rect, minValue, minY, scaleY);
		}

		if(this->m_BOFloatPLInString != "")
		{
			//QLOG_INFO("Chart") << "BORealTimeChart::paintEvent m_BOFloatPLInString：" << m_BOFloatPLInString;
			QFont font = this->font();
			font.setPointSize(14);
			font.setFamily("Arial");
			painter.setFont(font);
			painter.setPen(m_settings.m_colorBOText);
			painter.setBrush(Qt::NoBrush);
			QString BOFloatPLInString = QString("%1%2").arg(SR::get("BOFloatPL:"), this->m_BOFloatPLInString);			
			QFontMetrics fm(font);
			int width = fm.boundingRect(BOFloatPLInString).width();
			painter.drawText(rect.width() - width - 10, 25, BOFloatPLInString);
		}		

        // Draw BO count Down
		if (m_settings.m_isShowBOText)
		{
			//QLOG_INFO("Chart") << "BORealTimeChart::paintEvent drawBOCountdown：" << m_BOCountdown;
			drawBOCountdown(painter, rect);
		}

		
	}

	void BORealTimeChart::mousePressEvent(QMouseEvent* event)
	{
		if (m_settings.m_isShowXaxis && event->buttons() == Qt::LeftButton && this->isInXCoorPanel(event->pos()) || this->isInYCoorPanel(event->pos()))
		{
			m_startMovePos = event->pos();
		}
	}

	void BORealTimeChart::mouseMoveEvent(QMouseEvent *event)
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

	void BORealTimeChart::drawGrid(QPainter& painter, QRectF rect)
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

	void BORealTimeChart::drawXCoordinate(QPainter& painter, QRectF rect)
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
            //_ASSERT(dataIndex > 0);
			if (dataIndex >= dataSize) break;
			date = m_listRealTimeData[dataIndex].date;
			timeLabel = JulianDate::JDateItemToString(date, enShowTime | enShowSecond);
			painter.drawText(x, rect.top() + outMargin, textW, textH, Qt::AlignLeft | Qt::AlignTop, timeLabel);
		}

		// draw last label
		x = verticalGridLineIndex * m_gridUnitXLength;
		p1.setX(x);
		p2.setX(x);
		painter.drawLine(p1, p2);

		date = m_listRealTimeData[dataSize - 1].date;
		timeLabel = JulianDate::JDateItemToString(date, enShowTime | enShowSecond);
		painter.drawText(x, rect.top() + outMargin, textW, textH, Qt::AlignLeft | Qt::AlignTop, timeLabel);
	}

	void BORealTimeChart::drawYCoordinate(QPainter& painter, QRectF rect, double minValue, double minY, double scaleY)
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

	void BORealTimeChart::drawDataLine(QPainter& painter, QRectF rect, double minValue, double minY, double scaleY)
	{
		const double outMargin = Helper::GetMarginSpaceValue(enMarginOutGridStandardSpace);
		int listSize = m_listRealTimeData.size();

		QLinearGradient linearGradient(rect.width()/2, 0, rect.width()/2, rect.height()); 
		linearGradient.setSpread(QGradient::PadSpread);
		linearGradient.setColorAt(0, QColor(0, 128, 0, 255)); 
		linearGradient.setColorAt(1.0, QColor(0, 100, 0, 90)); 
		QBrush qp(linearGradient);

		QPen pen(m_settings.m_colorBarStroke);
		painter.setPen(pen);

		QPolygon polygon = QPolygon();
		polygon << QPoint(0, rect.height());
		int pIndex = 0;

		double startRangeBOStartTimeX = 0.0;
		double endRangeBOStartTimeX = 0.0;
		double BOStartTimeX = 0.0;
		double startRangeBOEndTimeX = 0.0;
		double endRangeBOEndTimeX = 0.0;
		double BOEndTimeX = 0.0;

		QPoint last;
		double value;
		for (int i = 0; i < m_drawDataCount; ++i)
		{
			auto primBORealTimeData = m_listRealTimeData[listSize - m_drawDataCount + i];
			value = primBORealTimeData.barStroke;

			double y = minY - (value - minValue) * scaleY;
			double x = i * m_dataUnitLengths[m_zoomIndex];
			if (i != 0)
			{
				painter.setPen(pen);
				painter.setBrush(Qt::NoBrush);
				painter.drawLine(last.x(), last.y(), x, y);
			}			
			polygon << QPoint( x, y);
			pIndex++;

			if(!m_boCompleted)
			{
				if(pIndex%20 == 0)
				{
					polygon << QPoint(x, rect.height());
					painter.setRenderHint(QPainter::Antialiasing, true); 	
					painter.setBrush(qp);
					painter.setPen(Qt::NoPen);
					painter.drawPolygon(polygon);
					polygon = QPolygon();
					pIndex = 0;
					polygon << QPoint(x, rect.height());
					polygon << QPoint( x, y);
				}

				if(primBORealTimeData.date == m_BOStartTime)
				{
					BOStartTimeX = x;
				}
				else
				{
					if(primBORealTimeData.date > m_BOStartTime)
					{
						if(startRangeBOStartTimeX == 0.0)
						{
							endRangeBOStartTimeX = x;
							startRangeBOStartTimeX = last.x();
						}
					}
				}

				if(primBORealTimeData.date == m_BOEndTime)
				{
					BOEndTimeX = x;
				}
				else
				{
					if(primBORealTimeData.date > m_BOEndTime)
					{
						if(startRangeBOEndTimeX == 0.0)
						{
							endRangeBOEndTimeX = x;
							startRangeBOEndTimeX = last.x();
						}
					}
				}
			}

			last.setX(x);
			last.setY(y);
		}


		if(!m_boCompleted)
		{
			polygon << QPoint(last.x(), rect.height());
			painter.setRenderHint(QPainter::Antialiasing, true); 			
			painter.setBrush(qp);
			painter.setPen(Qt::NoPen);
			painter.drawPolygon(polygon);


			if(m_BOStartTime > 0.0)
			{
				painter.setPen(Qt::green);
				painter.setBrush(Qt::NoBrush);
				if(BOStartTimeX == 0.0) BOStartTimeX = (startRangeBOStartTimeX + endRangeBOStartTimeX)/2.0;
				painter.drawLine(BOStartTimeX, 0, BOStartTimeX, rect.height());
			}
			if(m_BOEndTime > 0.0)
			{
				painter.setPen(Qt::red);
				painter.setBrush(Qt::NoBrush);
				if(BOEndTimeX == 0.0) BOEndTimeX = (startRangeBOEndTimeX + endRangeBOEndTimeX)/2.0;
				painter.drawLine(BOEndTimeX, 0, BOEndTimeX, rect.height());
			}
		}

		painter.setPen(pen);
		painter.setBrush(Qt::NoBrush);
		painter.drawLine(last.x(), last.y(), rect.left() + rect.width() + outMargin, last.y());



		QString priceText;
		priceText.setNum(value, 'f', m_decimalPlaces);

		QFontMetrics fontMetircs(painter.font());
		double textW = fontMetircs.width(priceText);
		double textH = fontMetircs.height();
		QRectF textRect(rect.left() + rect.width() + outMargin, last.y() - textH / 2, textW, textH);
		painter.setBrush(m_settings.m_colorBarStroke);
		painter.drawRect(textRect);

		QPen textPen(m_settings.m_colorBackground);
		painter.setPen(textPen);
		painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, priceText);
	}

	void BORealTimeChart::drawCrosshair(QPainter& painter, QRectF rect, double minValue, double minY, double scaleY)
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
		double barStrokeValue = m_listRealTimeData[sizeList - m_drawDataCount + index].barStroke;

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
		QString barStrokeText;
		priceText = barStrokeText.setNum(barStrokeValue, 'f', m_decimalPlaces);
		textWidth = fontMetircs.width(priceText) + 2;
		QRect textRectM(mousePos.x() + margin + outMargin, mousePos.y() - margin - outMargin - textH, textWidth, textH);
		if (textRectM.x() + textWidth > rect.x() + rect.width())
		{
			textRectM.moveLeft(textRectM.x() - textWidth - 2 * (margin + outMargin));
		}
		painter.drawRect(textRectM);
		painter.drawText(textRectM, Qt::AlignCenter, priceText);
	}

	void BORealTimeChart::drawBOCountdown(QPainter& painter, QRectF rect)
	{
		if(m_BOCountdown > 0)
		{
			QFont font = this->font();
			font.setPointSize(90);
			font.setFamily("Arial Black");
			painter.setFont(font);
			painter.setPen(m_settings.m_colorBOText);
			painter.setBrush(Qt::NoBrush);

			QFontMetrics fm(font);
			int width = fm.boundingRect(m_BOCountdown).width();
			painter.drawText((rect.width() / 2) - (width/ 2), rect.height() / 2, m_BOCountdown);
		}
	}

	void BORealTimeChart::drawBOExecutePrice(QPainter& painter, QRectF rect, double minValue, double minY, double scaleY)
	{
		const double outMargin = Helper::GetMarginSpaceValue(enMarginOutGridStandardSpace);
		QPen pen(this->m_isUp ? m_settings.m_colorBullish : m_settings.m_colorBearish);
		painter.setPen(pen);
		painter.setBrush(Qt::NoBrush);

		int BOExecutePriceY = minY - (m_BOExecutePrice.toDouble() - minValue) * scaleY;
		painter.drawLine(0, BOExecutePriceY, rect.left() + rect.width() + outMargin, BOExecutePriceY);

		QFontMetrics fontMetircs(painter.font());
		double textW = fontMetircs.width(m_BOExecutePrice);
		double textH = fontMetircs.height();
		QRectF textRect(rect.left() + rect.width() + outMargin, BOExecutePriceY - textH / 2, textW, textH);
		painter.setBrush(this->m_isUp ? m_settings.m_colorBullish : m_settings.m_colorBearish);
		painter.drawRect(textRect);

		QPen textPen(m_settings.m_colorBackground);
		painter.setPen(textPen);
		painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, m_BOExecutePrice);
	}

	void BORealTimeChart::setBOInfo(QString BOCountdown, QString BOFloatPLInString, double BOStartTime, double BOEndTime, QString BOExecutePrice, bool isUp)
	{
		m_boCompleted = false;
		this->m_BOCountdown = BOCountdown;
		this->m_BOFloatPLInString = BOFloatPLInString;
		this->m_BOStartTime = BOStartTime;
		this->m_BOEndTime = BOEndTime;
		this->m_BOExecutePrice = BOExecutePrice;
		this->m_isUp = isUp;
		if(BOEndTime != 0.0)
		{
			QTimer::singleShot(20000, this, SLOT(resetBOStartTimeAndEndTimeSlot()));
		}
	}

	void BORealTimeChart::getMaxMinValue(double& max, double& min)
	{
		const PrimBORealTimeData& lastData = m_listRealTimeData.last();
		min = lastData.barStroke; max = lastData.barStroke;

		int dataSize = m_listRealTimeData.size();
		for (int i = 1; i < m_drawDataCount; ++i)
		{
			const PrimBORealTimeData& data = m_listRealTimeData.at(dataSize - i - 1);
			double newMax, newMin;
			newMax = data.barStroke; newMin = data.barStroke;
			if (max < newMax) max = newMax;
			if (min > newMin) min = newMin;
		}
	}

	void BORealTimeChart::keyPressEvent(QKeyEvent* event)
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

	void BORealTimeChart::zoomIn()
	{
		if (m_zoomIndex < m_dataUnitLengths.count() - 1)
		{
			m_zoomIndex++;
			this->update();
		}
	}

	void BORealTimeChart::zoomOut()
	{
		if (m_zoomIndex > 0)
		{
			m_zoomIndex--;
			this->update();
		}
	}

	bool BORealTimeChart::isInXCoorPanel(QPoint pos)
	{
		QRect xAxisRect = this->rect();
		xAxisRect.moveTop(xAxisRect.height() - m_XAxisHeight);
		xAxisRect.setHeight(m_XAxisHeight);
		return xAxisRect.contains(pos);
	}

	bool BORealTimeChart::isInYCoorPanel(QPoint pos)
	{
		QRect yAxisRect = this->rect();
		yAxisRect.moveLeft(yAxisRect.width() - m_YAxisWidth);
		yAxisRect.setWidth(m_YAxisWidth);
		return yAxisRect.contains(pos);
	}

	void BORealTimeChart::contextMenuEvent(QContextMenuEvent *event)
	{
		QMenu menu;
		menu.addAction(m_actionHandleSetting);
		menu.exec(event->globalPos());
	}

	void BORealTimeChart::HandleSettingsSlot()
	{
		BORealTimeDialog dialog(m_settings, this);
		dialog.exec();

		if (dialog.result() == QDialog::Accepted)
		{
			m_settings = dialog.getSettings();
			update();
		}
	}

	void BORealTimeChart::resetBOStartTimeAndEndTimeSlot()
	{
		this->m_BOStartTime = 0.0;
		this->m_BOEndTime = 0.0;
		this->m_boCompleted = true;
	}
}

#include "moc_BORealTimeChart.cpp"

#include "PricePanel.h"
#include <QPen>
#include <QPainter>
#include "SplitPanel.h"
#include "Config.h"
#include <QVariant>
#include <QSettings>
#include <QStyleOptionGraphicsItem>
#include "JulianDate.h"
#include "MainPainterPanel.h"
#include "YCoorPanel.h"
#include <QEvent>
#include "IndicatorPanel.h"
#include <historicalchart.h>
#include "Cursors.h"
#include "../QtFramework/QsLog.h"

namespace HistoricalChartSpace
{

	PricePanel::PricePanel(HistoricalChart* pView, Config& config, BaseData& baseData) :ChartPanel(pView, config, baseData), m_priceSettings(m_config.getPriceSettings())
	{
		m_PanelType = enPanelPrice;
		m_PaintArgs.scaleY = 1.0;
		m_PaintArgs.minY = 0.0;
		m_PaintArgs.minValue = 0.0;
		m_adjustYScale = 1.0;
		this->applyConfig();
		this->changeStyle();

		// load cursor:
		QPixmap pixmap(adjust_vertical);
		QCursor cursor(pixmap, 0, 0);
		m_yCoorPanel->setCursor(cursor);
	}

	PricePanel::~PricePanel()
	{
	}

	void PricePanel::applyConfig()
	{
		m_indicators.clear();
        m_currentDrawingObjects.clear();
        m_drawingObjectMaps.clear();

		if (m_config.hasConfigData())
		{
			QVector<IndicatorSettings> indicatorSettings;
            QMap<QUuid, QVector<DrawingObjectSettings>> lineObjectMaps;
            m_config.getPricePanelSettings(m_heightRatio, indicatorSettings, lineObjectMaps);
            this->applySettings(indicatorSettings, lineObjectMaps);
		}
	}

    int PricePanel::getSpareBar()
    {
        const int barsCanBeDisplayed = getBarCountCanBeDisplayed();
        const int oldBeginIndex = m_baseData.getBeginIndex();
        const int oldEndIndex = m_baseData.getEndIndex();
        const int shiftCount = getShiftBarCount();
        int bars = barsCanBeDisplayed - (oldEndIndex - oldBeginIndex) - shiftCount;
        return bars;
    }

	/*   
	bars > 0 move right, bars < 0 move left

	Process logic:
		Movement is just to set BeginIndex = BeginIndex - bars.
		Then determine EndIndex according to barsCanBeDisplayed, BeginIndex two arguments.
	*/
	bool PricePanel::moveChart(int bars)
	{
		const bool isMoveLeft = bars < 0;
		const int moveCount = bars > 0 ? bars : -bars;
		const int lastIndex = m_baseData.size() - 1;

		const int barsCanBeDisplayed = getBarCountCanBeDisplayed();
		const int shiftCount = getShiftBarCount();

		const int oldBeginIndex = m_baseData.getBeginIndex();
		const int oldEndIndex = m_baseData.getEndIndex();
		qDebug() << " PricePanel::moveChart barsCanBeDisplayed:" << barsCanBeDisplayed << " shiftCount:" << shiftCount << " oldBeginIndex:" << oldBeginIndex << " oldEndIndex:" << oldEndIndex << " bars:" << bars;

		int beginIndex, actualMovedCount;
		if (isMoveLeft)
		{
			int maxBarsCanMoveLeft = lastIndex - oldBeginIndex - barsCanBeDisplayed + shiftCount;
			if (maxBarsCanMoveLeft > 0)
			{
				actualMovedCount = maxBarsCanMoveLeft > moveCount ? moveCount : maxBarsCanMoveLeft;
				beginIndex = oldBeginIndex + actualMovedCount;

				qDebug() << " PricePanel::moveChart maxBarsCanMoveLeft:" << maxBarsCanMoveLeft << " actualMovedCount:" << actualMovedCount << " beginIndex:" << beginIndex;
			}
			else
			{
				return false;
			}
		}
		else
		{
			int maxBarsCanMoveRight = oldBeginIndex;
			if (maxBarsCanMoveRight > 0)
			{
				actualMovedCount = maxBarsCanMoveRight > moveCount ? moveCount : maxBarsCanMoveRight;
				beginIndex = oldBeginIndex - actualMovedCount;
				qDebug() << " PricePanel::moveChart maxBarsCanMoveRight:" << maxBarsCanMoveRight << " actualMovedCount:" << actualMovedCount << " beginIndex:" << beginIndex;
			}
			else
			{
				return false;
			}
		}
		m_baseData.setBeginIndex(beginIndex);

		// determine endIndex
		int possibleEndIndex = beginIndex + barsCanBeDisplayed;
		int endIndex = possibleEndIndex > lastIndex ? lastIndex : possibleEndIndex;
		qDebug() << " PricePanel::moveChart possibleEndIndex:" << possibleEndIndex << " endIndex:" << endIndex;
		m_baseData.setEndIndex(endIndex);


		this->refresh();
		return true;
	}

	void PricePanel::moveToEnd(/*bool fillupChart*/)
	{
		int lastIndex = m_baseData.size() - 1;
		if (lastIndex >= 0)
		{
			const int barsCanBeDisplayed = this->getBarCountCanBeDisplayed();
			if (barsCanBeDisplayed > 0)
			{
				int shiftCount = this->getShiftBarCount();
				int beginIndex = lastIndex - barsCanBeDisplayed + shiftCount;
				if (beginIndex < 0) beginIndex = 0;

				m_baseData.setBeginIndex(beginIndex);
				m_baseData.setEndIndex(lastIndex);
				this->refresh();
			}
		}
	}

	void PricePanel::moveToHome()
	{
		int lastIndex = m_baseData.size() - 1;
		if (lastIndex >= 0)
		{
			const int barsCanBeDisplayed = this->getBarCountCanBeDisplayed();
			if (barsCanBeDisplayed > 0)
			{
				int beginIndex = 0;
				int endIndex = barsCanBeDisplayed - 1;
				if (endIndex > lastIndex) endIndex = lastIndex;
				m_baseData.setBeginIndex(beginIndex);
				m_baseData.setEndIndex(endIndex);
				this->refresh();
			}
		}
	}

	void PricePanel::updateEndIndex()
	{
		if (m_baseData.size() > 0)
		{
			const int lastIndex = m_baseData.size() - 1;
			const int oldEndIndex = m_baseData.getEndIndex();
			// remain beginIndex unchanged when resize the window, only change the end position.
			int endIndex = m_baseData.getBeginIndex() + this->getBarCountCanBeDisplayed();
			if (endIndex > lastIndex) endIndex = lastIndex;
			m_baseData.setEndIndex(endIndex);

			//if (m_baseData.getEndIndex() > lastIndex)
			//{
			//	m_baseData.setEndIndex(lastIndex);
			//}


			if (oldEndIndex != m_baseData.getEndIndex())
			{
				this->refresh();
			}
		}
	}

	void PricePanel::paintMainPanel(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
	{
		if (m_baseData.size() > 0)
		{
			const QRectF& rect = option->exposedRect;

			//// show bound rect for test
			//painter->setPen(QPen(QColor(0, 200, 255)));
			//painter->drawRect(rect);

            // Draw horizontal grid
			if (m_commonSettings.m_isShowGrid)
			{
				this->drawGrid(painter, rect);
			}
			QRectF priceRect(rect);
			priceRect.setHeight(m_PaintArgs.minY - rect.top() + 1);

			painter->setClipRect(priceRect);
			this->paintIndicators(painter);

			if (m_baseData.isValid())
			{
				m_PriceStyle->drawPrice(painter, priceRect, m_baseData.getBarDistance());

				painter->setClipping(false);
				this->drawBidAskLine(painter);

				if (m_priceSettings.m_isShowVolume)
				{
					const double scaleVolume = Helper::GetMarginSpaceValue(enMarginPriceVolumeSpaceScale);
					double top = rect.top() + rect.height() * (1 - scaleVolume);
					double hight = rect.top() + rect.height() - top;

					QRectF volumeRect(priceRect.left(), top, priceRect.width(), hight);
					//painter->setClipping(true);
					this->drawVolume(painter, volumeRect);
				}
				updateAllDrawingObject();
			}
			else
			{
				QLOG_WARN("Chart") << "PricePanel::paintMainPanel base data is invalid."
					<< "open.size: " << m_baseData.getOpens().size()
					<< ",high.size: " << m_baseData.getHighs().size()
					<< ",low.size: " << m_baseData.getLows().size()
					<< ",close.size: " << m_baseData.getCloses().size()
					<< ",beginIndex: " << m_baseData.getBeginIndex()
					<< ",open.size: " << m_baseData.getEndIndex();
			}
		}
	}

	void PricePanel::paintYCoorPanel(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
	{
		QRectF rect = option->exposedRect;

		// show bound rect for test
		//QPen p(QColor(255, 0, 0));
		//p.setWidth(5);
		//painter->setPen(p);
		//painter->drawRect(rect);

		QPen pen(m_commonSettings.m_foreground, 1);
		painter->setPen(pen);

		//const double scaleVolume = Helper::GetMarginSpaceValue(enMarginPriceVolumeSpaceScale);
		//const double scaleMargin = Helper::GetMarginSpaceValue(enMarginValueToBondSpaceScale);
		const double yStandardSpace = Helper::GetMarginSpaceValue(enMarginGridYStandardSpace);
		const double outGridSpace = Helper::GetMarginSpaceValue(enMarginOutGridStandardSpace);
		//const double yTextH = Helper::GetMarginSpaceValue(enMarginYTextHSpace);
		const double yTextW = Helper::GetMarginSpaceValue(enMarginYTextWSpace);
		const double spaceText = Helper::GetMarginSpaceValue(enMarginSpaceTextToCoordinate);

		QFontMetrics fontMetrics(painter->font());
		const double yTextH = fontMetrics.height();

		double top = rect.top();
		double Xbegin = rect.left();
		for (double y = top + rect.height() - yStandardSpace; y > top; y -= yStandardSpace)
		{
			QPointF pointBegin(Xbegin, y);
			QPointF pointEnd(Xbegin + outGridSpace, y);
			double yValue = ChartPanel::Y2Value(y);
			painter->drawLine(pointBegin, pointEnd);
			QString priceLabel;
			priceLabel.setNum(yValue, 'f', m_pView->getDecimalPlaces());
			painter->drawText(Xbegin + outGridSpace + spaceText, y - yTextH / 2, yTextW, yTextH, Qt::AlignLeft, priceLabel);
		}
	}

	void PricePanel::refresh()
	{
		const QRectF& rect = m_mainPainterPanel->boundingRect();
		const double volumeHeightPercent = Helper::GetMarginSpaceValue(enMarginPriceVolumeSpaceScale);
		const double marginHeigthPercent = Helper::GetMarginSpaceValue(enMarginValueToBondSpaceScale);

		m_PaintArgs.minValue = calculateMinValue();
		m_maxValue = calculateMaxValue();
		double minY = rect.top() + rect.height() * (1 - volumeHeightPercent - marginHeigthPercent);
		double height = rect.height() * (1 - volumeHeightPercent - 2 * marginHeigthPercent);
		m_PaintArgs.minY = minY - height * (1 - m_adjustYScale) / 2;
		double diff = m_maxValue - m_PaintArgs.minValue;
		diff = diff > 0 ? diff : 1;
		m_PaintArgs.scaleY = height * m_adjustYScale / diff;
		if (m_PaintArgs.scaleY <= 0) m_PaintArgs.scaleY = 1;

		//ChartPanel::refresh(rect);
		m_pView->updateOrderLines();
		m_pView->getPriceAlertManager()->updateUI();
		ChartPanel::updateUI();
	}

	void PricePanel::drawGrid(QPainter* painter, const QRectF& rect)
	{
		QPen pen(m_commonSettings.m_gridColor);
		pen.setStyle(Qt::DotLine);
		painter->setPen(pen);
		const double yStandardSpace = Helper::GetMarginSpaceValue(enMarginGridYStandardSpace);
		double nspace = rect.top() + rect.height();
		double rectTop = rect.top();
		double Xbegin = rect.left();
		double Xend = Xbegin + rect.width();

		for (nspace -= yStandardSpace; nspace > rectTop; nspace -= yStandardSpace)
		{
			QPointF pointBegin(Xbegin, nspace);
			QPointF pointEnd(Xend, nspace);
			painter->drawLine(pointBegin, pointEnd);
		}
	}

	void PricePanel::drawBidAskLine(QPainter* painter)
	{
		QRectF rect = this->boundingRect();
		double left = rect.left();
		double bid, ask;
		m_baseData.getBidAsk(bid, ask);
		double yClose = m_PaintArgs.minY - (bid - m_PaintArgs.minValue) * m_PaintArgs.scaleY;
		double yAsk = m_PaintArgs.minY - (ask - m_PaintArgs.minValue) * m_PaintArgs.scaleY;

        //Determine whether to draw the current price horizontal line
		if (m_priceSettings.m_isShowAskPriceLine)
		{
			QPen pen(m_priceSettings.m_askLineColor);
			painter->setPen(pen);
            //Line drawing and coordinates
			this->drawLineAndYCoordinate(rect, left, yAsk, painter, ask);
		}

		if (m_priceSettings.m_isShowBidPriceLine)
		{
			QPen pen(m_priceSettings.m_bidLineColor);
			painter->setPen(pen);
            // Line drawing and coordinates
			this->drawLineAndYCoordinate(rect, left, yClose, painter, bid);
		}
	}

	void PricePanel::drawLineAndYCoordinate(const QRectF& rect, double left, double y, QPainter *painter, double data)
    {
        // Determine whether the price line is out of bounds
		if (y < rect.top() || y > rect.top() + rect.height()) return;

		double right = rect.left() + rect.width() - Helper::GetMarginSpaceValue(enMarginYCoordinate);
		painter->drawLine(left, y, right, y);

		QRectF textRect;
		textRect.setLeft(right + Helper::GetMarginSpaceValue(enMarginOutGridStandardSpace));
		textRect.setTop(y - Helper::GetMarginSpaceValue(enMarginYTextHSpace) / 2);
		textRect.setHeight(Helper::GetMarginSpaceValue(enMarginYTextHSpace));
		textRect.setWidth(Helper::GetMarginSpaceValue(enMarginYTextWSpace));

		painter->setBrush(painter->pen().color());
		painter->drawRect(textRect);
		painter->setPen(m_commonSettings.m_foreground);
		QString text;
		text.setNum(data, 'f', m_pView->getDecimalPlaces());
		painter->drawText(textRect, Qt::AlignLeft | Qt::AlignCenter, text);
	}

	void PricePanel::drawVolume(QPainter* painter, QRectF& rect)
	{
		if (m_baseData.size() == 0) return;
		int begin = m_baseData.getBeginIndex();
		double max = m_baseData.getVolumes()[begin];
		if (max < 0) return;
		double min = max;

		int endIndex = m_baseData.getEndIndex();

		for (int i = begin + 1; i <= endIndex; ++i)
		{
			double temp = m_baseData.getVolumes()[i];
			if (max < temp)
			{
				max = temp;
				continue;
			}
			if (min > temp)
				min = temp;
		}

		double scaleY = rect.height() * (1 - Helper::GetMarginSpaceValue(enMarginValueToBondSpaceScale)) / (max - min > 0 ? max - min : 1);
		double rectY = rect.top() + rect.height();

		double penWidth = m_config.getCommonSettings().m_zoomLevel >= zoom_normal ? 2.0 : 1.0;
		QPen pen(m_priceSettings.m_volumesColor, penWidth);
		painter->setPen(pen);
		for (int i = begin; i <= endIndex; ++i)
		{
			double X = rect.left() + (i - begin) * m_baseData.getBarDistance();
			double Y = rectY - (m_baseData.getVolumes()[i] - min) * scaleY;
			QPointF beginPos(X, Y);
			QPointF endPos(X, rectY);
			painter->drawLine(beginPos, endPos);
		}
	}

	int PricePanel::getBarCountCanBeDisplayed() const
	{
		QRectF rect = m_mainPainterPanel->boundingRect();
		return rect.width() / m_baseData.getBarDistance();
	}

	int PricePanel::getShiftBarCount()
	{
		int shitCount = 1;
		const int barsCanDisplayed = this->getBarCountCanBeDisplayed();
		if (m_commonSettings.m_isShiftData)
		{
			//const double shiftSpace = Helper::GetMarginSpaceValue(enMarginDataShiftSpace);
			const double shiftPercent = 0.18;
			shitCount += barsCanDisplayed * shiftPercent;
		}
		if (shitCount > barsCanDisplayed)
		{
			shitCount = barsCanDisplayed;
		}
		return shitCount;
	}

	void PricePanel::changeStyle()
	{
		enPriceType type = m_priceSettings.m_drawType;

		switch (type)
		{
		case HistoricalChartSpace::enPriceBar:
			m_PriceStyle = QSharedPointer<PriceStyle>(new PriceBarStyle(this));
			break;
		case HistoricalChartSpace::enPriceLine:
			m_PriceStyle = QSharedPointer<PriceStyle>(new PriceLineStyle(this));
			break;
		case HistoricalChartSpace::enPriceCandlesticks:
		case HistoricalChartSpace::enPriceUnknown:
		default:
			m_PriceStyle = QSharedPointer<PriceStyle>(new PriceCandlestickStyle(this));
			break;
		}
		this->refresh();
	}

	void PricePanel::adjustYScale(double moveRatio)
	{
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
		this->refresh();
	}
}


#include "moc_PricePanel.cpp"

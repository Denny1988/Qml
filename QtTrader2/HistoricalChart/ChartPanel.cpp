#include "ChartPanel.h"
#include "SR.h"
#include "Config.h"
#include "BaseDrawing.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QMenu>
#include <QVariant>
#include <historicalchart.h>
#include <QGraphicsSceneContextMenuEvent>
#include "MainPainterPanel.h"
#include "YCoorPanel.h"
#include "IndicatorPanel.h"
#include "Helper.h"
#include "IndicatorDialog.h"
#include "DrawingObjectDialog.h"
#include "JulianDate.h"
#include "ToolBar.h"
#include "DrawingChannel.h"
#include "Fibbo.h"

namespace HistoricalChartSpace
{

	ChartPanel::ChartPanel(HistoricalChart* pView, Config& config, BaseData& baseData)
        :m_pView(pView), m_config(config), m_baseData(baseData), m_commonSettings(config.getCommonSettings()), m_currentEditLine(nullptr), m_PanelWidth(0), m_rect(0)
	{
		m_preSPlitPanel = NULL;
		m_nextSplitPanel = NULL;
		m_heightRatio = 1.0;

		this->setAcceptHoverEvents(true);
		this->setFlags(ItemIsSelectable | ItemClipsChildrenToShape);
		this->setCursor(Qt::ArrowCursor);
		m_yCoorPanel = new YCoorPanel(this);  // instantiate YCoorPanel first for paint first
		m_mainPainterPanel = new MainPainterPanel(this);

		m_indicatorPropertyAction = new QAction(SR::get("IndicatorProperty"), this);
		m_delIndicatorAction = new QAction(SR::get("DelIndicator"), this);
		m_DrawingObjectPropertyAction = new QAction(SR::get("DrawingObjectProperty"), this);
		m_delLineObjectAction = new QAction(SR::get("DeleteDrawingObject"), this);

		connect(m_indicatorPropertyAction, SIGNAL(triggered()), SLOT(ShowIndicatorPropertySlot()));
		connect(m_delIndicatorAction, SIGNAL(triggered()), SLOT(DeleteIndicatorSlot()));

		connect(m_DrawingObjectPropertyAction, SIGNAL(triggered()), SLOT(ShowDrawingObjectPropertySlot()));
		connect(m_delLineObjectAction, SIGNAL(triggered()), SLOT(DeleteLineObjectSlot()));

		connect(m_pView, SIGNAL(InstrumentChangedSignal(const QUuid&)), SLOT(InstrumentChangedSlot(const QUuid&)));
		connect(m_pView, SIGNAL(QuotePolicyChangedSignal(const QUuid&)), SLOT(QuotePolicyChangedSlot(const QUuid&)));

		m_rect.setParentItem(this);

//#ifndef WIN32
//#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
//        grabGesture(Qt::PanGesture);
//        grabGesture(Qt::TapAndHoldGesture);
//#endif
	}

	ChartPanel::~ChartPanel()
	{
		if (m_yCoorPanel != NULL) delete m_yCoorPanel;
		if (m_mainPainterPanel != NULL) delete m_mainPainterPanel;
	}

	void ChartPanel::showYGrid()
	{
		QRectF rect = this->rect();
		rect.setWidth(rect.width() - Helper::GetMarginSpaceValue(enMarginYCoordinate));
		update(rect);
	}

	void ChartPanel::shiftData()
	{
		QRectF rect = this->rect();
		rect.setWidth(rect.width() - Helper::GetMarginSpaceValue(enMarginYCoordinate));
		update(rect);
	}

	HistoricalChart* ChartPanel::getView()
	{
		return m_pView;
	}

	double ChartPanel::Y2Value(double y)
	{
		double value = -1;
		if (m_PaintArgs.scaleY > 0)
		{
			value = m_PaintArgs.minValue + (m_PaintArgs.minY - y) / m_PaintArgs.scaleY;
			value = Helper::round(value, m_pView->getDecimalPlaces());
		}
		return value;
	}

	double ChartPanel::Value2Y(double value)
	{
		double y = m_PaintArgs.minY - (value - m_PaintArgs.minValue) * m_PaintArgs.scaleY;
		return y;
	}

    // Note: Due to the time gap in baseData, linear algorithms cannot be used. When it is greater than EndIndex, the linear algorithm is used in the second half.
	double ChartPanel::Xpos2Time(const QPointF& point)
	{
		double targetTime;
		const int endIndex = m_baseData.getEndIndex();
		const double barDistance = m_baseData.getBarDistance();

        #ifdef  WIN32
            //_ASSERT(barDistance > 0);
        #endif
		int indexOffset = (point.x() - m_PaintArgs.startX + barDistance / 2) / barDistance;
		int targetIndex = m_baseData.getBeginIndex() + indexOffset;
		if (targetIndex > endIndex)
		{
			double interval = m_pView->getFrequency().getJDateTimeSpan();
			targetTime = m_baseData.getDate(endIndex) + (targetIndex - endIndex) * interval;
		}
		else
		{
			targetTime = m_baseData.getDate(targetIndex);
		}

		return targetTime;
	}

    // Note: Due to the time gap in baseData, linear algorithms cannot be used. When it is greater than EndTime, the linear algorithm is used in the second half.
	double ChartPanel::Time2Xpos(const double time)
	{
		const int beginIndex = m_baseData.getBeginIndex();
		double beginTime = m_baseData.getDate(beginIndex);

		const int endIndex = m_baseData.getEndIndex();
		double endTime = m_baseData.getDate(endIndex);
		double barDistance = m_baseData.getBarDistance();

		double xpos;
		if (time > endTime)
		{
			double interval = m_pView->getFrequency().getJDateTimeSpan();
			int barCount = endIndex - beginIndex;
            #ifdef  WIN32
                //_ASSERT(interval > 0);
            #endif
			xpos = barCount * barDistance + (time - endTime) / interval * barDistance;
		}
		else
		{
			vector<double>& dates = const_cast<vector<double>&>(m_baseData.getDates());
            // If the data of dates is increased or decreased after lower_bound is completed, the addition and subtraction of iterator will cause: vector iterators incompatible Exception, so it needs to be lockedã€?
            QMutexLocker locker(&m_baseData.m_mutex);
            // Use binary search to determine index
            vector<double>::iterator location = std::lower_bound(dates.begin(), dates.end(), time);
			int index = location - dates.begin();
			int barCount = index - beginIndex;
			xpos = barCount * barDistance;
		}
		//qDebug() << "ChartPanel::Time2Xpos, time:" << JulianDate::FromJDateToString(time) << ", xpos:" << xpos << ", time > endTime : " << (time > endTime ? "true" : "false");
		return xpos;
	}

	const QColor& ChartPanel::getForeground() const
	{
		return m_commonSettings.m_foreground;
	}

	void ChartPanel::updateUI()
	{
		const QRectF& rect = m_mainPainterPanel->boundingRect();
		m_PaintArgs.startX = rect.left();
		m_PaintArgs.canvasWidth = rect.width();
		m_PaintArgs.foreground = m_commonSettings.m_foreground;
		this->update();
	}

	double ChartPanel::calculateMaxValue()
	{
		double max = std::numeric_limits<double>::quiet_NaN();
        foreach (Indicator* indicator, m_indicators)
		{
			if (indicator->hasIndicatorData())
			{
				double temp = indicator->calculateMaxValue();
                if (std::isnan(max))
				{
					max = temp;
				}
                else if (!std::isnan(temp) && max < temp)
				{
					max = temp;
				}
			}
		}

		if (m_PanelType == enPanelPrice)
		{
			int dataSize = m_baseData.size();
			int beginIndex = m_baseData.getBeginIndex(), endIndex = m_baseData.getEndIndex();
			if (dataSize == 0 || endIndex < 0 || endIndex >= dataSize) return -1.0;

			double maxOfData = m_baseData.getHighs()[endIndex];
			for (int i = endIndex; i >= beginIndex; --i)
			{
				if (maxOfData < m_baseData.getHighs()[i])
				{
					maxOfData = m_baseData.getHighs()[i];
				}
			}
            if (std::isnan(max))
			{
				max = maxOfData;
			}
			else if (max < maxOfData)
			{
				max = maxOfData;
			}
		}
		return max;
	}

	double ChartPanel::calculateMinValue()
	{
		double min = std::numeric_limits<double>::quiet_NaN();
        foreach (Indicator* indicator, m_indicators)
		{
			if (indicator->hasIndicatorData())
			{
				double temp = indicator->calculateMinValue();
                if (std::isnan(min))
				{
					min = temp;
				}
                else if (!std::isnan(temp) && min > temp)
				{
					min = temp;
				}
			}
		}

		if (m_PanelType == enPanelPrice)
		{
			int dataSize = m_baseData.size();
			int beginIndex = m_baseData.getBeginIndex(), endIndex = m_baseData.getEndIndex();
			if (dataSize == 0 || endIndex < 0 || endIndex >= dataSize) return -1.0;

			double minOfData = m_baseData.getLows()[endIndex];
			for (int i = endIndex; i >= beginIndex; --i)
			{
				if (minOfData > m_baseData.getLows()[i])
				{
					minOfData = m_baseData.getLows()[i];
				}
			}
            if (std::isnan(min))
			{
				min = minOfData;
			}
			else if ( min > minOfData)
			{
				min = minOfData;
			}
		}
		return min;
	}

	void ChartPanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
	{
		QRectF rect = this->rect();
		QRectF mainRect(rect);
		mainRect.setWidth(rect.width() - Helper::GetMarginSpaceValue(enMarginYCoordinate));

		QPen pen(m_commonSettings.m_foreground);
		painter->setPen(pen);
		painter->drawRect(mainRect);
	}

	void ChartPanel::paintIndicators(QPainter *painter)
	{
        foreach (Indicator* pIndicator, m_indicators)
		{
			pIndicator->paintIndicator(painter, m_PaintArgs);
		}
	}

	void ChartPanel::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
	{
		const QPointF& screenPos = event->scenePos();// screenPos();
		const QPointF& p = this->mapFromScene(screenPos);
		for (int index = 0; index < m_indicators.size(); index++)
		{
			if (m_indicators[index]->containsPoint(p))
			{
				QMenu menu(m_pView);
				menu.addAction(m_indicatorPropertyAction);
				menu.addAction(m_delIndicatorAction);
				QVariant var;
				var.setValue<int>(index);
				m_delIndicatorAction->setData(var);
				menu.exec(QCursor::pos());
				return;
			}
		}
		m_pView->showCommonMenu(event, m_PanelType == enPanelPrice);
	}


//#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
//    //------------mobile----------//
//    bool ChartPanel::event(QEvent *event)
//    {
//        if (event->type() == QEvent::Gesture)
//            return gestureEvent(static_cast<QGestureEvent*>(event));
//        return true;
//    }

//    bool ChartPanel::gestureEvent(QGestureEvent *event)
//    {
//        if (QGesture *tapAndHold = event->gesture(Qt::TapAndHoldGesture))
//               tapAndHoldTriggered(static_cast<QTapAndHoldGesture *>(tapAndHold));
//        return true;
//    }

//    void ChartPanel::tapAndHoldTriggered(QTapAndHoldGesture *gesture)
//    {
//        const QPointF& screenPos = gesture->position();
//        const QPointF& p = this->mapFromScene(screenPos);
//        for (int index = 0; index < m_indicators.size(); index++)
//        {
//            if (m_indicators[index]->containsPoint(p))
//            {
//                QMenu menu(m_pView);
//                menu.addAction(m_indicatorPropertyAction);
//                menu.addAction(m_delIndicatorAction);
//                QVariant var;
//                var.setValue<int>(index);
//                m_delIndicatorAction->setData(var);
//                menu.exec(QCursor::pos());
//                return;
//            }
//        }
//        m_pView->showCommonMenu(event, m_PanelType == enPanelPrice);
//    }
//#endif


	void ChartPanel::popupLineObjectContextMenu(BaseDrawing* line, QGraphicsSceneContextMenuEvent* event)
	{
		m_currentEditLine = line;
		QMenu menu(m_pView);
		menu.addAction(m_DrawingObjectPropertyAction);
		menu.addAction(m_delLineObjectAction);
		menu.exec(event->screenPos());
	}

	void ChartPanel::editDrawingObjectProperty(BaseDrawing* drawingObject)
	{
		m_currentEditLine = drawingObject;
		this->ShowDrawingObjectPropertySlot();
	}

	void ChartPanel::setPreSplitPanel(SplitPanel* splitPanel)
	{
		m_preSPlitPanel = splitPanel;
	}
	void ChartPanel::setNextSplitPanel(SplitPanel* splitPanel)
	{
		m_nextSplitPanel = splitPanel;
	}
	SplitPanel* ChartPanel::getPreSplitPanel()
	{
		return m_preSPlitPanel;
	}
	SplitPanel* ChartPanel::getNextSplitPanel()
	{
		return m_nextSplitPanel;
	}

	enPanelType ChartPanel::getPanelType() const
	{
		return m_PanelType;
	}

	void ChartPanel::addIndicator(Indicator* indicator)
	{
		m_indicators.append(indicator);
	}

	void ChartPanel::extendIndicatorDataBuffer()
	{
		for (int i = 0; i < m_indicators.size(); i++)
		{
			m_indicators[i]->extendDataBuffer();
		}
	}

	void ChartPanel::calculateIndicators(int beginIndex, int endIndex)
	{
        // Since the two new dependencies are: First, Previous, the indicator data can be updated in order:
		for (int i = 0; i < m_indicators.size(); i++)
		{
			m_pView->getCalculator()->beginCalculate(m_indicators[i], beginIndex, endIndex);
		}
	}

	bool ChartPanel::removeLineObject(BaseDrawing* line)
	{
        int i  = m_currentDrawingObjects.indexOf(line);
		if(i != -1)
		{
            this->m_drawingObjectMaps[m_pView->getInstrument().getId()].removeAt(i);
            m_currentDrawingObjects.removeAt(i);
			delete line;            
			return true;
		}
		return false;
	}

	void ChartPanel::removeIndicator(Indicator* indicator)
	{
		int index = m_indicators.indexOf(indicator);
		if (index >= 0)
		{
			this->deleteIndicator(index);
		}
	}

	void ChartPanel::updateAllDrawingObject()
	{
		if (m_baseData.size() > 0)
		{
            foreach(BaseDrawing* drawingObject, m_currentDrawingObjects)
			{
				if (drawingObject)
				{
					drawingObject->updateIfNeed();
				}
			}
		}
	}

	void ChartPanel::updateAllDrawingObjectVisible()
	{
        foreach(BaseDrawing* drawingObject, m_currentDrawingObjects)
		{
			if (drawingObject)
			{
				drawingObject->updateVisible();
			}
		}
	}

	void ChartPanel::moveChart(int pixels)
	{
		m_pView->moveChartPixels(pixels);
	}

	void ChartPanel::adjustSubItem()
	{
		if (m_mainPainterPanel == NULL || m_yCoorPanel == NULL) return;

		double adjust = Helper::GetMarginSpaceValue(enMarginAdjustBound);
		m_mainPainterPanel->setPos(rect().left() + adjust, rect().top() + adjust);

		double margin = Helper::GetMarginSpaceValue(enMarginYCoordinate);
		double leftPos = rect().left() + rect().width() - margin + adjust;

#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
        adjust = 2*adjust;
#endif
		m_yCoorPanel->setPos(leftPos, rect().top() + adjust);

        m_PanelWidth = (int)leftPos;

		//this->refresh(m_mainPainterPanel->boundingRect());
		this->updateUI();
	}

	QGraphicsRectItem* ChartPanel::getMainPainterPanel() const
	{
		return m_mainPainterPanel;
	}

	QGraphicsRectItem* ChartPanel::getYCoorPanel() const
	{
		return m_yCoorPanel;
	}

	void ChartPanel::setHeightRatio(double ratio)
	{
		m_heightRatio = ratio;
		this->refresh();
	}

	double ChartPanel::getHeightRatio()
	{
		return m_heightRatio;
	}

	void ChartPanel::saveSettings()
	{
        m_config.savePanelSettings(m_PanelType, m_heightRatio, m_indicators, m_drawingObjectMaps);
	}

    void ChartPanel::applySettings(QVector<IndicatorSettings>& indicatorSettings, QMap<QUuid, QVector<DrawingObjectSettings>>& lineObjectMaps)
	{
		// indicators
		for (int i = 0; i < indicatorSettings.size(); ++i)
		{
			Indicator* indicator = IndicatorCore::CreateIndicator(m_baseData, indicatorSettings[i], false, m_pView->getCalculator());
			m_indicators.append(indicator);
		}
		m_pView->getCalculator()->addEndTask();
qDebug() << " applySettings 111111 :" << lineObjectMaps.count();
        // lineObjects
        QMapIterator<QUuid, QVector<DrawingObjectSettings>> lineObjectItem(lineObjectMaps);
        while (lineObjectItem.hasNext()) {
            lineObjectItem.next();
            QUuid id = lineObjectItem.key();
            QList<BaseDrawing*> drawingObjectList;
            m_drawingObjectMaps.insert(id, drawingObjectList);
            auto lineObjects = lineObjectItem.value();
            for (int i = 0; i < lineObjects.size(); ++i)
            {
                BaseDrawing* lineObject = Helper::CreateDrawingObject(lineObjects[i], this, DrawFinished);
                m_drawingObjectMaps[id].append(lineObject);
            }
        }
        qDebug() << " applySettings 2222 :" << m_pView->getInstrument().getId() << " m_drawingObjectMaps.count:" << m_drawingObjectMaps.count();
        if(m_drawingObjectMaps.count() > 0)
        {
            if(m_drawingObjectMaps.contains(m_pView->getInstrument().getId()))
            {
                qDebug() <<  " applySettings m_currentDrawingObjects";
                m_currentDrawingObjects = m_drawingObjectMaps[m_pView->getInstrument().getId()];
            }
        }
	}

	void ChartPanel::ShowDrawingObjectPropertySlot()
	{
		if (m_currentEditLine)
		{
			DrawingObjectDialog dialog(m_currentEditLine->getSettings(), m_pView);
			dialog.exec();

			if (dialog.result() == QDialog::Accepted)
			{
				m_currentEditLine->setSettings(dialog.getSettings());
			}
			m_currentEditLine = nullptr;
		}
	}

	void ChartPanel::DeleteLineObjectSlot()
	{
		if (m_currentEditLine)
		{
			this->removeLineObject(m_currentEditLine);
			m_currentEditLine = nullptr;
		}
	}

	void ChartPanel::ShowIndicatorPropertySlot()
	{
		int index = m_delIndicatorAction->data().value<int>();
		Indicator* indicator = m_indicators[index];
		if (indicator == NULL) return;
		IndicatorDialog::ShowDialog(indicator, m_pView->getDialogParent());
	}

	/*
	   Two dependence ways of indicators
		1. first dependence(emCPDFirstIndicatorData): depend on the datas of first Indicator;
		2. previous dependence (emCPDPreviousIndicatorData): depend on previous indicator's datas;
	*/
	void ChartPanel::DeleteIndicatorSlot()
	{
		int index = m_delIndicatorAction->data().value<int>();
		this->deleteIndicator(index);
	}

	void ChartPanel::deleteIndicator(int index)
	{
		// 1. remove all dependent indicators
        // Remove the first dependency
        if (index == 0)
		{
			if (m_PanelType == enPanelIndicator)
			{
				for (int i = 0; i < m_indicators.size(); i++)
				{
					delete m_indicators[i];
				}
				IndicatorPanel* indicatorPanel = dynamic_cast<IndicatorPanel*>(this);
				m_pView->removeIndicatorPanel(indicatorPanel);
				return;
			}
			else if (m_PanelType == enPanelPrice)
			{
				for (int i = m_indicators.size() - 1; i > 0; i--)
				{
					if (m_indicators[i]->getIndicatorSettings().m_dependency == emCPDFirstIndicatorData)
					{
						this->internalRemoveIndicators(i);
					}
				}
			}
		}
        else
		{
            // Remove post-order dependencies
			this->internalRemoveIndicators(index);
		}

		// 2. remove self
		delete m_indicators[index];
		m_indicators.remove(index);

		if (m_indicators.size() == 0 && m_PanelType == enPanelIndicator)
		{
			IndicatorPanel* indicatorPanel = dynamic_cast<IndicatorPanel*>(this);
			m_pView->removeIndicatorPanel(indicatorPanel);
		}
		this->update();
	}

    void ChartPanel::InstrumentChangedSlot(const QUuid& id)
	{
        //this->clearLineObjects();
        foreach (BaseDrawing* line, m_currentDrawingObjects)
        {
            line->setVisible(false);
            if(line->getSettings().m_type == enDrawingObjectType::enDrawingObjectChannel)
                ((DrawingChannel*)line)->setSecondLineIsVisible(false);
            else if(line->getSettings().m_type == enDrawingObjectType::enDrawingObjectFibo)
                ((Fibbo*)line)->setLineAndTextIsVisible(false);
        }
        if(m_drawingObjectMaps.contains(id))
        {
            m_currentDrawingObjects = m_drawingObjectMaps[id];
            foreach (BaseDrawing* line, m_currentDrawingObjects)
            {
                line->setVisible(true);
                if(line->getSettings().m_type == enDrawingObjectType::enDrawingObjectChannel)
                    ((DrawingChannel*)line)->setSecondLineIsVisible(true);
                else if(line->getSettings().m_type == enDrawingObjectType::enDrawingObjectFibo)
                    ((Fibbo*)line)->setLineAndTextIsVisible(true);
            }
            this->update();
        }
        else
        {
            m_currentDrawingObjects.clear();
        }
	}

	void ChartPanel::QuotePolicyChangedSlot(const QUuid&)
	{
		this->clearLineObjects();
	}

    // Remove the indicator of the post-order dependency
	void ChartPanel::internalRemoveIndicators(int index)
	{
		std::vector<int> indexes;
		for (int i = index + 1; i < m_indicators.size(); i++)
		{
			if (m_indicators[i]->getIndicatorSettings().m_dependency == emCPDPreviousIndicatorData)
			{
				indexes.push_back(i);
			}
		}

		for (int i = indexes.size() - 1; i >= 0; i--)
		{
			delete m_indicators[i];
			m_indicators.remove(i);
		}
	}

	void ChartPanel::clearLineObjects()
	{
        foreach (BaseDrawing* line, m_currentDrawingObjects)
		{
			delete line;
		}
        m_currentDrawingObjects.clear();
	}

	void ChartPanel::handleMousePressEvent(QGraphicsSceneMouseEvent *event)
	{
		bool isDrawing = false;
		BaseDrawing* drawingObject = nullptr;
        if (m_currentDrawingObjects.size() > 0)
		{
            drawingObject = m_currentDrawingObjects.last();
			if (drawingObject->getState() != DrawFinished)
			{
				drawingObject->drawToPos(event->scenePos());
				isDrawing = true;
			}
		}
		
		if (!isDrawing)
		{
			enDrawingObjectType lineType = m_pView->getDrawingObjectType();
			if (lineType != enDrawingObjectTypeUnknown)
			{
				DrawingObjectSettings settings(lineType);
				drawingObject = Helper::CreateDrawingObject(settings, this, DrawReady);
				drawingObject->drawToPos(event->scenePos());
                m_currentDrawingObjects.append(drawingObject);
                m_drawingObjectMaps[m_pView->getInstrument().getId()].append(drawingObject);
				m_pView->DrawingObjectCreated(lineType);
				isDrawing = true;
			}
			else
			{
				//m_pView->handleMousePressEvent(event);
			}
		}

		if (isDrawing)
		{
			m_mainPainterPanel->setAcceptHoverEvents(true);
		}
	}

	void ChartPanel::handleMouseMoveEvent(QGraphicsSceneMouseEvent *event)
	{
		if ((ChartPanel*)m_pView->getPricePanel() == this && m_pView->getToolbar()->isInZoomIn())
		{
			QPointF startPos = m_mainPainterPanel->dragStartPos();
			QPointF endPos = event->pos();
			double width = std::abs(endPos.x() - startPos.x());
			double height = std::abs(endPos.y() - startPos.y());
			double x = std::min(endPos.x(), startPos.x());
			double y = std::min(endPos.y(), startPos.y());
			m_rect.setRect(0, 0, width, height);
			m_rect.setPos(x, y);
			//m_rect.setBrush(Qt::yellow);
			m_rect.setPen(QPen(Qt::yellow));
			m_rect.show();
		}
	}
	void ChartPanel::handleMouseReleaseEvent(QGraphicsSceneMouseEvent *event)
	{
		if ((ChartPanel*)m_pView->getPricePanel() == this && m_pView->getToolbar()->isInZoomIn())
		{
			QPointF pos = m_rect.pos();
			double startTime = this->Xpos2Time(pos);
			pos.setX(pos.x() + m_rect.rect().width());
			double endTime = this->Xpos2Time(pos);

			m_rect.setRect(0, 0, 0, 0);
			m_rect.hide();

			m_pView->showSpecifyRange(startTime, endTime);
		}
	}

	void ChartPanel::handleHoverMoveEvent(QGraphicsSceneHoverEvent *event)
	{
        if (m_currentDrawingObjects.size() > 0)
		{
            BaseDrawing* lastLineObject = m_currentDrawingObjects.last();
			lastLineObject->hindToPos(event->scenePos());
		}
	}

	void ChartPanel::handleWheelEvent(QGraphicsSceneWheelEvent *event)
	{
		m_pView->ZoomSlot(event->delta() > 0);
	}
}


#include "moc_ChartPanel.cpp"

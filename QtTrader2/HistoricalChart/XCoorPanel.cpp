#include "XCoorPanel.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "Common.h"
#include "JulianDate.h"
#include "../QtFramework/QsLog.h"


using namespace HistoricalChartSpace;

XCoorPanel::XCoorPanel(Config& config, ZoomController* &zoomController)
	: m_zoomController(zoomController),
	m_commonSettings(config.getCommonSettings())
{
	this->setZValue(-1.0);
}

XCoorPanel::~XCoorPanel()
{
}

void XCoorPanel::updateXCoor(BaseData& baseData)
{
	int nspace = Helper::GetMarginSpaceValue(enMarginGridXStandardSpace) * 2 / baseData.getBarDistance();
	int beingIndex = baseData.getBeginIndex();
	int endIndex = baseData.getEndIndex();

	if (nspace == 0) nspace = 1;
	int num = (endIndex - beingIndex) / nspace + 1;
	
	//QLOG_INFO("Chart") << "XCoorPanel::updateXCoor, date num:" << num
	//	<< ",beginIndex:" << beingIndex
	//	<< ",endIndex:" << endIndex
	//	<< ",nspace:" << nspace;

	m_vecDate.clear();
	for (int i = 0; i < num; ++i)
	{
		double jdate = baseData.getDate(i * nspace + beingIndex);
		m_vecDate.push_back(jdate);
	}

	if (m_vecDate.size() > 0)
	{
        double xCoorMargen = Helper::GetMarginSpaceValue(enMarginXCoordinate);
		QRectF rect = this->rect();
		QRectF xcoorRect(rect.left(), rect.top() + rect.height() - xCoorMargen, rect.width(), xCoorMargen);
		update(xcoorRect);
    }
}

void XCoorPanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
{
    double xCoorMargen = Helper::GetMarginSpaceValue(enMarginXCoordinate);
	const double margin = Helper::GetMarginSpaceValue(enMarginAdjustBound);
	QRectF rect = this->rect();
	rect.setLeft(margin + rect.left());

	if (m_commonSettings.m_isShowGrid)
	{
		QRectF gridRect = rect;
		gridRect.setHeight(rect.height() - xCoorMargen);
		gridRect.setWidth(rect.width() - Helper::GetMarginSpaceValue(enMarginYCoordinate));
		drawGrid(painter, gridRect);
	}
	QRectF xcoorRect(rect.left(), rect.top() + rect.height() - xCoorMargen, rect.width(), xCoorMargen);
	drawXCoor(painter, xcoorRect);
}

void XCoorPanel::drawGrid(QPainter* painter, QRectF& rect)
{
	const double xSpace = Helper::GetMarginSpaceValue(enMarginGridXStandardSpace);
	QPen gridPen(m_commonSettings.m_gridColor);
	gridPen.setStyle(Qt::DotLine);
	painter->setPen(gridPen);

	double rectW = rect.left() + rect.width();
	double rectTop = rect.top();
	double rectBottom = rect.top() + rect.height();
	double XPosition = rect.left() + xSpace;
	while (XPosition < rectW)
	{
		QPointF beginPoint(XPosition, rectTop);
		QPointF endPoint(XPosition, rectBottom);
		painter->drawLine(beginPoint, endPoint);
		XPosition += xSpace;
	}
}

void XCoorPanel::drawXCoor(QPainter* painter, QRectF& rect)
{    
	QPen XCoorPen(m_commonSettings.m_foreground, 1);
	painter->setPen(XCoorPen);

	double rectW = rect.left() + rect.width();
	double rectTop = rect.top();
	double XPosition = rect.left();
	const double outGrid = Helper::GetMarginSpaceValue(enMarginOutGridStandardSpace);
	const double textSpace = Helper::GetMarginSpaceValue(enMarginSpaceTextToCoordinate);
	const double xSpace = Helper::GetMarginSpaceValue(enMarginGridXStandardSpace);
	const double textHight = Helper::GetMarginSpaceValue(enMarginXTextHSpace);
	const double textWight = Helper::GetMarginSpaceValue(enMarginXTextWSpace);

	for (int i = 0; i < m_vecDate.size(); ++i, XPosition += 2 * xSpace)
	{
		QPointF yBegin(XPosition, rectTop);
		QPointF yEnd(XPosition, rectTop + outGrid);
		painter->drawLine(yBegin, yEnd);
		double value = m_vecDate[i];
		// MayBe add time Func
		QString label;
		if (value > 0)
		{
			if (i == 0)
			{
				label.append(JulianDate::JDateItemToString(value, enShowDay | enShowYear | enShowMonth, 3));
			}
			else
			{
				label.append(JulianDate::JDateItemToString(value, enShowDay | enShowMonth | enShowTime, 3));
			}
        }
		painter->drawText(XPosition, rectTop + outGrid + textSpace, textWight, textHight, Qt::AlignLeft | Qt::AlignTop, label);
	}
}

void XCoorPanel::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   #if defined (RUNTIME_OS_WIN32) || defined (RUNTIME_OS_MACX)
    if (event->buttons() == Qt::LeftButton)
    {
        m_mouseStartPos = event->pos();
    }
    return QGraphicsRectItem::mousePressEvent(event);  // comment this line to insure mouseMoveEvent work
    #endif
}

void XCoorPanel::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    #if defined (RUNTIME_OS_WIN32) || defined (RUNTIME_OS_MACX)
    if (event->buttons() == Qt::LeftButton)
    {
        qreal diff = event->pos().x() - m_mouseStartPos.x();
        if (abs(diff) > 5)
        {
            bool isZoomIn = diff > 0;
            m_zoomController->zoom(isZoomIn);
            m_mouseStartPos = event->pos();
        }
        qDebug() << "diff: " << diff;
    }
    #endif
}


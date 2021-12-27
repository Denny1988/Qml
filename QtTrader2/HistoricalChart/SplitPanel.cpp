#include "SplitPanel.h"
#include "ChartPanel.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
using namespace HistoricalChartSpace;

SplitPanel::SplitPanel(Config& config):m_prePanel(NULL),m_nextPanel(NULL),m_config(config),m_commonSettings(m_config.getCommonSettings())
{
	this->setRect(0,0,500,10);
	this->setAcceptHoverEvents(true);
	this->setFlags(ItemIsSelectable);
	this->setCursor(Qt::SplitVCursor);
}

SplitPanel::~SplitPanel()
{
}

void SplitPanel::setPrePanel(ChartPanel* panel)
{
	m_prePanel = panel;
}

void SplitPanel::setNextPanel(ChartPanel* panel)
{
	m_nextPanel = panel;
}

void SplitPanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
{
	QRectF rect = this->rect();
	QPen pen(m_commonSettings.m_foreground);
	QBrush brush(m_commonSettings.m_background);
	QPen backPen(m_commonSettings.m_background);
	painter->setPen(backPen);
	painter->setBrush(brush);
	painter->drawRect(rect);
	double len = rect.width() - Helper::GetMarginSpaceValue(enMarginYCoordinate);
	painter->setPen(pen);
	painter->drawLine(rect.left(), rect.top(), rect.left() + len, rect.top());
	painter->drawLine(rect.left(), rect.top() + rect.height(), rect.left() + len, rect.top() + rect.height());
}

void SplitPanel::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_mouseStartPos = event->pos();
	}
	return QGraphicsRectItem::mousePressEvent(event);
}

void SplitPanel::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if ((event->pos() - m_mouseStartPos).manhattanLength()
		< QApplication::startDragDistance())
		return;

	if (!(event->buttons() & Qt::LeftButton))
		return;

	if (m_prePanel == NULL || m_nextPanel == NULL) return;
    //Adjust the size of the related pannel
	QRectF preRect = m_prePanel->rect();
	QRectF nextRect = m_nextPanel->rect();
	QPointF prePos = m_prePanel->pos();
	QPointF nextPos = m_nextPanel->pos();
	QPointF currentPos = this->pos();

	double YShift = m_mouseStartPos.y() - event->pos().y();
	double YCurrent = currentPos.y();
	double YHigh = prePos.y();
	double YLow = nextPos.y() + nextRect.height() - this->rect().height();

	double preShift = YCurrent - YShift;

	if (preShift > YLow)
	{
		this->setPos(currentPos.x(), YLow);
		preRect.setHeight(YLow - prePos.y());
		m_prePanel->setRect(preRect);
		nextRect.setHeight(0.0);
		m_nextPanel->setRect(nextRect);
		m_nextPanel->setPos(nextPos.x(), YLow + this->rect().height());
	}
	else if (preShift < YHigh)
	{
		preRect.setHeight(0.0);
		m_prePanel->setRect(preRect);
		this->setPos(currentPos.x(), YHigh);

		nextRect.setHeight(nextRect.height() + nextPos.y() - YHigh - this->rect().height());

		m_nextPanel->setRect(nextRect);
		m_nextPanel->setPos(nextPos.x(), prePos.y() + this->rect().height());
	}
	else
	{
		this->setPos(currentPos.x(), preShift);

		preRect.setHeight(preRect.height() - YShift);
		m_prePanel->setRect(preRect);

		nextRect.setHeight(nextRect.height() + YShift);
		m_nextPanel->setRect(nextRect);
		m_nextPanel->setPos(nextPos.x(), nextPos.y() - YShift);
	}

    // Save overall ratio
	double preRatio = m_prePanel->getHeightRatio();
	double nextRatio = m_nextPanel->getHeightRatio();
	double oldRatio = preRatio + nextRatio;
	double currentRatio = preRect.height() / (preRect.height() + this->rect().height() + nextRect.height());
	preRatio = currentRatio * oldRatio;
	nextRatio = oldRatio - preRatio;
	m_prePanel->adjustSubItem();
	m_nextPanel->adjustSubItem();
	m_prePanel->setHeightRatio(preRatio);
	m_nextPanel->setHeightRatio(nextRatio);

	//	return QGraphicsRectItem::mouseMoveEvent(event);
}

ChartPanel* SplitPanel::getPrePanel() const
{
	return m_prePanel;
}

ChartPanel* SplitPanel::getNextPanel() const
{
	return m_nextPanel;
}

//void SplitPanel::changeCommonSettings(const CommonSettings& settings)
//{
//	m_commonSettings = settings;
//	update();
//}


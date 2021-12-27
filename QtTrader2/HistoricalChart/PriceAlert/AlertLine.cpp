#include "AlertLine.h"
#include "Cursors.h"
#include "SR.h"
#include "PricePanel.h"
#include "historicalchart.h"
#include "AlertHintLine.h"
#include <QCursor>
#include <QBrush>
#include <QBitmap>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

namespace HistoricalChartSpace
{
	static const int BidLessThan = 0;
	static const int BidGreaterThan = 1;
	static const int AskLessthan = 2;
	static const int AskGreaterThan = 3;

	AlertLine::AlertLine(PricePanel* pricePanel, PriceAlertAdapter* manager, const PriceAlertStruct& priceAlert)
		:QGraphicsLineItem(pricePanel->getMainPainterPanel()),
		m_pricePanel(pricePanel),
		m_manager(manager),
		m_priceAlert(priceAlert),
		m_lineLength(60),
		m_hintLine(nullptr),
		m_modifyAlertAction(nullptr),
		m_deleteAlertAction(nullptr)
	{
		// cursor
		QPixmap pixmap(adjust_vertical);
		QCursor cursor(pixmap, 0, 0);
		QGraphicsItem::setCursor(cursor);
	}

	AlertLine::~AlertLine()
	{
		if (m_hintLine)
		{
			delete m_hintLine;
		}
	}

	void AlertLine::updateParameter(const PriceAlertStruct& priceAlert)
	{
		m_priceAlert = priceAlert;
		update();
	}

    // To solve the problem that the AlertLine does not refresh (cannot be displayed) when the AlertLine is completely out of the view of the PricePanel after the resize during Chart resize.
	void AlertLine::ensureInView()
	{
		this->placeLine();
	}

	void AlertLine::hide()
	{
		m_pricePanel->getView()->getScene()->removeItem(this);
	}

	void AlertLine::prepareLine()
	{
		// tool tip
		QString condition;
		QString bidText = SR::get("Bid");
		QString askText = SR::get("Ask");
		switch (m_priceAlert.Condition)
		{
		case BidLessThan:
			condition = QString("%1 <").arg(bidText);
			break;
		case BidGreaterThan:
			condition = QString("%1 >").arg(bidText);
			break;
		case AskLessthan:
			condition = QString("%1 <").arg(askText);
			break;
		case AskGreaterThan:
			condition = QString("%1 >").arg(askText);
			break;
		}
		QGraphicsItem::setToolTip(condition);

		// pen
		m_pen.setColor(m_priceAlert.Color);
		m_pen.setStyle(Qt::SolidLine);
		//m_pen.setWidthF(0.5);
		QGraphicsLineItem::setPen(m_pen);

		// line
		this->placeLine();
	}

	void AlertLine::placeLine()
	{
		QRectF rect = m_pricePanel->getMainPainterPanel()->boundingRect();
		double y = m_pricePanel->Value2Y(m_priceAlert.Price.toDouble());
		double x2 = rect.left() + rect.width();
		double x1 = x2 - m_lineLength;
		m_line.setLine(x1, y, x2, y);
		QGraphicsLineItem::setLine(m_line);
	}

	void AlertLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
	{
		/* draw line */
		this->prepareLine();
		QGraphicsLineItem::paint(painter, option, widget);

		/* draw arrow */
		QPixmap redPixmap(right_arrow);
		QPixmap arrowPixmap(redPixmap.size());
		arrowPixmap.fill(m_priceAlert.Color);
		arrowPixmap.setMask(redPixmap.createMaskFromColor(Qt::transparent));
		painter->drawPixmap(m_line.x2() - 4, m_line.y2() - 3, arrowPixmap);
	}

	void AlertLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
	{
		QMenu menu(m_pricePanel->getView());
		if (!m_modifyAlertAction)
		{
			m_modifyAlertAction = new QAction(SR::get("Edit"), m_pricePanel);
			QObject::connect(m_modifyAlertAction, &QAction::triggered, [=]()
			{
				m_manager->editAlertSignal(m_priceAlert.ID);
			});
		}
		if (!m_deleteAlertAction)
		{
			m_deleteAlertAction = new QAction(SR::get("Delete"), m_pricePanel);
			QObject::connect(m_deleteAlertAction, &QAction::triggered, [=]()
			{
				m_manager->removeAlertSignal(m_priceAlert.ID);
			});
		}
		menu.addAction(m_modifyAlertAction);
		menu.addAction(m_deleteAlertAction);

		menu.exec(QCursor::pos());
	}

	void AlertLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
	{
		// to insure mouseMoveEvent work
	}

	void AlertLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
	{
		if (event->button() | Qt::LeftButton)
		{
			if (m_hintLine)
			{
				m_hintLine->show();
			}
			else
			{
				m_hintLine = new AlertHintLine(m_pricePanel, m_pen, m_lineLength);
			}
			m_hintLine->moveTo(event->pos().y());
		}
	}

	void AlertLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
	{
		int button = event->button();
		if (m_hintLine && (event->button() | Qt::LeftButton))  // TODO: bug: click mouse right button will enter following code.
		{
			double price = m_pricePanel->Y2Value(event->pos().y());
			QString priceString = m_pricePanel->getView()->getInstrument().getPriceString(price);
			if (priceString != m_priceAlert.Price)
			{
				m_manager->changeAlertPriceSignal(m_priceAlert.ID, priceString);
			}
			m_hintLine->hide();
			m_pricePanel->getMainPainterPanel()->update();
		}
	}

}

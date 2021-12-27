#include "DrawingHorizontal.h"
#include <QGraphicsSceneMouseEvent>
#include "ChartPanel.h"
#include "historicalchart.h"
#include "Settings.h"
namespace HistoricalChartSpace
{
	DrawingHorizontal::DrawingHorizontal(const DrawingObjectSettings& settings, ChartPanel* parentPanel) :BaseDrawing(parentPanel, settings), m_lineElement(NULL), m_textRect(NULL)
	{
		if (parentPanel != NULL)
		{
			initDrawElement();
			m_stage = DrawFinished;
			updateSettings();
		}
	}

	void DrawingHorizontal::drawToPos(const QPointF& scenePosPoint)
	{
		QPointF localPoint = m_parentPanel->getMainPainterPanel()->mapFromScene(scenePosPoint);
		if (m_settings.m_vecValue.size() == 1)
		{
			m_settings.m_vecValue[0] = m_parentPanel->Y2Value(localPoint.y());
			updateSettings();
			drawLine(localPoint.y());
		}
	}

	DrawingHorizontal::~DrawingHorizontal()
	{
		if(m_lineElement != NULL)
		{
			delete m_lineElement;
			m_lineElement = NULL;
		}
		if(m_textRect != NULL)
		{
			delete m_textRect;
			m_textRect = NULL;
		}
	}

	void DrawingHorizontal::updateIfNeed()
	{
		if(m_stage != DrawFinished) return;
		if(m_settings.m_vecValue.size() != 1) return;

		drawLine(m_parentPanel->Value2Y(m_settings.m_vecValue[0]));
	}

	bool DrawingHorizontal::handleMouseMove(QGraphicsItem* item, QGraphicsSceneMouseEvent *event)
	{
		DrawingElementLine* line = dynamic_cast<DrawingElementLine*>(item);
		if (line == NULL || line != m_lineElement) return false;

		QPointF point = event->pos();
		double top = m_parentPanel->rect().top();
		double bottom = top + m_parentPanel->rect().height();
		double margin = Helper::GetMarginSpaceValue(enMarginDrawingObjectToBound);
		if (point.y() < top + margin) point.setY(top + margin);
		if (point.y() > bottom - margin) point.setY(bottom - margin);


        // Prevent m_settings from being accidentally modified
		if (m_settings.m_vecValue.size() != 1)  return false;
		m_settings.m_vecValue[0] = m_parentPanel->Y2Value(point.y());

		drawLine(point.y());
		return true;
	}

	void DrawingHorizontal::initDrawElement()
	{
		m_lineElement = new DrawingElementLine(this);
		m_textRect = new DrawingElementText(this, m_parentPanel->getYCoorPanel());

		//const double width = Helper::GetMarginSpaceValue(enMarginYTextWSpace);
		//const double hight = Helper::GetMarginSpaceValue(enMarginYTextHSpace);
		//m_textRect->setRect(0, 0, width, hight);
		m_textRect->setColor(m_settings.m_colorLine, m_parentPanel->getView()->getConfig().getCommonSettings().m_background);
	}

	void DrawingHorizontal::drawLine(double y)
	{
		QRectF rect = m_parentPanel->getMainPainterPanel()->boundingRect();
		double left = rect.left();
		double right = rect.left() + m_parentPanel->rect().width();
		double margin = Helper::GetMarginSpaceValue(enMarginOutGridStandardSpace);

		double textLeft = m_parentPanel->getYCoorPanel()->boundingRect().left();
		m_lineElement->setLine(left, y, right, y);
		if (m_settings.m_vecValue.size() != 1) return;
		m_textRect->setText(QString::number(m_settings.m_vecValue[0]));
		m_textRect->setPos(textLeft + margin, y - m_textRect->rect().height() / 2);
	}

	void DrawingHorizontal::setSelectProperty(bool flag)
	{
		m_lineElement->setFlag(QGraphicsItem::ItemIsSelectable,flag);
	}

	void DrawingHorizontal::setAsBackgroundProperty(double i)
	{
		m_lineElement->setZValue(i);
	}

	void DrawingHorizontal::setVisible(bool flag)
	{
		m_lineElement->setVisible(flag);
		m_textRect->setVisible(flag);
	}

	void DrawingHorizontal::setPen(const QPen& pen)
	{
		m_lineElement->setPen(pen);
		m_textRect->setColor(pen.color(), m_parentPanel->getView()->getConfig().getCommonSettings().m_background);
	}
}

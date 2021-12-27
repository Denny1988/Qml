#include "DrawingText.h"
#include "ChartPanel.h"
#include <QGraphicsSceneMouseEvent>
#include "historicalchart.h"

namespace HistoricalChartSpace
{
	DrawingText::DrawingText(const DrawingObjectSettings& settings, ChartPanel* parent, DrawStage state)
		: BaseDrawing(parent, settings), m_editor(Q_NULLPTR)
	{
		m_rectShape = new RectShape(m_parentPanel->getMainPainterPanel());
		m_textElement = new DrawingElementText(this, m_parentPanel->getMainPainterPanel());
		m_positionNode = new DrawingElementNode(this, true);// Create this node at last to insure Node element can be moved by mouse.
		m_stage = DrawFinished;
		updateSettings();

		if (state == DrawFinished)
		{
			m_rectShape->setVisible(false);
			m_positionNode->setVisible(false);
		}
	}

	DrawingText::~DrawingText()
	{
		if (m_textElement)
			delete m_textElement;
		if (m_rectShape)
			delete m_rectShape;
		if (m_positionNode)
			delete m_positionNode;
	}

	void DrawingText::drawToPos(const QPointF& scenePosPoint)
	{
		QPointF localPoint = m_parentPanel->getMainPainterPanel()->mapFromScene(scenePosPoint);
		adjustBound(localPoint);
        //_ASSERT(m_settings.m_vecDate.size() == 1);
		m_settings.m_vecDate[0] = m_parentPanel->Xpos2Time(localPoint);
		m_settings.m_vecValue[0] = m_parentPanel->Y2Value(localPoint.y());
		updateSettings();
		this->draw(localPoint);
	}

	void DrawingText::updateIfNeed()
	{
		QPointF pos(m_parentPanel->Time2Xpos(m_settings.m_vecDate[0]), m_parentPanel->Value2Y(m_settings.m_vecValue[0]));
		this->draw(pos);
	}

	bool DrawingText::handleMouseMove(QGraphicsItem* item, QGraphicsSceneMouseEvent *event)
	{
		DrawingElementNode* node = dynamic_cast<DrawingElementNode*>(item);
		if (node == m_positionNode)
		{
			this->drawToPos(event->scenePos());
		}
		return true;
	}

	bool DrawingText::handleMouseDoubleClick(QGraphicsItem* item, QGraphicsSceneMouseEvent* event)
	{
		if (m_positionNode->isVisible())
		{
			if (item == m_positionNode)
			{
				m_positionNode->setVisible(false);
				m_rectShape->setVisible(false);
			}
			else
			{
				if (!m_editor)
				{
					m_editor = new QTextEdit(m_parentPanel->getView());
					m_editor->installEventFilter(this);
				}
				int wide = m_textElement->getTextWidth() + 15;
				int high = m_textElement->getTextHeight() + 20;
				if (wide < 150) wide = 150;
				if (high < 100) high = 100;
				QPointF point = m_positionNode->pos();
				m_editor->setGeometry(point.x(), point.y(), wide, high);
				m_editor->setFontFamily(m_settings.m_fontFamily);
				m_editor->setFontPointSize(m_settings.m_fontSize);
				m_editor->setPlainText(m_settings.m_describe);
				m_editor->show();
				m_editor->setFocus();
			}
		}
		else
		{
			m_positionNode->setVisible(true);
			m_rectShape->setVisible(true);
		}
		return true;
	}

	bool DrawingText::eventFilter(QObject* target, QEvent* event)
	{
		if (target == m_editor)
		{
			if (event->type() == QEvent::FocusOut)
			{
				m_settings.m_describe = m_editor->toPlainText();
				m_editor->clear();
				m_editor->hide();
			}
		}
		return QObject::eventFilter(target, event);
	}

	void DrawingText::setVisible(bool flag)
	{
		m_positionNode->setVisible(flag);
		m_textElement->setVisible(flag);
		m_rectShape->setVisible(flag);
	}

	void DrawingText::setPen(const QPen& pen)
	{
		m_positionNode->setPen(pen);
		m_textElement->setPen(pen);
		m_rectShape->setPen(pen);
	}

	void DrawingText::setSelectProperty(bool flag)
	{
		m_positionNode->setFlag(QGraphicsItem::ItemIsSelectable, flag);
	}

	void DrawingText::setAsBackgroundProperty(double i)
	{
		m_positionNode->setZValue(i);
		m_textElement->setZValue(i);
		m_rectShape->setZValue(i);
	}

	void DrawingText::draw(QPointF& pos)
	{
		// text
		m_textElement->setText(m_settings.m_describe);
		QRectF rect(pos.x(), pos.y(), m_textElement->getTextWidth(), m_textElement->getTextHeight());

		m_textElement->setPos(pos);
		m_textElement->setRect(rect);
		m_textElement->setRotation(m_settings.m_angle);
		m_textElement->setFont(QFont(m_settings.m_fontFamily, m_settings.m_fontSize));
		m_textElement->setColor(Qt::transparent, m_settings.m_colorLine);

		// Rect
		m_rectShape->setPos(pos);
		m_rectShape->setRect(0, 0, rect.width(), rect.height());
		m_rectShape->setRotation(m_settings.m_angle);
		
		// node
		m_positionNode->setPos(pos);
	}
}

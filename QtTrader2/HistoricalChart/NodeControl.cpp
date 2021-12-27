#include "NodeControl.h"
#include "ChartPanel.h"
#include "historicalchart.h"
#include <QGraphicsSceneMouseEvent>
#include <QPen>

namespace HistoricalChartSpace
{
	NodeControl::NodeControl(const DrawingObjectSettings& settings, ChartPanel* parentPanel) :BaseDrawing(parentPanel, settings), m_centerNode(NULL)
	{
	}

	NodeControl::~NodeControl()
	{
		foreach(DrawingElementNode* node,m_vecNode)
		{
			delete node;
		}
		delete m_centerNode;
	}

	void NodeControl::drawToPos(const QPointF& scenePosPoint)
	{
		QPointF localPoint = m_parentPanel->mapFromScene(scenePosPoint);
		if (m_stage == DrawReady)
		{
			foreach(DrawingElementNode* node, m_vecNode)
			{
				node->setPos(localPoint);
				updatePosData(node);
			}
			m_centerNode->setPos(localPoint);
			updateCenterNode();
			updateSettings();

            //If there are control points, there are at least two, so there must be
			m_stage = DrawFirstStep;
		}
		else
		{
			int index = m_stage;
			if(m_vecNode.size() < m_stage) return; 

			m_vecNode[index]->setPos(localPoint);
			updatePosData(m_vecNode[index]);

            // Drawing state plus one
			m_stage = (DrawStage)(m_stage + 1);
			if(m_settings.m_vecDate.size() <= m_stage)
			{
				updateCenterNode();
				drawLine();
				m_stage = DrawFinished;
			}
		}
	}

	bool NodeControl::handleMouseMove(QGraphicsItem* item, QGraphicsSceneMouseEvent *event)
	{
		DrawingElementNode* node = dynamic_cast<DrawingElementNode*>(item);
		if (node != NULL)
		{
			return	node->isCenterNode() ? centerNodeMove(event) : nonCenterNodeMove(event, node);
		}
		return false;
	}

	void NodeControl::initDrawElement()
	{
		int size = m_settings.m_vecDate.size();

		for(int i = 0; i < size; ++i)
		{
			DrawingElementNode* node = new DrawingElementNode(this);
			m_vecNode.append(node);
		}

		m_centerNode = new DrawingElementNode(this,true);
	}

	void NodeControl::setSelectProperty(bool flag)
	{
		foreach(DrawingElementNode* node,QVector<DrawingElementNode*>() << m_vecNode << m_centerNode)
		{
			node->setFlag(QGraphicsItem::ItemIsSelectable,flag);
		}
	}

	void NodeControl::setAsBackgroundProperty(double i)
	{
		foreach(DrawingElementNode* node,QVector<DrawingElementNode*>() << m_vecNode << m_centerNode)
		{
			node->setZValue(i);
		}
	}

	bool NodeControl::nonCenterNodeMove(QGraphicsSceneMouseEvent *event,DrawingElementNode* node)
	{
		m_centerNode->setVisible(false);
		QPointF point0 = event->pos();
		QPointF point = node->mapToParent(point0);
		adjustBound(point);
		node->setPos(point);
		updatePosData(node);
		drawLine();
		return true;
	}

	bool NodeControl::centerNodeMove(QGraphicsSceneMouseEvent *event)
	{
		QPointF point = event->pos();
		QPointF currentPoint = m_centerNode->mapToParent(point);
		QPointF oldPos = m_centerNode->pos();

		QPointF temp = currentPoint;
		adjustBound(currentPoint);
		if (temp != currentPoint)
		{
			m_centerNode->setPos(currentPoint);
		}

		QPointF pointShift = currentPoint - oldPos;

		foreach(DrawingElementNode* node, m_vecNode)
		{
			node->setPos(node->pos() + pointShift);
			updatePosData(node);
		}
		drawLine();
		return true;
	}

	void NodeControl::handleMouseRelease(QGraphicsItem* item, QGraphicsSceneMouseEvent* event)
	{
		DrawingElementNode* node = dynamic_cast<DrawingElementNode*>(item);
		if (node == NULL || node->isCenterNode()) return;
		updateCenterNode();
		m_centerNode->setVisible(true);
	}

	void NodeControl::setVisible(bool flag)
	{
		foreach(DrawingElementNode* node,QVector<DrawingElementNode*>() << m_vecNode << m_centerNode)
		{
			node->setVisible(flag);
		}
	}

	void NodeControl::setPen(const QPen& pen)
	{
		foreach(DrawingElementNode* node, QVector<DrawingElementNode*>() << m_vecNode << m_centerNode)
		{
			node->setPen(pen);
		}
	}

	void NodeControl::updatePosData(DrawingElementNode* node)
	{
		const QPointF& point = node->pos();

		double xValue = m_parentPanel->Xpos2Time(point);
		double yValue = m_parentPanel->Y2Value(point.y());
		int index = m_vecNode.indexOf(node);

		if(index < 0 || index >= m_settings.m_vecDate.size() || index >= m_settings.m_vecValue.size()) return;

		m_settings.m_vecDate[index] = xValue;
		m_settings.m_vecValue[index] = yValue;
	}

	void NodeControl::updateIfNeed()
	{
		if(m_stage != DrawFinished) return;
		if(m_vecNode.size() != m_settings.m_vecDate.size() || m_vecNode.size() != m_settings.m_vecValue.size()) return;

		bool flag = false;
		for (int i = 0; i < m_vecNode.size(); ++i)
		{
			double xPos = m_parentPanel->Time2Xpos(m_settings.m_vecDate[i]);
			double yPos = m_parentPanel->Value2Y(m_settings.m_vecValue[i]);

			QPointF currentPos(xPos,yPos);
			const QPointF& oldPos = m_vecNode[i]->pos();
			if(currentPos != oldPos)
			{
				m_vecNode[i]->setPos(currentPos);
				flag = true;
			}
		}

		if(flag || isSettingChange())
		{
			updateCenterNode();
			drawLine();
		}
	}

	void NodeControl::updateCenterNode()
	{
		if(m_vecNode.size() == 0) return;
		QPointF centerPos(0,0);

		foreach(DrawingElementNode* node,m_vecNode)
		{
			centerPos += node->pos();
		}

		centerPos /= m_vecNode.size();
		m_centerNode->setPos(centerPos);
	}

	void NodeControl::setBrushToFill(const QColor& color)
	{
	}
}

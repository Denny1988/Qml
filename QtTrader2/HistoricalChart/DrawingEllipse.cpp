#include "DrawingEllipse.h"
#include "ChartPanel.h"
namespace HistoricalChartSpace
{
	DrawingEllipse::DrawingEllipse(const DrawingObjectSettings& settings, ChartPanel* parent, DrawStage state) : NodeControl(settings, parent), m_ellipseLine(NULL)
	{
		initDrawElement();
		m_stage = state;
		updateSettings();
	}

	DrawingEllipse::~DrawingEllipse()
	{
		delete m_ellipseLine;
	}

	void DrawingEllipse::drawLine()
	{
        //1.Roughly judge whether the Node is legal or not, if it is not legal, do not draw
		if(m_vecNode.size() != 3) return;
		const QPointF& beginPos = m_vecNode[0]->pos();
		const QPointF& endPos = m_vecNode[1]->pos();
		const QPointF& onPos = m_vecNode[2]->pos();
		const QPointF& centerPos = m_centerNode->pos();

		if(onPos.x() == beginPos.x() || onPos.x() == endPos.x() || onPos.y() == beginPos.y() || onPos.y() == endPos.y())
		{
			//m_ellipseLine->setVisible(false);
			return;
		}

        // 2.Convert the coordinates to the coordinates of the standard ellipse equation
		QPointF diff = endPos - beginPos;
		double angleNew = -std::atan2(-diff.y(),diff.x())* 180.0 / 3.1415926;
        m_ellipseLine->setRotation(angleNew);// Very high performance
		m_ellipseLine->setPos(centerPos);
		QPointF stdPoint = m_ellipseLine->mapFromParent(onPos);

        // 3.Calculate the values of a and b in the elliptic equation X ^2 / a^2 + Y ^2 / b^2 = 1
		double aa = (diff.x() * diff.x() + diff.y()* diff.y()) / 4 ;
		double bb = stdPoint.y() * stdPoint.y() * aa / (aa - stdPoint.x()* stdPoint.x());

        //4.Judge whether it is legal, if it is not legal, it proves that it does not meet the drawing conditions of the ellipse, then it will not be drawn
		if(bb < 0 || bb == bb + 1)
		{
			//m_ellipseLine->setVisible(false);
			return;
		}
		double a = std::sqrt(aa);
		double b = std::sqrt(bb);

        // 5. Draw an ellipse
		QRectF ellipseRect(-a,-b,2*a,2*b);
		m_ellipseLine->setRect(ellipseRect);

        // 6.display
        //m_ellipseLine->setVisible(true); -- Causes visibility control to be ineffective
	}

	void DrawingEllipse::initDrawElement()
	{
		NodeControl::initDrawElement();
		m_ellipseLine = new EllipseShape(m_parentPanel->getMainPainterPanel());
	}
	void DrawingEllipse::setVisible(bool flag)
	{
		NodeControl::setVisible(flag);
		m_ellipseLine->setVisible(flag);
	}
	void DrawingEllipse::setPen(const QPen& pen)
	{
		NodeControl::setPen(pen);
		m_ellipseLine->setPen(pen);
	}
	void DrawingEllipse::setAsBackgroundProperty(double i)
	{
		NodeControl::setAsBackgroundProperty(i);
		m_ellipseLine->setZValue(i);
	}

	void DrawingEllipse::setBrushToFill(const QColor& color)
	{
		NodeControl::setBrushToFill(color);
		m_ellipseLine->setBrush(color);
	}

	void DrawingEllipse::updateCenterNode()
	{
		if(m_vecNode.size() != 3) return;
		const QPointF& beginPos = m_vecNode[0]->pos();
		const QPointF& endPos = m_vecNode[1]->pos();

		QPointF centerPos = (beginPos + endPos) / 2;
		m_centerNode->setPos(centerPos);
	}
}

#include "Helper.h"
#include "BaseDrawing.h"
#include "ChartPanel.h"
#include <QGraphicsSceneContextMenuEvent>
#include "historicalchart.h"
namespace HistoricalChartSpace
{
	BaseDrawing::BaseDrawing(ChartPanel* parentPanel, const DrawingObjectSettings& settings)
		:m_parentPanel(parentPanel),
		m_settings(settings),
		m_stage(DrawReady),
		m_isVisible(true),
		m_bSettingsChange(false)
	{
	}

	BaseDrawing::~BaseDrawing()
	{
		//Common::DelObjectIndex(m_settings.m_type, m_settings.m_index);
	}
	const DrawingObjectSettings& BaseDrawing::getSettings() const
	{
		return m_settings;
	}
	void BaseDrawing::setSettings(const DrawingObjectSettings& settings)
	{
		m_settings = settings;
		updateSettings();
	}

	DrawStage BaseDrawing::getState() const
	{
		return m_stage;
	}
	void BaseDrawing::setState(DrawStage state)
	{
		m_stage = state;
	}

	ChartPanel* BaseDrawing::getPanel() const
	{
		return m_parentPanel;
	}

	void BaseDrawing::showContextMenu(QGraphicsSceneContextMenuEvent *event)
	{
		m_parentPanel->popupLineObjectContextMenu(this, event);
	}

	void BaseDrawing::handleMouseRelease(QGraphicsItem* item,QGraphicsSceneMouseEvent* event)
	{
        // Null implementation, it is convenient to choose whether to use or not later
	}
	void BaseDrawing::updateVisible()
	{
		bool flag = m_settings.m_intFlagsToShow & m_parentPanel->getView()->getFrequency().getTypicalShowTime();
		setVisible(flag);
	}

	void BaseDrawing::updateSettings()
	{
		updateVisible();

		QPen pen(m_settings.m_colorLine, m_settings.m_lfLineWidth, (Qt::PenStyle)m_settings.m_intLineType);
		setPen(pen);

		bool flag = m_settings.m_bSelectAble;
		setSelectProperty(flag);

		double i = m_settings.m_bAsBackground ? -1.0 : 1.0;
		setAsBackgroundProperty(i);

		flag = m_settings.m_intFillShape & brushToFillShape_NeedBrush && m_settings.m_intFillShape & brushToFillShape_SetBrush;
		if (flag)
		{
			setBrushToFill(m_settings.m_colorLine);
		}
		else
		{
			setBrushToFill(Qt::NoBrush);
		}

		m_bSettingsChange = true;
		updateIfNeed();
		m_bSettingsChange = false;
	}

	bool BaseDrawing::isSettingChange()
	{
		return m_bSettingsChange;
	}

	void BaseDrawing::adjustBound(QPointF& point)
	{
		double margin = Helper::GetMarginSpaceValue(enMarginDrawingObjectToBound);
		QRectF rect = m_parentPanel->getMainPainterPanel()->boundingRect();
		if(point.x() < rect.left()) point.setX(rect.left());
		if(point.x() > rect.left() + rect.width()) point.setX(rect.left() + rect.width());
		if(point.y() < rect.top() + margin) point.setY(rect.top() + margin);
		if(point.y() > rect.top() + rect.height() - margin) point.setY(rect.top() + rect.height() - margin);

	}

	void BaseDrawing::setBrushToFill(const QColor& color)
	{
        //Null implementation, waiting for specific subclasses to be implemented according to requirements
	}

	void BaseDrawing::showPropertyDialog()
	{
		m_parentPanel->editDrawingObjectProperty(this);
	}

	bool BaseDrawing::handleMouseDoubleClick(QGraphicsItem* item, QGraphicsSceneMouseEvent* event)
	{
		return false;
	}

	DrawingElementLine::DrawingElementLine(BaseDrawing* baseLine)
		:QGraphicsLineItem(baseLine->getPanel()->getMainPainterPanel()), m_baseLine(baseLine)
	{
        //_ASSERT(baseLine->getPanel()->getMainPainterPanel());
		this->setFlag(QGraphicsItem::ItemIsSelectable);
		this->setCursor(Qt::ClosedHandCursor);
		this->setFlag(QGraphicsItem::ItemUsesExtendedStyleOption);
	}

	void DrawingElementLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
	{	
		bool isClickedOnLine = this->shape().contains(event->pos()); // or can refer to : http://blog.csdn.net/guxch/article/details/7612033
		if (m_baseLine != NULL && isClickedOnLine )
		{
			m_baseLine->showContextMenu(event);
		}
		else
		{
			QGraphicsLineItem::contextMenuEvent(event);
		}
	}

	void DrawingElementLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
	{
		if(m_baseLine != NULL )
		{
			if(m_baseLine->handleMouseMove(this,event)) return; 
		}
		return QGraphicsLineItem::mouseMoveEvent(event);
	}

	void DrawingElementLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
	{
        // Since the default condition is not clear when in the selected state, I will change the state secretly to make the drawing option pretend to be a non-selected state
		if(option->state & QStyle::State_Selected)
		{
			QStyleOptionGraphicsItem tempOption = QStyleOptionGraphicsItem(*option);
			tempOption.state = QStyle::State(tempOption.state & ~QStyle::State_Selected);
			return QGraphicsLineItem::paint(painter,&tempOption,widget); 
		}
		
		return QGraphicsLineItem::paint(painter,option,widget);
	}

	DrawingElementNode::DrawingElementNode(BaseDrawing* baseLine, bool isCenter)
		:QGraphicsEllipseItem(baseLine->getPanel()->getMainPainterPanel()), m_baseLine(baseLine),
		m_bIsCenter(isCenter)
	{
		QGraphicsItem *parent = baseLine == NULL ? 0 : baseLine->getPanel() == NULL ? 0 : baseLine->getPanel()->getMainPainterPanel();
        //_ASSERT(parent);
		this->setFlag(QGraphicsItem::ItemIsSelectable);
		this->setCursor(Qt::ClosedHandCursor);
		double d = Helper::GetMarginSpaceValue(enMarginDrawingObjectNodeDia);
        //Fill white
        this->setBrush(Qt::white);
		this->setRect(-d / 2, -d / 2, d, d);
		this->setFlag(ItemUsesExtendedStyleOption);
	}

	void DrawingElementNode::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
	{
		if(m_baseLine != NULL)
		{
			m_baseLine->showContextMenu(event);
		}
	}

	bool DrawingElementNode::isCenterNode() const
	{
		return m_bIsCenter;
	}

	void DrawingElementNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
	{
		if (!m_baseLine->handleMouseDoubleClick(this, event))
		{
			m_baseLine->showPropertyDialog();
		}
	}

	void DrawingElementNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
	{
		if(m_baseLine != NULL)
		{
			if(m_baseLine->handleMouseMove(this,event)) return; 
		}
		return QGraphicsEllipseItem::mouseMoveEvent(event);
	}

	void DrawingElementNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
	{
		if(m_baseLine != NULL)
		{
			 m_baseLine->handleMouseRelease(this,event);
		}
		return QGraphicsEllipseItem::mouseReleaseEvent(event);
	}

	void DrawingElementNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
	{
        // ince the default condition is not clear when in the selected state, I will change the state secretly to make the drawing option pretend to be a non-selected state
		if(option->state & QStyle::State_Selected)
		{
			QStyleOptionGraphicsItem tempOption = QStyleOptionGraphicsItem(*option);
			tempOption.state = QStyle::State(tempOption.state & ~QStyle::State_Selected);
			return QGraphicsEllipseItem::paint(painter,&tempOption,widget); 
		}

		return QGraphicsEllipseItem::paint(painter,option,widget);
	}

	DrawingElementText::DrawingElementText(BaseDrawing* baseLine, QGraphicsItem* parent) :QGraphicsRectItem(parent), m_baseLine(baseLine)
	{
		this->setFlag(QGraphicsItem::ItemIsSelectable); // To insure mouseDoubleClickEvent work.
		m_text = "";
		m_foreground = Qt::black;
		m_background = Qt::white;
		m_font.setFamily("Arial");
		m_font.setPointSize(9);
	}

	void DrawingElementText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
	{
		QRectF rect = this->rect();
		painter->setPen(m_background);
		painter->setBrush(m_background);
		painter->drawRect(rect);

		painter->setPen(m_foreground);
		painter->setFont(m_font);
		painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, m_text);

		//if (m_text.contains('\n'))
		//{
		//	QFontMetrics fontMetrics(m_font);
		//	QStringList lineTexts = m_text.split('\n');
		//	int lineHeight = fontMetrics.height();
		//	for (int i = 0; i < lineTexts.count(); i++)
		//	{
		//		painter->drawText(0, lineHeight*(i + 1), lineTexts[i]);
		//	}
		//}
		//else
		//{
		//	painter->drawText(rect.x(), rect.y() + 12, m_text);
		//}
	}

	void DrawingElementText::setText(const QString& text)
	{
		m_text = text;
		this->calculateTextSize();
		this->setRect(0, 0, m_textWidth, m_textHeight);
	}

	void DrawingElementText::setColor(const QColor& background,const QColor& foreground)
	{
		m_background = background;
		m_foreground = foreground;
	}

	void DrawingElementText::setFont(const QFont& font)
	{
		m_font = font;
		this->calculateTextSize();
		this->setRect(0, 0, m_textWidth, m_textHeight);
	}

	int DrawingElementText::getTextWidth()
	{
		return m_textWidth;
	}

	int DrawingElementText::getTextHeight()
	{
		return m_textHeight;
	}

	void DrawingElementText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
	{
		m_baseLine->handleMouseDoubleClick(this, event);
	}

	void DrawingElementText::calculateTextSize()
	{
		QFontMetrics fontMetrics(m_font);
		int lines = m_text.count('\n') + 1;
		m_textHeight = fontMetrics.height() * lines;
		if (lines == 1)
		{
			m_textWidth = fontMetrics.width(m_text);
		}
		else
		{
			m_textWidth = 0;
			QStringList lineTexts = m_text.split('\n');
            foreach (QString lineText, lineTexts)
			{
				int width = fontMetrics.width(lineText);
				if (m_textWidth < width)
				{
					m_textWidth = width;
				}
			}
		}
	}

}
//#include "moc_BaseLine.cpp"

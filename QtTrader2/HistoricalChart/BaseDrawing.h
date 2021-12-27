#pragma once
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include "Settings.h"
#include <QPainter>
namespace HistoricalChartSpace
{
	class ChartPanel;
	class HistoricalChart;
	enum DrawStage
	{
		DrawReady,
		DrawFirstStep,
		DrawSecondStep,
		DrawThirdStep,
		DrawFinished = DrawThirdStep + 0x00FF,
		DrawNoDrwaingObject
	};
	class BaseDrawing //: public QObject
	{
		//Q_OBJECT
	public:
        // Normally parent is NULL during initialization and no drawing element is created
        BaseDrawing(ChartPanel* parentPanel, const DrawingObjectSettings& settings);
		virtual ~BaseDrawing();
		const DrawingObjectSettings& getSettings() const;
		void setSettings(const DrawingObjectSettings& settings);
		DrawStage getState() const;
		void setState(DrawStage state);
		ChartPanel* getPanel() const;
		void showContextMenu(QGraphicsSceneContextMenuEvent *event);
		void updateSettings();
		void updateVisible();
		void adjustBound(QPointF& point);
		bool isSettingChange();

        //Step-by-step drawing
        virtual void drawToPos(const QPointF& scenePosPoint) = 0;
        //Step-by-step drawing tips
        virtual void hindToPos(const QPointF& scenePosPoint){}
        //Call update, in order to improve performance, when the drawing data has not changed, do not do other processing such as update, return directly, the system will update
        virtual void updateIfNeed() = 0;
		virtual bool handleMouseMove(QGraphicsItem* item, QGraphicsSceneMouseEvent *event) = 0;
		virtual void handleMouseRelease(QGraphicsItem* item, QGraphicsSceneMouseEvent* event);
		virtual bool handleMouseDoubleClick(QGraphicsItem* item, QGraphicsSceneMouseEvent* event);

        //Set specific properties
		virtual void setVisible(bool flag) = 0;
		virtual void setPen(const QPen& pen) = 0;
		virtual void setSelectProperty(bool flag) = 0;
		virtual void setAsBackgroundProperty(double i) = 0;
        // Not everything needs to be implemented, only closed graphics with filling requirements are required to be implemented
        virtual void setBrushToFill(const QColor& color);
		void showPropertyDialog();
	protected:
		DrawingObjectSettings m_settings;
		ChartPanel* m_parentPanel;
		DrawStage m_stage;
        bool m_isVisible;

	private:
        // The subclass does not change directly, but calls the isSettingChange() method
        bool m_bSettingsChange;

	};

    // The basic components of the line segment are composed of lines and control points
	class DrawingElementLine : public QGraphicsLineItem
	{
	public:
		DrawingElementLine(BaseDrawing* baseLine);
		void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
	private:
		BaseDrawing*	m_baseLine;
	};

	class DrawingElementNode : public QGraphicsEllipseItem
	{
	public:
		DrawingElementNode(BaseDrawing* baseLine,bool isCenterNode = false);
		void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
		bool isCenterNode() const;

	protected:
		virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

	private:
		BaseDrawing*	m_baseLine;
		bool		m_bIsCenter;
	};

	class DrawingElementText : public QGraphicsRectItem
	{
	public:
		DrawingElementText(BaseDrawing* baseLine, QGraphicsItem* parent);
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
		void setText(const QString& text);
		void setColor(const QColor& background, const QColor& foreground);
		void setFont(const QFont& font);
		int getTextWidth();
		int getTextHeight();
	protected:
		virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
	private:
		void calculateTextSize();
	private:
		BaseDrawing* m_baseLine;
		QString m_text;
		QColor	m_background;
		QColor	m_foreground;
		QFont   m_font;

		int m_textHeight;
		int m_textWidth;
	};

    //Basic shape, need to overload paint function to modify QPainter properties
	class EllipseShape : public QGraphicsEllipseItem
	{
	public:
		EllipseShape(QGraphicsItem* parent = 0):QGraphicsEllipseItem(parent){}
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
		{
			painter->setCompositionMode(QPainter::CompositionMode_Exclusion);
			return QGraphicsEllipseItem::paint(painter,option,widget);
		}
	};
	class RectShape : public QGraphicsRectItem
	{
	public:
		RectShape(QGraphicsItem* parent = 0):QGraphicsRectItem(parent){}
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
		{
			painter->setCompositionMode(QPainter::CompositionMode_Exclusion);
			return QGraphicsRectItem::paint(painter,option,widget);
		}
	};

	class PolygonShape : public QGraphicsPolygonItem
	{
	public:
		PolygonShape(QGraphicsItem* parent = 0):QGraphicsPolygonItem(parent){}
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
		{
			painter->setCompositionMode(QPainter::CompositionMode_Exclusion);
			return QGraphicsPolygonItem::paint(painter,option,widget);
		}
	};
}

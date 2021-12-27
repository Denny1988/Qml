#pragma  once
#include <QGraphicsRectItem>
#include <QObject>
#include <QAction>
#include "Common.h"
#include "BaseData.h"
#include "Settings.h"
#include "Indicator/Indicator.h"

//#ifndef WIN32
//    #include <QGesture>
//    #include <QTapAndHoldGesture>
//    #include <QGestureEvent>
//    #include <QTapAndHoldGesture>
//#endif

namespace HistoricalChartSpace
{
	class SplitPanel;
	class Config;
	class BaseDrawing;
	class MainPainterPanel;
	class YCoorPanel;
	class IndicatorPanel;
	class HistoricalChart;
    class ChartPanel :public QObject, public QGraphicsRectItem
	{
		Q_OBJECT

	public:
		ChartPanel(HistoricalChart* pView, Config& config, BaseData& baseData);
		virtual ~ChartPanel();
		void showYGrid();
		void shiftData();
	
		void setPreSplitPanel(SplitPanel* splitPanel);
		void setNextSplitPanel(SplitPanel* splitPanel);
		SplitPanel* getPreSplitPanel();
		SplitPanel* getNextSplitPanel();

		void addIndicator(Indicator* indicator);
		void extendIndicatorDataBuffer();
		void calculateIndicators(int beginIndex, int endIndex);

		bool removeLineObject(BaseDrawing* line);
        void removeIndicator(Indicator* indicator);
		void updateAllDrawingObject();
		void updateAllDrawingObjectVisible();

		void popupLineObjectContextMenu(BaseDrawing* line, QGraphicsSceneContextMenuEvent* event);
		void editDrawingObjectProperty(BaseDrawing* drawingObject);

		void setHeightRatio(double ratio);
		double getHeightRatio();
		enPanelType getPanelType() const;
		virtual void refresh() = 0;
		void moveChart(int pixels);
		void handleMousePressEvent(QGraphicsSceneMouseEvent *event);
		void handleMouseMoveEvent(QGraphicsSceneMouseEvent *event);
		void handleMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		void handleHoverMoveEvent(QGraphicsSceneHoverEvent *event);
		void handleWheelEvent(QGraphicsSceneWheelEvent *event);
		void adjustSubItem();
		QGraphicsRectItem* getMainPainterPanel() const;
		QGraphicsRectItem* getYCoorPanel() const;
		void saveSettings();
        void applySettings(QVector<IndicatorSettings>& indicatorSettings, QMap<QUuid, QVector<DrawingObjectSettings>>& lineObjectMaps);

		HistoricalChart* getView();

        // Get drawing information
		double Y2Value(double y);
		double Value2Y(double value);
		double Xpos2Time(const QPointF& point);
		double Time2Xpos(const double time);
		const QColor& getForeground() const;

        //Child drawing information
		void updateUI();
		virtual void paintMainPanel(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */) = 0;
		virtual void paintYCoorPanel(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */) = 0;

        const QList<BaseDrawing*>& getDrawingObjects(){ return m_currentDrawingObjects; }
		// for test dump data
		const QVector<Indicator*>& getIndicators(){ return m_indicators; }
        int getPanelWidth() { return m_PanelWidth; }


	protected:
		double calculateMaxValue();
		double calculateMinValue();
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
		void paintIndicators(QPainter *painter);

//        #ifndef WIN32
//            bool event(QEvent *event) Q_DECL_OVERRIDE;
//            bool gestureEvent(QGestureEvent *event);
//            void tapAndHoldTriggered(QTapAndHoldGesture* gesture);
//        #endif

	protected:
        // Determined by the subclass, initialized by the subclass
        enPanelType	m_PanelType;
		SplitPanel*	m_preSPlitPanel;
		SplitPanel*	m_nextSplitPanel;
		Config&	m_config;
		BaseData& m_baseData;
		const CommonSettings& m_commonSettings;
		MainPainterPanel* m_mainPainterPanel;
		YCoorPanel* m_yCoorPanel;
        // Global scale
        double m_heightRatio;
        int m_PanelWidth;

	protected:
		PaintArgs m_PaintArgs;
		double m_maxValue;

		QVector<Indicator*> m_indicators;
        QList<BaseDrawing*> m_currentDrawingObjects;
        QMap<QUuid, QList<BaseDrawing*>> m_drawingObjectMaps;

		QAction* m_indicatorPropertyAction;
		QAction* m_delIndicatorAction;
		HistoricalChart* m_pView;

	private slots:
		void ShowDrawingObjectPropertySlot();
		void DeleteLineObjectSlot();
		void ShowIndicatorPropertySlot();
		void DeleteIndicatorSlot();
		
		void InstrumentChangedSlot(const QUuid&);
		void QuotePolicyChangedSlot(const QUuid&);
	
    private:
		void deleteIndicator(int index);
		inline void internalRemoveIndicators(int index);
		void clearLineObjects();
		virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

	private:
		QAction*      m_DrawingObjectPropertyAction;
		QAction*      m_delLineObjectAction;
		BaseDrawing*  m_currentEditLine;
		RectShape	  m_rect;
	};
}

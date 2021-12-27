#ifndef HISTORICALCHART_H
#define HISTORICALCHART_H

#include "historicalchart_global.h"
#include <QGraphicsView>
#include "Common.h"
#include "BaseData.h"
#include <QGraphicsScene>
#include <QVector>
#include <vector>
#include "Config.h"
#include <QAction>
#include <QComboBox>
#include <QToolBar>
#include <QLabel>
#include <QTimer>
#include "Instrument.h"
#include <QSet>
#include "SystemTime.h"
#include "ZoomController.h"
#include "InfoLabel.h"
#include "Template/TemplateManager.h"
#include "QuickPlacer.h"
#include "QuickModifier.h"
#include "TradeHelper/TradeHelper.h"
#include "TradeHelper/OrderLineManager.h"
#include "CursorPosition.h"
#include "PriceAlert/PriceAlertAdapter.h"
#if defined(RUNTIME_OS_IPHONE) || defined(RUNTIME_OS_ANDROID)
    #include <QGestureEvent>
    #include <QPinchGesture>
#endif

namespace HistoricalChartSpace
{
	class PricePanel;
	class SplitPanel;
	class XCoorPanel;
	class HistoricalDataSource;
	class Crosshair;
	class IndicatorPanel;
    class ToolBar;
    class IndicatorSettingDialog;
    class IndicatorSettingPanel;

	class HISTORICALCHART_EXPORT HistoricalChart : public QGraphicsView
	{
		Q_OBJECT
    signals:
		void InstrumentChangedSignal(const QUuid& instrumentId);
		void QuotePolicyChangedSignal(const QUuid& quotePolicyId);
		void DataProcessedSignal(bool isBaseDataUpdated, bool isFixPeriodData, bool isInitialData);
        void UserSetChartColorSignSignal();
#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
        void moveChartFullScreenSignal(int moveY);
#endif

	public:
		friend class Crosshair;
		friend class ZoomController;
		friend class TradeHelper;
		friend class InfoLabel;
        friend class ToolBar;
        friend class IndicatorSettingDialog;

        HistoricalChart(QWidget *parent, QWidget* dialogParent, HistoricalDataSource* dataSource, QString& templateFilePath, const QString& language, QColor& upColor, QColor& downColor,
                        bool isBlackTheme = false, bool isChangeChartToolbarByChangeSystemTheme = false, QString merchantPath = "",
                        double pixelSizeRatio = 1.0, bool horizontalScreen = false);
		~HistoricalChart();

		QWidget* getDialogParent();
		QString getConfigXml();
		void loadConfigXml(QString& configXml);
		void applyTemplate(QString& configXml);

		BaseData& getBaseData();
		PricePanel* getPricePanel();
		QGraphicsScene* getScene();
		PriceAlertAdapter* getPriceAlertManager();
		Calculator* getCalculator();

		QString getTitle();
		const Instrument& getInstrument();
		QUuid getQuotePolicyId();
		KeyDescription getQuotePolicy();
		int getDecimalPlaces();
		Frequency getFrequency();
		void setQuickPlacer(QuickPlacer* placer);
		void setQuickModifier(QuickModifier* modifier);

		void arrangePanels();
		QRectF getViewPortRect();
		enDrawingObjectType getDrawingObjectType();
		Config& getConfig();
		QGraphicsItem* getXCoordinate() const;
		bool isInFixPeriodMode();
		void showSpecifyRange(double startTime, double endTime);

		void setFrequency(const Frequency& frequency);
		void showGrid();
		void shiftData();
		void moveChartPixels(int pixels);
		void changePriceStyle(enPriceType type);
		void showCommonMenu(QGraphicsSceneContextMenuEvent* event, bool fromPricePanel);
        //void showCommonMenu(QTapAndHoldGesture* event, bool fromPricePanel);
		void addOrder(const OrderInfo& order);
		void removeOrder(const QUuid& orderId);
		void clearOrders();
		void removeIndicatorPanel(IndicatorPanel*);
		void updateOrderLines();
		// Callback functions:
		void UpdateQuotePolicy(int callId, QList<KeyDescription>&);
		void UpdateHistoricalData(int callId, QSharedPointer<vector<PrimHistoricalData>>);
		void setSystemTime(int callId, double currentTime);
		void LastBarDataCome(int callId, QSharedPointer<vector<PrimHistoricalData>> datas);
		const QVector<IndicatorPanel*>& getIndicatorPanels(){ return m_indicatorPanels; }
		void test();
		ToolBar* getToolbar();

		void OnThemeChanged(bool isBlack);
        void OnThemeChanged(bool isBlack, bool userSetChartColorSign, QColor bgColor, QColor fontColor);
        void DrawingObjectCreated(enDrawingObjectType type);
		void handleMousePressEvent(QGraphicsSceneMouseEvent *event);
        Indicator* AddIndicator(int indicatorType);
#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
        void setPixelSizeRatio(double pixelSizeRatio) { m_pixelSizeRatio = pixelSizeRatio; }
        void AddIndicatorToIndicatorSettingPanel(bool isMainIndicator, enIndicatorType indicatorType);
        void RemoveIndicatorToIndicatorSettingPanel(bool isMainIndicator, enIndicatorType indicatorType);
        bool isHorizontalScreen() { return m_horizontalScreen; }
        int getChartFullScreenY() { return m_ChartFullScreenY; }
        void showInfoPanel(bool isShowLabelInfo);
        void showCrossHair(bool isCrossHair);
        void setDialogParent(QWidget* dialogParent);
#endif
        void changeFrequency(const Frequency& frequency);

	public slots:
		void DataProcessedSlot(bool isBaseDataUpdated, bool isFixPeriodData, bool isInitialData);
		void RealTimeDataComeSlot(const PrimRealTimeData&);
		void SetQuotePolicySlot(int);
		void SetInstrumentSlot(int);
		void SetFrequencySlot(int);
		void ShowGridSwitchSlot();
		void AutoScrollSwitchSlot();
		void ShiftDataSwitchSlot();
		void SetPriceChartSlot(int);
		void SetPeriodSlot(int);
		void OnZoomInSlot();
		void OnZoomOutSlot();
		void HideToolBarSlot();
		void AddIndicatorSlot();
        void AddIndicatorSlot(int indicatorType);
		void ShowCommonPropertySlot();
		void RefreshSlot();
		void ShowIndicatorListSlot();
		void ShowDrawingObjectListSlot();
		void LabelInfoSwitchSlot();
		void CrosshairSwitchSlot();
		void QuickPlacerSwitchSlot(bool checked);
		void ShowExecutedOrderSwitchSlot(bool checked);
		void ShowPendingOrderSwitchSlot(bool checked);
		void ChartLagCheckSlot();
		void CreatorDrawingObjectSlot();
		void CreateTextSlot(bool checked);
        // Interact with Item
		void ChangeTradeStateSlot(bool paused);
		void ChartDataNotifySlot(PrimHistoricalData data);
		void ChangeUpDownColorSlot(QColor& upColor, QColor& downColor);
        void ZoomSlot(bool isZoomIn);
        void ClickInstrumentSwitchChartSlot(QUuid instrumentId);

        #if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
            void ShowIndicatorSettingDialogSlot();
            void ChangeMainIndicatorInfoSlot();
            void ChangeSecondaryIndicatorInfoSlot();
            void initIndicatorSettingPanelSlot();            
        #endif

	protected:
		void resizeEvent(QResizeEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void keyPressEvent(QKeyEvent *event);
		virtual void timerEvent(QTimerEvent *event) override;

        #if defined(RUNTIME_OS_IPHONE) || defined(RUNTIME_OS_ANDROID)
            void mousePressEvent(QMouseEvent *event);
            bool event(QEvent *event) Q_DECL_OVERRIDE;
            bool gestureEvent(QGestureEvent *event);
            void pinchTriggered(QPinchGesture*);
            void calIndicatorInfoValue(QPointF& scenePoint);
        #endif


	private:
		void exportData(); // method for debug
		void requestLastBars(double beginTime);
		void synchronizeSystemTime();
		void updateLastBars(vector<PrimHistoricalData>* datas, bool isAppend = false);
		void resizePanelsRect(const QRectF& rect);
		void updateChart(bool isUpadteEndIndex);
		void createActions();
		void initializePanels();
		void calculateIndicatorsData(int beginIndex, int endIndex);
		void addSplitPanel(IndicatorPanel* panel);
		void moveChartBars(int bars);
		void requestChartData(bool isRequstInitData, double endTime = 0, int size = 0);
		void requestQuotePolicies();
		ChartProperty& getChartProperty();
		void setIsGettingChartData(bool isChanging);
		void startTimerWithRandDelay(int interval);
        void setInfoPanelContent();

	private:
		HistoricalDataSource*   m_dataSource;
		const QList<Instrument>& m_listInstrument;
		QList<KeyDescription>   m_listQuotePolicy;
		const QList<Frequency>& m_listFrequency;
		Instrument				m_instrument;
		KeyDescription          m_quotePolicy;
		Frequency				m_frequency;
		BaseData                m_baseData;
		XCoorPanel*				m_XCoorPanel;
		PricePanel*				m_pricePanel;
		QGraphicsScene*			m_scene;
		Config					m_config;
		Crosshair*				m_crosshair;
		QVector<IndicatorPanel*>	m_indicatorPanels;
		InfoLabel* m_infoLabel;
		enDrawingObjectType            m_drawingObjectType;
		QTimer*						m_timer;

        #if defined(RUNTIME_OS_IPHONE) || defined(RUNTIME_OS_ANDROID)
            qreal m_currentStepScaleFactor;
            double m_pixelSizeRatio;
            IndicatorSettingPanel*				m_mainIndicatorSettingPanel;
            IndicatorSettingPanel*				m_secondaryIndicatorSettingPanel;
            bool m_horizontalScreen;
            int m_ChartFullScreenY;
            bool m_isZoomIn;
        #endif

	private:
		QAction* m_toolbarSwitchAction;
		QAction* m_commonPropertyAction;
		QAction* m_refreshAction;
		QAction* m_indicatorListAction;
		QAction* m_drawingObjectListAction;

		// call ids:
		int m_quotePolicesRequestCallId;
		int m_initDataRequestCallId;
		int m_lastBarRequestCallId;
		int m_fixedPeriodDataRequestCallId;
		int m_systemTimeRequestCallId;
		int m_appendDataRequestCallId;
		double m_endTimeOfAppendDataRequest;

		static const int m_RetrieveBarCount;

		bool m_tradePaused;
		SystemTime* m_systemTime;
		ZoomController* m_zoomController;
		TemplateManager* m_templateManager;
		TradeHelper* m_tradeHelper;
		OrderLineManager* m_orderLineManager;
		CursorPosition* m_cursorPosition;
        ToolBar* m_toolbar;
		PriceAlertAdapter* m_priceAlertAdapter;
		Calculator* m_calculator;

		QWidget* m_dialogParent;
		int m_requestQuotePolicyTimerId;
		int m_requestInitDataTimerId;
		QString m_language;
        QString m_merchantPath;
        #if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
            int m_InitIndicatorSettingPanelTimerId;
        #endif

         bool m_userClickAtProductCode;

         bool m_isGettingChartData;
         ChartProperty m_chartProperty;
         PeriodType m_chartPeriodType;
         bool m_inFixPeriodMode;

         QPoint m_lastViewPoint;
	};
}


#endif // HISTORICALCHART_H

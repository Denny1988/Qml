#pragma  once
#include <QToolBar>
#include <QAction>
#include "Common.h"
#include "Config.h"
#include "Instrument.h"
#include "Template/TemplateManager.h"
#include <QComboBox>
#include <QMouseEvent>
#include <QLabel>

namespace HistoricalChartSpace
{
	class HistoricalChart;
	class HISTORICALCHART_EXPORT ToolBar : public QToolBar
	{
		friend class HistoricalChart;
	public:
		ToolBar(HistoricalChart* pView, bool isBlackTheme);
		~ToolBar();
        void createToolBar(const QList<Instrument>& instruments, const QList<Frequency>& frequencies, TemplateManager* templateManager, bool isChangeChartToolbarByChangeSystemTheme = false);

		void switchVisible();
		void resetPeriodComboBox(const Frequency& currentFrequency);
        void applySettings(Config& config, const Instrument& currentInstrument, const Frequency& currentFrequency, bool userClickAtProductCode);
		void updateCommonSettings(const CommonSettings& settings);
		void makeComboBoxConnections();
		void updateQuotePolicy(const QList<KeyDescription>& quotePolicies, KeyDescription& currentQuotePolicy);
		void changeDrawType(enPriceType drawType);
		void OnChartResize(int chartWidth);
		void OnGettingChartData(bool isGettingChartData);
		void setZoomState(bool isMinimalState, bool isMaximalState);

		//void changeZoomInStyle(bool isInFixPeriodMode);
		bool isInZoomIn();
		void OnZoomInEnd();

		void OnThemeChanged(bool isBlack);
		void test();
		void DrawingObjectCreated(enDrawingObjectType lineType);
		bool IsChartDestoryed() { return m_chartDestoryed; }

        void mouseReleaseEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);

        int getCurrentWidth();
        int getCurrentInstrumentIndex();
		int getDisplayPeriod();

	private:
		void createActions();
		//virtual void mousePressEvent(QMouseEvent *) override;
		//virtual void mouseMoveEvent(QMouseEvent *) override;
		//virtual void mouseDoubleClickEvent(QMouseEvent *) override;
		//void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
		virtual bool eventFilter(QObject *, QEvent *) override;
        QIcon getResQIcon(QString fileName);

    private:
		HistoricalChart* m_pView;
		//QToolBar* m_toolbar;
		bool m_chartDestoryed;

		QAction* m_gridSwitchAction;
		QAction* m_crosshairSwitch;
		QAction* m_autoScrollSwitch;
		QAction* m_shiftDataSwitch;
		QAction* m_zoomOutAction;
		QAction* m_zoomInAciton;
		QAction* m_labelInfoSwitch;
		QAction* m_quickPlacerSwitch;
		QAction* m_showExecutedOrderSwitch;
		QAction* m_showPendingOrderSwitch;
		QAction* m_drawTextAction;

		QAction* m_trendIndicatorAction;
		QAction* m_oscillatorsIndicatorAction;
		QAction* m_volumesIndicatorAction;
		QAction* m_othersIndicatorAction;

		QList<QAction*> m_trendIndicatorActions;
		QList<QAction*> m_oscillatorsIndicatorActions;
		QList<QAction*> m_volumesIndicatorActions;
		QList<QAction*> m_othersIndicatorActions;

		QList<QAction*> m_listDrawingObjectAction;
		QAction* m_indicatorsAction;
		QAction* m_drawAction;
		QAction* m_templatesAction;

		QComboBox* m_instrumentComboBox;
		QComboBox* m_quotePolicyComboBox;
		QAction* m_quotePolicyComboBoxAction;
		QComboBox* m_frequencyComboBox;
		QComboBox* m_graphTypeComboBox;
		QComboBox* m_periodComboBox;
        QLabel* m_markLabel;
		int m_toolbarWidth;

		// for drag
		QPoint m_startPos;    // mouse pos
		QPoint m_basePoint;   // (left, top) of QWidget
		bool m_isMovedByUser;
		
		bool m_isBlackTheme;
		QWidget* m_lastWidget;
        bool m_isChangeChartToolbarByChangeSystemTheme;
	};
}

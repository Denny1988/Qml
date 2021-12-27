#pragma once
#include <QWidget>
#include "RealtimeDataSource.h"
#include "Settings.h"
#include <QAction>
namespace HistoricalChartSpace
{
	class HISTORICALCHART_EXPORT RealTimeChart : public QWidget
	{
		Q_OBJECT
	public:
		RealTimeChart(RealtimeDataSource* dataSource, RealTimePriceSettings settings, QWidget* parent = 0);
		~RealTimeChart();

		void showAskLineOnly();
		void showBidLineOnly();
		void showBidAskLines();

		const RealTimePriceSettings& getSettings();
        void OnThemeChanged(bool userSetRealTimeChartColorSign, QColor bgColor, QColor fontColor);

	protected:
		virtual void paintEvent(QPaintEvent *event) override;
		virtual void mousePressEvent(QMouseEvent *) override;
		virtual void mouseMoveEvent(QMouseEvent *event) override;
		virtual void contextMenuEvent(QContextMenuEvent *event) override;
		virtual void keyPressEvent(QKeyEvent *) override;

    signals:
        void UserSetRealTimeChartColorSignSignal();

	public slots:
		void RealTimeDataComeSlot(const PrimRealTimeData&);
		void UpdateRealTimeDataSlot(const QList<PrimRealTimeData>&);
		void HandleSettingsSlot();

	private:
		enum drawDataType
		{
			drawAsk,
			drawBid
		};
		void drawGrid(QPainter& painter, QRectF rect);
		void drawXCoordinate(QPainter& painter, QRectF rect);
		void drawYCoordinate(QPainter& painter, QRectF rect, double minValue, double minY, double scaleY);
		void drawDataLine(QPainter& painter, QRectF rect, double minValue, double minY, double scaleY, drawDataType type);
		void drawCrosshair(QPainter& painter, QRectF rect, double minValue, double minY, double scaleY);
		void getMaxMinValue(double& max, double& min);
		bool isInXCoorPanel(QPoint pos);
		bool isInYCoorPanel(QPoint pos);
		void zoomIn();
		void zoomOut();

	private:
		QList<PrimRealTimeData>	m_listRealTimeData;
		RealtimeDataSource*     m_dataSource;
		RealTimePriceSettings	m_settings;
		QAction*				m_actionHandleSetting;
		int m_decimalPlaces;
		const double m_gridUnitXLength;
		int m_verticalGridLineCount;
		int m_drawDataCount;
		const int m_maxDataCount;
		const double m_XAxisHeight;
		const double m_YAxisWidth;
		double m_adjustYScale;

		QList<int> m_dataUnitLengths;
		int m_zoomIndex;
		QPoint m_startMovePos;
	};
}


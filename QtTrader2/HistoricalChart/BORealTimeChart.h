#pragma once
#include <QWidget>
#include "BORealtimeDataSource.h"
#include "Settings.h"
#include <QAction>
namespace HistoricalChartSpace
{
	class HISTORICALCHART_EXPORT BORealTimeChart : public QWidget
	{
		Q_OBJECT
	public:
		BORealTimeChart(BORealtimeDataSource* dataSource, BORealTimePriceSettings settings, QWidget* parent = 0);
		~BORealTimeChart();
		
		void setBOInfo(QString BOCountdown, QString BOFloatPLInString, double BOStartTime, double BOEndTime, QString BOExecutePrice, bool isUp);
		BORealTimePriceSettings	getBORealTimePriceSettings() { return m_settings; }

	protected:
		virtual void paintEvent(QPaintEvent *event) override;
		virtual void mousePressEvent(QMouseEvent *) override;
		virtual void mouseMoveEvent(QMouseEvent *event) override;
		virtual void contextMenuEvent(QContextMenuEvent *event) override;
		virtual void keyPressEvent(QKeyEvent *) override;

	public slots:
		void RealTimeDataComeSlot(const PrimBORealTimeData&);
		void UpdateRealTimeDataSlot(const QList<PrimBORealTimeData>&);
		void HandleSettingsSlot();
		void resetBOStartTimeAndEndTimeSlot();

	private:
		enum drawDataType
		{
			drawAsk,
			drawBid
		};
		void drawGrid(QPainter& painter, QRectF rect);
		void drawXCoordinate(QPainter& painter, QRectF rect);
		void drawYCoordinate(QPainter& painter, QRectF rect, double minValue, double minY, double scaleY);
		void drawDataLine(QPainter& painter, QRectF rect, double minValue, double minY, double scaleY);
		void drawCrosshair(QPainter& painter, QRectF rect, double minValue, double minY, double scaleY);
		void drawBOCountdown(QPainter& painter, QRectF rect);		
		void drawBOExecutePrice(QPainter& painter, QRectF rect, double minValue, double minY, double scaleY);
		void getMaxMinValue(double& max, double& min);
		bool isInXCoorPanel(QPoint pos);
		bool isInYCoorPanel(QPoint pos);
		void zoomIn();
		void zoomOut();

	private:
		QList<PrimBORealTimeData>	m_listRealTimeData;
		BORealtimeDataSource*     m_dataSource;
		BORealTimePriceSettings	m_settings;
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
		QString m_BOCountdown;
		QString m_BOFloatPLInString;
		double m_BOStartTime;
		double m_BOEndTime;
		bool m_boCompleted;
		QString m_BOExecutePrice;
		bool m_isUp;

	};
}


#pragma once
#include <QString>
#include <QPointF>

#include "Indicator/IndicatorCore.h"

namespace HistoricalChartSpace
{
	class ChartPanel;
	class HistoricalChart;
	class CursorPosition
	{
	public:
		CursorPosition(HistoricalChart* pView);
		~CursorPosition();

		void OnPositionChange(QPointF& scenePoint, ChartPanel* targetPanel);
		const QString& getYValueText();
		const QString& getDateText();
		const QPointF& getScenePos();
		QString getInfoText(int& lines);

        QString getIndicatorInfoText(enIndicatorType m_type);

	private:
		QPointF m_scenePos;
		int m_dataIndex;
		double m_yValue;
		QString m_yValueText;
		QString m_date;

		ChartPanel* m_targetPanel;
		HistoricalChart* m_pView;
	};
}

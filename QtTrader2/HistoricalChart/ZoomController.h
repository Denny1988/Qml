#pragma once
#include <QList>
#include <vector>
#include "Config.h"

namespace HistoricalChartSpace
{
	class HistoricalChart;

	struct BarRange
	{
		int BeginIndex;
		int EndIndex;
		bool operator!=(BarRange& other)
		{
			return BeginIndex != other.BeginIndex || EndIndex != other.EndIndex;
		}
	};

	class ZoomController
	{
	public:
		static double getBarDistance(ZoomLevel zoomLevel);

	public:
		ZoomController(HistoricalChart& m_view, Config& m_config, QGraphicsRectItem& mainPainterPanel);
		~ZoomController();

		void zoom(bool isZoomIn);
		void setZoomLevel(ZoomLevel zoomLevel);
		void zoomAll();
		void zoomRagne(int beginIndex, int endIndex);
		void zoomRagneForUnlimited(int beginIndex, int endIndex);
		void zoomCurrentRagne();
		bool zoomPrevRagne();
		//bool isOnZoomStackHead();
		void resetZoomLevels();
		void resetZoomHistory();
		bool isSmallestLevel();
		bool isLargestLevel();

	private:
		void updateBarDistance(double barDistance);
		void setToolbarState();
		void internalZoomRagne(int beginIndex, int endIndex);

	private:
		HistoricalChart& m_view;
		QGraphicsRectItem& m_mainPainterPanel;
		Config& m_config;
		vector<BarRange> m_zoomRangeStack;
	};
}
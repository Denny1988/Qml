#pragma once
#include "qmath.h"
#include "Settings.h"
#include "BaseData.h"
#include "Indicator/Indicator.h"
#include "BaseDrawing.h"

namespace HistoricalChartSpace
{
	class Helper
	{
	public:
		static QString getChartProfilePath();
		static BaseDrawing* CreateDrawingObject(const DrawingObjectSettings& settings, ChartPanel* parent, DrawStage state);

		static double GetMarginSpaceValue(enMarginSpace enType);
		static QVariant GetDefaultConfig(enParameterConfig enType);
		static QString getShowInTimeText(showInTime type);
		static QMap<double, QString> getDefaultLevels(enDrawingObjectType type);
		static QString getDrawingObjBaseStrInfo(enDrawingObjectType type);
		static double getMinValue(const std::vector<double>* datas, int begin, int end);
		static double getMaxValue(const std::vector<double>* datas, int begin, int end);

		inline static double round(double n, unsigned d)
		{
			return floor(n * qPow(10., d) + .5) / qPow(10., d);
		}

        static void setMarginXYCoordinate(double marginXCoordinate, double marginYCoordinate);
        static double m_pixelSizeRatio;

        static QString getAppDataDirPath();
        static QString getChartResPath(QString filepathArgs);
	};
}

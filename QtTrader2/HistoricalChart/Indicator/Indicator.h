#pragma  once
#include <vector>
#include <QPainter>
#include <QHash>
#include <QPolygonF>
//#include "Settings.h"
#include "IndicatorCore.h"
#include "BaseData.h"
#include "Calculator.h"
#include "Helper.h"
#include "SC.h"

using namespace std;

class CDatabase;
class CField;

namespace HistoricalChartSpace
{
    class IndicatorSettingDialog;
	class Indicator
	{
	public:
		friend class Calculator;
		friend class ImmediateCalculator;
        friend class IndicatorSettingDialog;
		Indicator(const IndicatorSettings& settings, BaseData& baseData);
        virtual ~Indicator();

		void setCalculator(Calculator* calculator);
		Calculator* getCalculator();

		bool containsPoint(const QPointF& point);
		bool hasIndicatorData();
		double calculateMinValue();
		double calculateMaxValue();
		const QString& getTitle() const;
		bool isMa() { return m_isMa; }
        // For Indicators with only a single (OutReal) output, overload implementation is required¡£
        virtual const QString& getName();
		const IndicatorSettings& getIndicatorSettings() const;
		void setIndicatorSettings(const IndicatorSettings& settings);
		void paintIndicator(QPainter* painter, const PaintArgs& paintArgs);
		void calculateAll();
		// for test dump data
		map<QString, vector<double>*>& getData(){ return m_indicatorDatas; };

		virtual void getParameterRange(const QString& key, int& min, int& max);
		virtual void getParameterRange(const QString& key, double& min, double& max, double& decimals);

		int extendDataBuffer();

	protected:
		virtual void calculate(int beginIndex, int endIndex) = 0;

		void GetCPDA(enCPD type, const double opens[], const double closes[], const double highs[], const double lows[], double result[], int size);
		double GetCPDValue(int index);
		CDatabase* prepare(int beginIndex, int endIndex, int period, const char* inputTypes = nullptr);

	protected:
		// for indicator calculation
		int m_dataStartIndex;
		int m_inputDataBufferSize;
		CField* m_inputSeries;

	protected:
		BaseData& m_baseData;
		IndicatorSettings m_settings;
		QVector<QPolygonF> m_vecPainterPath;
	    map<QString, vector<double>*> m_indicatorDatas;
		bool m_isMa;

        QVector<Indicator*> m_MAIndicators;//for MA indicator

	private:
		void drawLineType(QPainter* painter, const PaintArgs& paintArgs, QString lineName, QPolygonF& polygon);
		void drawColumnType(QPainter* painter, const PaintArgs& paintArgs, QString lineName, QPolygonF& polygon);
		void drawDotType(QPainter* painter, const PaintArgs& paintArgs, QString lineName, QPolygonF& polygon);
		int getStartIndex(const vector<double>* datas);

		QMutex m_mutex;
		Calculator* m_calculator;
	};
}

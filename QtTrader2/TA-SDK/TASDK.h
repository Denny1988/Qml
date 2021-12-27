#pragma once
//disable warnings on STD export for VC++ only
#pragma warning (disable : 4251)

#include "math.h"
using namespace std;
#include <QtCore/qglobal.h>

# define TASDK_API

//#ifdef BUILD_STATIC
//# if defined(TASDK_EXPORTS)
//#  define TASDK_API Q_DECL_EXPORT
//# else
//#  define TASDK_API Q_DECL_IMPORT
//# endif
//#else
//# define TASDK_API
//#endif

//#define TASDK_API __declspec(dllexport)
#define EXPIMP_TEMPLATE

/*************************************************/
/*
interface modification:
1. LPCTSTR ??? const char*
2. TASDK????????? lstrcmp ??? strcmp
3. TASDK????????Character Set ??Multi-Byte???Unicode???HistoricalChart?????? Proprecessor Definitions????UNICODE??
4. CDatabase ??????setRecordset????
*/
/*************************************************/

typedef	enum eMAType{
    indSimpleMovingAverage = 1,
    indExponentialMovingAverage = 2,
    indTimeSeriesMovingAverage = 3,
    indVariableMovingAverage = 4,
    indTriangularMovingAverage = 5,
    indWeightedMovingAverage = 6,
    indVIDYA = 7
} MATYPE;


//Moving Average Types
#if !defined(SIMPLE)
#define		MA_START		0
#define		MA_SIMPLE		1
#define		MA_EXPONENTIAL	2
#define		MA_TIMESERIES	3
#define		MA_VARIABLE		4
#define		MA_TRIANGULAR	5
#define		MA_WEIGHTED		6
#define		MA_VIDYA		7
#define		MA_END			8
#endif

#define	NULL_VALUE -987654321
#define GREATER 1
#define EQUAL	0
#define LESS	-1

//#include <windows.h>
//#include <iostream>
//#include <math.h>
#include <vector>
#include <memory>
// commented by simon
//EXPIMP_TEMPLATE template class TASDK_API std::vector<double>;
//EXPIMP_TEMPLATE template class TASDK_API std::vector<int>;
//EXPIMP_TEMPLATE template class TASDK_API std::vector<char>;

class CRecordset;
//class CDatabase;
//class CField;
//class CNote;
class TASDK;
class CLinearRegression;
class CLineStudy;
class CMovingAverage;
class CNeuralNetwork;
class COscillator;

extern "C"{
    //Classes exported from TASDK.dll
    class TASDK_API CNote
    {
    public:
        CNote();
        virtual ~CNote();
        void setPeriod(int Period);
        void setValue(double Value);
        void setNote(const char* Note);
        int getPeriod();
        double getValue();
        const char* getNote();

    private:
        int m_Period;
        double m_Value;
        const char* m_Note;
    };

    class TASDK_API CField
    {
    public:
        CField(int RecordCount, const char* Name);
        virtual ~CField();
        void setNote(CNote* NewNote);
        CNote* getNote();
        int getRecordCount();
        void setName(const char* NewFieldName);
        const char* getName();
        void setValue(int RowIndex, double Value);
        double getValue(int RowIndex);
        void setStrValue(int RowIndex, const char* Value);
        const char* getStrValue(int RowIndex);
    private:
        TASDK* TASDK1;
        const char* m_Name;  // Used to be CString
        int m_RecordCount;
        CNote* m_Note;
        std::vector<const char*> strpData;
        std::vector<double> dblpData;
    };

    class TASDK_API CDatabase
    {
    public:
        CDatabase();
        virtual ~CDatabase();
        CRecordset* getRecordset();
        int getRecordCount();
        void setPosition(int Index);
        int getPosition();
        void MoveNext();
        void MovePrevious();
        void MoveFirst();
        void MoveLast();
        void setRecordset(CRecordset* recordset, int recordCount);  // add this method to replace OpenDatabase method, Simon(2016-05-06).
        void OpenDatabase(char* fileName, char* delimiter);
        void CloseDatabase();
    private:
        CField* m_Date;
        CField* m_Open;
        CField* m_High;
        CField* m_Low;
        CField* m_Close;
        CField* m_Volume;
        CRecordset* m_Recordset;
        int m_Index;
        int m_RecordCount;
    };

    class TASDK_API CRecordset
    {
    public:
        CRecordset();
        virtual ~CRecordset();
        void setDatabase(CDatabase* NewDatabase);
        CDatabase* getDatabase();
        void addField(CField* NewField);
        void renameField(const char* OldFieldName, const char* NewFieldName);
        void removeField(const char* FieldName);
        double getValue(const char* FieldName, int RowIndex);
        void setValue(const char* FieldName, int RowIndex, double Value);
        CField* getField(const char* FieldName);
        int getIndex(const char* FieldName);
        const char* getName(int FieldIndex);
        int getFieldCount();
        bool isValidField(const char* FieldName);

    private:
        std::vector<CField*> FieldData;
        int FieldCount;
        CDatabase* m_CDatabase;
    };

    class TASDK_API BarType
    {
    public:
        BarType();
        virtual ~BarType();
        double Volume;
        double ClosePrice;
        double LowPrice;
        double HighPrice;
        double OpenPrice;
    };

    class TASDK_API CandleType
    {
    public:
        CandleType(char* Name, double Range, int BullBear);
        CandleType();
        virtual ~CandleType();
        char* getName();
        double getRange();
        double getBullBear();
        void setName(char* Name);
        void setRange(double Range);
        void setBullBear(int BullBear);
    private:
        char* m_Name;
        double m_Range;
        int m_BullBear;
    };

    class TASDK_API CBands
    {
    public:
        CBands();
        virtual ~CBands();
        CRecordset* HighLowBands(CDatabase* pData, CField* HighPrice,
            CField* LowPrice, CField* ClosePrice, int Periods);
        CRecordset* MovingAverageEnvelope(CDatabase* pData, CField* pSource,
            int Periods, int MAType, double Shift);
        CRecordset* BollingerBands(CDatabase* pData, CField* pSource,
            int Periods, int StandardDeviations, int MAType);
        CRecordset* FractalChaosBands(CDatabase* pData, CRecordset* pOHLCV,
            int Periods);
    };

    class TASDK_API CCandleStick
    {
    public:
        CCandleStick();
        virtual ~CCandleStick();
        CNote* IdentifyPattern(CRecordset* OHLCV, int Period);
    };

    class TASDK_API CChartPattern
    {
    public:
        CChartPattern();
        virtual ~CChartPattern();
        CNote* PercentRetracement(CDatabase* Data,
            CField* Source, int StartPeriod, int EndPeriod);
    };

    class TASDK_API CCycle
    {
    public:
        CCycle();
        virtual ~CCycle();
        int Analyze(CField* Source);
    };


    class TASDK_API CGeneral
    {
    public:
        CGeneral();
        virtual ~CGeneral();
        CRecordset* HighMinusLow(CDatabase* pData, CRecordset* pOHLCV, const char* Alias = "High Minus Low");
        CRecordset* MedianPrice(CDatabase* pData, CRecordset* pOHLCV, const char* Alias = "Median Price");
        CRecordset* TypicalPrice(CDatabase* pData, CRecordset* pOHLCV, const char* Alias = "Typical Price");
        CRecordset* WeightedClose(CDatabase* pData, CRecordset* pOHLCV, const char* Alias = "Weighted Close");
        CRecordset* VolumeROC(CDatabase* pData, CField* Volume, int Periods, const char* Alias = "Volume ROC");
        CRecordset* PriceROC(CDatabase* pData, CField* pSource, int Periods, const char* Alias = "Price ROC");
        double CorrelationAnalysis(CField* pSource1, CField* pSource2);
        CRecordset* StandardDeviation(CDatabase* pData, CField* pSource, int Periods,
            int StandardDeviations, int MAType, const char* Alias = "Standard Deviation");
        CNote* MaxValue(CField* pSource, int StartPeriod, int EndPeriod);
        CNote* MinValue(CField* pSource, int StartPeriod, int EndPeriod);

    };

    class TASDK_API CIndex
    {
    public:
        CIndex();
        virtual ~CIndex();
        CRecordset* MoneyFlowIndex(CDatabase* pData, CRecordset* pOHLCV,
            int Periods, const char* Alias = "Money Flow Index");
        CRecordset* TradeVolumeIndex(CDatabase* pData, CField* pSource,
            CField* Volume, double MinTickValue, const char* Alias = "Trade Volume Index");
        CRecordset* SwingIndex(CDatabase* pData, CRecordset* pOHLCV,
            double LimitMoveValue, const char* Alias = "Swing Index");
        CRecordset* AccumulativeSwingIndex(CDatabase* pData, CRecordset* pOHLCV,
            double LimitMoveValue, const char* Alias = "Accumulative Swing Index");
        CRecordset* RelativeStrengthIndex(CDatabase* pData, CField* pSource,
            int Periods, const char* Alias = "Relative Strength Index");
        CRecordset* ComparativeRelativeStrength(CDatabase* pData, CField* pSource1,
            CField* pSource2, const char* Alias = "Comparative Relative Strength");
        CRecordset* PriceVolumeTrend(CDatabase* pData, CField* pSource,
            CField* Volume, const char* Alias = "Price Volume Trend");
        CRecordset* PositiveVolumeIndex(CDatabase* pData,
            CField* pSource, CField* Volume, const char* Alias = "Positive Volume Index");
        CRecordset* NegativeVolumeIndex(CDatabase* pData,
            CField* pSource, CField* Volume, const char* Alias = "Negative Volume Index");
        CRecordset* Performance(CDatabase* pData, CField* pSource, const char* Alias = "Performance");
        CRecordset* MassIndex(CDatabase* pData, CRecordset* pOHLCV,
            int Periods, const char* Alias = "Mass Index");
        CRecordset* OnBalanceVolume(CDatabase* pData, CField* pSource,
            CField* Volume, const char* Alias = "On Balance Volume");
        CRecordset* ChaikinMoneyFlow(CDatabase* pData, CRecordset* pOHLCV,
            int Periods, const char* Alias = "Chaikin Money Flow");
        CRecordset* CommodityChannelIndex(CDatabase* pData, CRecordset* pOHLCV,
            int Periods, const char* Alias = "CCI");

    };

    class TASDK_API CLinearRegression
    {
    public:
        CLinearRegression();
        virtual ~CLinearRegression();
        CRecordset* Regression(CDatabase* pData, CField* pSource, int Periods);
        CRecordset* TimeSeriesForecast(CDatabase* pData, CField* pSource, int Periods, const char* Alias = "Time Series Forecast");
    };

    class TASDK_API CLineStudy
    {
    public:
        CLineStudy();
        virtual ~CLineStudy();
        CRecordset* QuadrantLines(CDatabase* Data, CRecordset* OHLCV,
            int StartPeriod, int EndPeriod);
        CRecordset* TironeLevel(CDatabase* Data, CRecordset* OHLCV,
            int StartPeriod, int EndPeriod);
        CRecordset* SpeedResistanceLines(CDatabase* Data,
            CRecordset* OHLCV, int StartPeriod, int EndPeriod);
        CRecordset* Fibonacci(CField* Source, int StartPeriod,
            int EndPeriod, char* Alias);
    };

    class TASDK_API CMovingAverage
    {
    public:
        CMovingAverage();
        virtual ~CMovingAverage();
        CRecordset* SimpleMovingAverage(CDatabase* pData, CField* pSource, int Periods, const char* Alias = "Simple Moving Average");
        CRecordset* ExponentialMovingAverage(CDatabase* pData, CField* pSource, int Periods, const char* Alias = "Exponential Moving Average");
        CRecordset* TimeSeriesMovingAverage(CDatabase* pData, CField* pSource, int Periods, const char* Alias = "Time Series Moving Average");
        CRecordset* VariableMovingAverage(CDatabase* pData, CField* pSource, int Periods, const char* Alias = "Variable Moving Average");
        CRecordset* TriangularMovingAverage(CDatabase* pData, CField* pSource, int Periods, const char* Alias = "Triangular Moving Average");
        CRecordset* WeightedMovingAverage(CDatabase* pData, CField* pSource, int Periods, const char* Alias = "Weighted Moving Average");
        CRecordset* VIDYA(CDatabase* pData, CField* pSource, int Periods, double R2Scale, const char* Alias = "VIDYA");
        CRecordset* WellesWilderSmoothing(CField* pSource, int Periods, const char* Alias = "Welles Wilder Smoothing");

    };

    class TASDK_API CNeuralNetwork
    {
    public:
        CNeuralNetwork();
        virtual ~CNeuralNetwork();
        CRecordset* NeuralIndicator(CDatabase* Data, CField* Source,
            int Periods, double LearningRate, long Epochs,
            double PercentTrain, char* Alias);
    private:
        double m_LearningRate;
        //Neurons
        double Neuron1;
        double Neuron2;
        double Neuron3;
        double Neuron4;
        double Neuron5;
        double Neuron6;
        //Weights
        double W13;
        double W14;
        double W15;
        double W23;
        double W24;
        double W25;
        double W36;
        double W46;
        double W56;
        //Biases
        double B3;
        double B4;
        double B5;
        double B6;
        double activation(double Value);
        double feedForward(double Input1, double Input2);
        void train(double Input1, double Input2, double Target);
        void initialize();
    };

    class TASDK_API COscillator
    {
    public:
        COscillator();
        virtual ~COscillator();
        CRecordset* ChandeMomentumOscillator(CDatabase* pData, CField* pSource, int Periods, const char* Alias = "CMO");
        CRecordset* Momentum(CDatabase* pData, CField* pSource, int Periods, const char* Alias = "Momentum");
        CRecordset* TRIX(CDatabase* pData, CField* pSource, int Periods, const char* Alias = "TRIX");
        CRecordset* UltimateOscillator(CDatabase* pData, CRecordset* pOHLCV,
            int Cycle1, int Cycle2, int Cycle3, const char* Alias = "Ultimate Oscillator");
        CRecordset* VerticalHorizontalFilter(CDatabase* pData, CField* pSource, int Periods, const char* Alias = "VHF");
        CRecordset* WilliamsPctR(CDatabase* pData, CRecordset* pOHLCV, int Periods, const char* Alias = "Williams' %R");
        CRecordset* WilliamsAccumulationDistribution(CDatabase* pData, CRecordset* pOHLCV, const char* Alias = "Williams' Accumulation Distribution");
        CRecordset* VolumeOscillator(CDatabase* pData, CField* Volume, int ShortTerm,
            int LongTerm, int PointsOrPercent, const char* Alias = "Volume Oscillator");
        CRecordset* ChaikinVolatility(CDatabase* pData, CRecordset* pOHLCV,
            int Periods, int ROC, int MAType, const char* Alias = "Chaikin Volatility");
        CRecordset* StochasticOscillator(CDatabase* pData, CRecordset* pOHLCV,
            int KPeriods, int KSlowingPeriods, int DPeriods, int MAType);
        CRecordset* PriceOscillator(CDatabase* pData, CField* pSource,
            int LongCycle, int ShortCycle, int MAType, const char* Alias = "Price Oscillator");
        CRecordset* MACD(CDatabase* pData, CField* pSource, int SignalPeriods, int LongCycle, int ShortCycle, const char* Alias = "MACD");
        CRecordset* EaseOfMovement(CDatabase* pData, CRecordset* pOHLCV,
            int Periods, int MAType, const char* Alias = "Ease of Movement");
        CRecordset* DetrendedPriceOscillator(CDatabase* pData, CField* pSource,
            int Periods, int MAType, const char* Alias = "DPO");
        CRecordset* ParabolicSAR(CDatabase* pData, CField* HighPrice,
            CField* LowPrice, double MinAF = 0.02, double MaxAF = 0.2,
            const char* Alias = "Parabolic SAR");
        CRecordset* DirectionalMovementSystem(CDatabase* pData, CRecordset* pOHLCV, int Periods);
        CRecordset* TrueRange(CDatabase* pData, CRecordset* pOHLCV, const char* Alias = "TR");
        CRecordset* Aroon(CDatabase* pData, CRecordset* pOHLCV, int Periods);
        CRecordset* RainbowOscillator(CDatabase* pData, CField* pSource, int Levels,
            int MAType, const char* Alias = "Rainbow Oscillator");
        CRecordset* FractalChaosOscillator(CDatabase* pData, CRecordset* pOHLCV,
            int Periods, const char* Alias = "Fractal Chaos Oscillator");
    };

    class TASDK_API TASDK
    {
    public:
        TASDK();
        virtual ~TASDK();
        double maxVal(double Value1, double Value2);
        double minVal(double Value1, double Value2);
        int maxVal(int Value1, int Value2);
        int minVal(int Value1, int Value2);
        double normalize(double Max, double Min, double Value);
        char* concatCharPtr(char* String1, char* String2);
        int compareChar(register char *p, register char *q);
        double Abs(double Value);
    };

}

#pragma  once
#include <QGraphicsRectItem>
#include <QObject>
#include <QPixmap>
#include <QPushButton>
#include <QPointF>
#include <QMap>
#include <QLabel>

#include "Indicator/IndicatorCore.h"
#include "Common.h"

namespace HistoricalChartSpace
{
    class IndicatorSettingPanel : public QObject, public QGraphicsRectItem
	{
        Q_OBJECT
	public:
        IndicatorSettingPanel(double pixelSizeRatio, QString merchantPath, QMap<QString, enIndicatorType> mainIndicators, QList<Frequency> frequencyList, bool horizontalScreen = false);
        IndicatorSettingPanel(double pixelSizeRatio, QString merchantPath, enIndicatorType indicator, bool horizontalScreen = false);
        ~IndicatorSettingPanel();
        void addMainIndicator(enIndicatorType indicatorType);
        void removeMainIndicator(enIndicatorType indicatorType);
        void changeMainIndicator(enIndicatorType indicatorType);
        void setIndicatorInfoValue(QString info);
        QString getCurrentIndicatorName();
        const Frequency& getDefaultFrequency(){ return _defaultFrequency; }
        void setFrequency(Frequency value);
        bool isNullIndicatorInfoLabel();
        void changeStyle(QColor bgColor, QColor fontColor);

	private:
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        Frequency getNextFrequency();

    signals:
        void ShowIndicatorSettingDialogSignal();
        void ChangeIndicatorSignal();
        void SetFrequencySignal(int index);


    private:
        QString _imagePath;
        double _pixelSizeRatio;
        bool _isMainIndicator;
        int _oneAliquotWidth;

        QPushButton* _indicatorButton;
        QPushButton* _changeFrequencyButton;
        QPixmap* _settingPixmap;
        QLabel* _indicatorInfoLabel;

        QPointF _indicatorPos;
        QPointF _settingPos;

        QMap<QString, enIndicatorType> _mainIndicators;
        QList<Frequency> _frequencyList;
        Frequency _defaultFrequency;
        //enIndicatorType _secondaryIndicator;
        bool _horizontalScreen;
        QString _bgColor;
        QString _fontColor;
	};
}

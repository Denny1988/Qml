#include "IndicatorSettingPanel.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QScreen>
#include <QApplication>
#include <QDebug>
#include <QFont>
#include "SR.h"

namespace HistoricalChartSpace
{
    IndicatorSettingPanel::IndicatorSettingPanel(double pixelSizeRatio, QString merchantPath,
                                                 QMap<QString, enIndicatorType> mainIndicators,
                                                 QList<Frequency> frequencyList,
                                                 bool horizontalScreen)
    {        
        this->_imagePath = QString("%1/Images/settings.png").arg(merchantPath);
        this->_pixelSizeRatio = pixelSizeRatio;
        this->_isMainIndicator = true;
        this->_mainIndicators = mainIndicators;
        this->_frequencyList = frequencyList;
        this->_horizontalScreen = horizontalScreen;

        QScreen *screen = QApplication::primaryScreen();
        this->_oneAliquotWidth = screen->size().width()/7;
#if defined (RUNTIME_OS_IPHONE)
        if(this->_horizontalScreen)
            this->_oneAliquotWidth = screen->size().width()/10;
#endif

        int margin = 4 * pixelSizeRatio;
        int height =  30 * this->_pixelSizeRatio;

        int settingSize = 20*this->_pixelSizeRatio;
        this->_settingPixmap = new QPixmap(settingSize, settingSize);
        this->_settingPixmap->load(this->_imagePath);

        this->_indicatorButton = new QPushButton;
        if(this->_mainIndicators.count() == 0)
            this->_indicatorButton->setText("Null");
        else
            this->_indicatorButton->setText(this->_mainIndicators.firstKey());
        this->_indicatorButton->setFixedSize(this->_oneAliquotWidth- margin/2, 24*this->_pixelSizeRatio);

        this->_indicatorInfoLabel = new QLabel();
        this->_indicatorInfoLabel->setWordWrap(true);
        this->_indicatorInfoLabel->setStyleSheet("background-color: transparent;");
        this->_indicatorInfoLabel->setFixedSize(this->_oneAliquotWidth*4, height);
#if defined (RUNTIME_OS_IPHONE)
        if(this->_horizontalScreen)
            this->_indicatorInfoLabel->setFixedSize(this->_oneAliquotWidth*7, height);
#endif
        QFont font = this->_indicatorInfoLabel->font();
        font.setPointSize(12);
        this->_indicatorInfoLabel->setFont(font);

        this->_indicatorPos = QPointF(this->_oneAliquotWidth, height);
        this->_settingPos = QPointF(this->_oneAliquotWidth*2, height);

        if(this->_horizontalScreen)
        {
            this->_changeFrequencyButton = new QPushButton;
            this->_changeFrequencyButton->setText("1 M");
            QFont frequencyfont = this->_changeFrequencyButton->font();
            frequencyfont.setPointSize(14);
            this->_changeFrequencyButton->setFont(frequencyfont);
            this->_changeFrequencyButton->setFixedSize(this->_oneAliquotWidth, height);
            this->_changeFrequencyButton->setMinimumSize(this->_oneAliquotWidth, height);
            this->_changeFrequencyButton->setStyleSheet("background-color: #ffffff; color: red; border:0px;");
        }
    }

    IndicatorSettingPanel::IndicatorSettingPanel(double pixelSizeRatio, QString merchantPath, enIndicatorType indicator, bool horizontalScreen)
    {
        this->_imagePath = QString("%1/Images/settings.png").arg(merchantPath);
        this->_pixelSizeRatio = pixelSizeRatio;
        this->_isMainIndicator = false;
        this->_horizontalScreen = horizontalScreen;

        QScreen *screen = QApplication::primaryScreen();
        this->_oneAliquotWidth = screen->size().width()/7;
#if defined (RUNTIME_OS_IPHONE)
        if(this->_horizontalScreen)
            this->_oneAliquotWidth = screen->size().width()/10;
#endif

        int margin = 4 * pixelSizeRatio;
        int height =  30 * this->_pixelSizeRatio;

        int settingSize = 20*this->_pixelSizeRatio;
        this->_settingPixmap = new QPixmap(settingSize, settingSize);
        this->_settingPixmap->load(this->_imagePath);

        this->_indicatorButton = new QPushButton;
        this->_indicatorButton->setText(IndicatorCore::getIndicatorShorthandName(indicator));
        this->_indicatorButton->setFixedSize(this->_oneAliquotWidth- margin/2, 24*this->_pixelSizeRatio);

        this->_indicatorInfoLabel = new QLabel();
        this->_indicatorInfoLabel->setWordWrap(true);
        this->_indicatorInfoLabel->setStyleSheet("background-color: transparent;");
        this->_indicatorInfoLabel->setFixedSize(this->_oneAliquotWidth*4, height);
#if defined (RUNTIME_OS_IPHONE)
        if(this->_horizontalScreen)
            this->_indicatorInfoLabel->setFixedSize(this->_oneAliquotWidth*7, height);
#endif
        QFont font = this->_indicatorInfoLabel->font();
        font.setPointSize(12);
        this->_indicatorInfoLabel->setFont(font);

        this->_indicatorPos = QPointF(this->_oneAliquotWidth, height);
        this->_settingPos = QPointF(this->_oneAliquotWidth*2, height);
    }

    IndicatorSettingPanel::~IndicatorSettingPanel()
    {
    }

    void IndicatorSettingPanel::addMainIndicator(enIndicatorType indicatorType)
    {
        QString shorthandName = IndicatorCore::getIndicatorShorthandName(indicatorType);
        if(this->_mainIndicators.count() == 0)
        {
            this->_indicatorButton->setText(shorthandName);
            emit ChangeIndicatorSignal();
        }
        this->_mainIndicators.insert(shorthandName, indicatorType);
        this->update();
    }

    void IndicatorSettingPanel::removeMainIndicator(enIndicatorType indicatorType)
    {
        if(_isMainIndicator)
        {
            QString shorthandName = IndicatorCore::getIndicatorShorthandName(indicatorType);
            this->_mainIndicators.remove(shorthandName);
            if(this->_mainIndicators.count() == 0)
            {
                this->_indicatorButton->setText("Null");
                this->_indicatorInfoLabel->setText("");
            }
            else
            {
                this->_indicatorButton->setText(this->_mainIndicators.firstKey());
                emit ChangeIndicatorSignal();
            }
        }
        else
        {
            this->_indicatorButton->setText("Null");
            this->_indicatorInfoLabel->setText("");
        }
        this->update();
    }

    void IndicatorSettingPanel::changeMainIndicator(enIndicatorType indicatorType)
    {
        QString shorthandName = IndicatorCore::getIndicatorShorthandName(indicatorType);
        this->_indicatorButton->setText(shorthandName);
        emit ChangeIndicatorSignal();
        this->update();
    }

    void IndicatorSettingPanel::setIndicatorInfoValue(QString info)
    {
        this->_indicatorInfoLabel->setText(info);
        this->update();
    }

    QString IndicatorSettingPanel::getCurrentIndicatorName()
    {
        return this->_indicatorButton->text();
    }

    void IndicatorSettingPanel::setFrequency(Frequency value)
    {
        if(this->_horizontalScreen)
        {
            this->_defaultFrequency = value;
            this->_changeFrequencyButton->setText(SR::get(this->_defaultFrequency.getName()));
            this->update();
        }
    }

    bool IndicatorSettingPanel::isNullIndicatorInfoLabel()
    {
        return this->_indicatorInfoLabel->text().length() == 0;
    }

    void IndicatorSettingPanel::changeStyle(QColor bgColor, QColor fontColor)
    {
        this->_bgColor = bgColor.name();
        this->_fontColor = fontColor.name();
        this->_indicatorInfoLabel->setStyleSheet(QString("background-color: %1; color:%2").arg(this->_bgColor, this->_fontColor));
#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
        this->_indicatorButton->setStyleSheet(QString("background-color: %1; color: #d6bf95; border:1px solid #d6bf95; border-radius:2px;").arg(this->_bgColor));
#else
        this->_indicatorButton->setStyleSheet(QString("background-color: %1; color: #d6bf95; border:1px solid #d6bf95; border-radius:5px;").arg(this->_bgColor));
#endif

    }

    void IndicatorSettingPanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
    {
        int height =  30 * this->_pixelSizeRatio;
        int margin = 4 * this->_pixelSizeRatio;
        int topMargin = 3 * this->_pixelSizeRatio;
        int settingTopMargin = 5 * this->_pixelSizeRatio;
        if(this->_isMainIndicator)
        {
            if(this->_mainIndicators.count() == 0)
                this->_indicatorButton->setText("Null");

            QPixmap map = this->_indicatorButton->grab();            
            painter->drawPixmap(margin/2, topMargin, map);

            if(this->_mainIndicators.count() > 0)
            {                
                int y = height - margin;
                int x = this->_oneAliquotWidth - margin/4;
                int signHeight =  7 * this->_pixelSizeRatio;

                painter->setRenderHint(QPainter::Antialiasing,true);
                QPointF signPoints[3] = {
                    QPointF(x, y),
                    QPointF(x, y-signHeight),
                    QPointF(x- signHeight, y)
                };
                painter->setBrush(QBrush(QColor("#d6bf95")));
                painter->setPen(Qt::NoPen);
                painter->drawPolygon(signPoints, 3);
            }

            painter->resetTransform();
            int settingSize = 20*this->_pixelSizeRatio;
            QPixmap settingPixmap = this->_settingPixmap->scaled(settingSize, settingSize);
            painter->drawPixmap(this->_oneAliquotWidth + ((this->_oneAliquotWidth-settingSize)/2), settingTopMargin,  settingPixmap);

            if(this->_horizontalScreen)
            {
                QPixmap fmap = this->_changeFrequencyButton->grab();
                painter->drawPixmap(this->_oneAliquotWidth*2, 0, this->_oneAliquotWidth, 30*this->_pixelSizeRatio, fmap);

                int fy = height - margin;
                int fx = this->_oneAliquotWidth*3 - margin/4;
#if defined (RUNTIME_OS_IPHONE)
                if(this->_horizontalScreen)
                    fx = this->_oneAliquotWidth*3 - margin;
#endif
                int fSignHeight =  7 * this->_pixelSizeRatio;

                painter->setRenderHint(QPainter::Antialiasing,true);
                QPointF signPoints[3] = {
                    QPointF(fx, fy),
                    QPointF(fx- fSignHeight, fy-fSignHeight),
                    QPointF(fx- fSignHeight, fy)
                };
                painter->setBrush(QBrush(Qt::black));
                painter->setPen(Qt::NoPen);
                painter->drawPolygon(signPoints, 3);

                QPixmap lmap = this->_indicatorInfoLabel->grab();
                painter->drawPixmap(this->_oneAliquotWidth*3, 0, lmap);
            }
            else
            {
                QPixmap lmap = this->_indicatorInfoLabel->grab();
                painter->drawPixmap(this->_oneAliquotWidth*2, 0, lmap);
            }
        }
        else
        {
            QPixmap map = this->_indicatorButton->grab();
            painter->drawPixmap(margin/2, topMargin, map);

            int settingSize = 20*this->_pixelSizeRatio;
            QPixmap settingPixmap = this->_settingPixmap->scaled(settingSize, settingSize);
            painter->drawPixmap(this->_oneAliquotWidth + ((this->_oneAliquotWidth-settingSize)/2), settingTopMargin,  settingPixmap);

            QPixmap lmap = this->_indicatorInfoLabel->grab();
            painter->drawPixmap(this->_oneAliquotWidth*2, 0, lmap);

            painter->setPen(Qt::NoPen);
            painter->setBrush(QBrush(QColor(this->_bgColor.length() > 0 ? this->_bgColor : "#ffffff")));
            painter->drawRect(this->_oneAliquotWidth*6, 1, this->_oneAliquotWidth, height-1);
        }
        painter->resetTransform();
    }

    void IndicatorSettingPanel::mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        qDebug() << " IndicatorSettingPanel::mousePressEvent " << event->type();
        if (event->buttons() == Qt::LeftButton)
        {
            QPointF pos = event->pos();
            qDebug() << " IndicatorSettingPanel::mousePressEvent pos:" << pos;
            if(pos.x() >= 0 && pos.x() < this->_indicatorPos.x() && pos.y() >=0 &&  pos.y() <= this->_indicatorPos.y() && this->_isMainIndicator)
            {
                qDebug() << " IndicatorSettingPanel::mousePressEvent ChangeIndicatorSignal:" ;
                if(this->_mainIndicators.count() > 1)
                {
                    auto shorthandNames = this->_mainIndicators.keys();
                    int index = shorthandNames.indexOf(this->_indicatorButton->text());
                    index++;
                    if(index > (this->_mainIndicators.count()-1))
                        index = 0;
                    this->_indicatorButton->setText(shorthandNames[index]);
                    emit ChangeIndicatorSignal();
                }
            }
            else if(pos.x() >= this->_indicatorPos.x() && pos.x() < this->_settingPos.x() && pos.y() >=0 &&  pos.y() <= this->_settingPos.y())
            {
                qDebug() << " IndicatorSettingPanel::mousePressEvent ShowIndicatorSettingDialogSignal:" ;
                emit ShowIndicatorSettingDialogSignal();
            }
//            else if(this->_isMainIndicator && pos.x() >= this->_oneAliquotWidth*6
//                    && pos.y() >=0 &&  pos.y() <= this->_settingPos.y() && !this->_horizontalScreen)
//            {
//                 qDebug() << " IndicatorSettingPanel::mousePressEvent old _defaultFrequency:" << _defaultFrequency.getName();
//                this->_defaultFrequency = this->getNextFrequency();
//                 qDebug() << " IndicatorSettingPanel::mousePressEvent new _defaultFrequency:" << _defaultFrequency.getName();
//                this->_changeFrequencyButton->setText(SR::get(this->_defaultFrequency.getName()));
//                int index = this->_frequencyList.indexOf(this->_defaultFrequency);
//                emit SetFrequencySignal(index);
//                this->update();
//            }
            else if(this->_isMainIndicator && pos.x() >= this->_oneAliquotWidth*2 && pos.x() <= this->_oneAliquotWidth*3
                    && pos.y() >=0 &&  pos.y() <= this->_settingPos.y() && this->_horizontalScreen)
            {
                 qDebug() << " IndicatorSettingPanel::mousePressEvent old _defaultFrequency:" << _defaultFrequency.getName();
                this->_defaultFrequency = this->getNextFrequency();
                 qDebug() << " IndicatorSettingPanel::mousePressEvent new _defaultFrequency:" << _defaultFrequency.getName();
                this->_changeFrequencyButton->setText(SR::get(this->_defaultFrequency.getName()));
                int index = this->_frequencyList.indexOf(this->_defaultFrequency);
                emit SetFrequencySignal(index);
                this->update();
            }
        }
    }



    Frequency IndicatorSettingPanel::getNextFrequency()
    {
        FrequencyType nextType;
        int nextUnit;
        int currentUnit = this->_defaultFrequency.getUint();
        switch (this->_defaultFrequency.getType())
        {
        case HistoricalChartSpace::Minute:
            if (currentUnit == 1)
            {
                nextType = HistoricalChartSpace::Minute;
                nextUnit = 5;
            }
            else if (currentUnit == 5)
            {
                nextType = HistoricalChartSpace::Minute;
                nextUnit = 15;
            }
            else if (currentUnit == 15)
            {
                nextType = HistoricalChartSpace::Minute;
                nextUnit = 30;
            }
            else if (currentUnit == 30)
            {
                nextType = HistoricalChartSpace::Hour;
                nextUnit = 1;
            }
            break;
        case HistoricalChartSpace::Hour:
            if (currentUnit == 1)
            {
                nextType = HistoricalChartSpace::Hour;
                nextUnit = 2;
            }
            else if (currentUnit == 2)
            {
                nextType = HistoricalChartSpace::Hour;
                nextUnit = 3;
            }
            else if (currentUnit == 3)
            {
                nextType = HistoricalChartSpace::Hour;
                nextUnit = 4;
            }
            else if (currentUnit == 4)
            {
                nextType = HistoricalChartSpace::Day;
                nextUnit = 1;
            }
            break;
        case HistoricalChartSpace::Day:
            nextType = HistoricalChartSpace::Week;
            nextUnit = 1;
            break;
        case HistoricalChartSpace::Week:
            nextType = HistoricalChartSpace::Month;
            nextUnit = 1;
            break;
        case HistoricalChartSpace::Month:
            nextType = HistoricalChartSpace::Minute;
            nextUnit = 1;
            break;
        default:
            break;
        }
        return Frequency(nextType, nextUnit);
    }

}


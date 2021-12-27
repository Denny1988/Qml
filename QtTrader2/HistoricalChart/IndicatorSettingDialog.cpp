#include "IndicatorSettingDialog.h"
#include "ui_indicatorsettingdialog.h"
#include "Indicator/Indicator.h"
#include "historicalchart.h"
#include "IndicatorPanel.h"
#include "PricePanel.h"
#include "SR.h"
#include "colorComboxwidget.h"

#include <QColorDialog>
#include <QScroller>
#include <QScreen>
#include <QDebug>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QHeaderView>

namespace HistoricalChartSpace
{
    //-----------------------ItemWidget------------------------//
    ItemWidget::ItemWidget(int row, int column, int indicatorType, QString name, bool isChecked, QWidget *parent) :
        QLabel(parent), _isStartMouseMoveing(false), _row(row), _column(column),
        _indicatorType(indicatorType), _isChecked(isChecked)
    {
        this->setCheckStyle();
        this->setAlignment(Qt::AlignCenter);
        this->setText(name);
    }

    void ItemWidget::setChecked(bool isCheck, bool isSubmitSignal)
    {
        if(isSubmitSignal && this->_isChecked != isCheck)
        {
            emit checkItemSignal(_row, _column, _indicatorType, isCheck);
        }
        this->_isChecked = isCheck;
        this->setCheckStyle();
    }

    void ItemWidget::setCheckStyle()
    {
        if(_isChecked)
            this->setStyleSheet("background-color:#e6e6e6; color: #d6bf95;");
        else
            this->setStyleSheet("background-color: #ffffff; color: black;");
    }

    bool ItemWidget::event(QEvent *event)
    {
        if(event->type() == QEvent::MouseMove)
        {
            if(!this->_isStartMouseMoveing)
            {
                this->_isStartMouseMoveing = true;
                QMouseEvent * mEvent = (QMouseEvent *)event;
                this->_mouseMoveStartPoint = mEvent->globalPos();
                auto mPoint = mEvent->pos();
                qDebug() << "ClickItemWidget MouseMove mGPoint:" << this->_mouseMoveStartPoint << " mPoint:" << mPoint;
                return false;
            }
        }
        else if(event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent *mEvent = (QMouseEvent *)event;
            auto mGPoint = mEvent->globalPos();
            auto mPoint = mEvent->pos();
            qDebug() << "ClickItemWidget MouseButtonRelease mGPoint:" << mGPoint << " mPoint:" << mPoint << " mouseMoveStartPoint:"<< this->_mouseMoveStartPoint;
            if(qAbs(this->_mouseMoveStartPoint.x()-mGPoint.x()) < 5
            || qAbs(this->_mouseMoveStartPoint.y()-mGPoint.y()) < 5
            || (this->_mouseMoveStartPoint.x() == 0 && this->_mouseMoveStartPoint.y() == 0)
            || (this->_mouseMoveStartPoint.x() == -1 && this->_mouseMoveStartPoint.y() == -1)
            )
            {
                qDebug() << "ClickItemWidget MouseButtonRelease click event";
               if(_isChecked)
                   _isChecked = false;
               else
                   _isChecked = true;
               this->setCheckStyle();
               emit checkItemSignal(_row, _column, _indicatorType, _isChecked);
               this->_mouseMoveStartPoint = QPoint(-1, -1);
               this->_isStartMouseMoveing = false;
               return true;
            }
            else
            {
                this->_mouseMoveStartPoint = QPoint(-1, -1);
                this->_isStartMouseMoveing = false;
            }
        }
        return QWidget::event(event);
    }



    //-----------------------MainIndicatorListWidget------------------------//
    MainIndicatorListWidget::MainIndicatorListWidget(int height, int width, QList<enIndicatorType> showIndicators, bool horizontalScreen, QWidget *parent) : QTableWidget(parent)
    {
        this->setFixedSize(width, height);
        this->_horizontalScreen = horizontalScreen;

        this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        this->setContentsMargins(0, 0, 0, 0);
        this->setFrameShape(QFrame::NoFrame);
        this->setShowGrid(false);
        this->setSelectionMode(QAbstractItemView::MultiSelection);
        this->setDragEnabled(false);
        this->setDragDropMode(QAbstractItemView::NoDragDrop);
        this->setEditTriggers(QAbstractItemView::NoEditTriggers);
        this->setFocusPolicy(Qt::NoFocus);
#ifndef RUNTIME_OS_MACX
        this->verticalHeader()->hide();
        this->horizontalHeader()->hide();
#endif
        QScroller::grabGesture(this, QScroller::TouchGesture);
        int itemHeight;
        if(horizontalScreen)
        {
            this->setRowCount(3);
            this->setColumnCount(10);
            itemHeight = height/3;
        }
        else
        {
            this->setRowCount(5);
            this->setColumnCount(5);
            itemHeight = height/5;
        }

        int column = 0;
        int row = 0;
        this->setRowHeight(row, itemHeight);
        this->addListWidgetItem(row, column++, "BB", enIndicatorBollingerBands, showIndicators.contains(enIndicatorBollingerBands));
        this->addListWidgetItem(row, column++, "MAE", enIndicatorMovingAverageEnvelope, showIndicators.contains(enIndicatorMovingAverageEnvelope));
        this->addListWidgetItem(row, column++, "MA", enIndicatorMovingAverages, showIndicators.contains(enIndicatorMovingAverages));
        this->addListWidgetItem(row, column++, "PSAR", enIndicatorParabolicSAR, showIndicators.contains(enIndicatorParabolicSAR));
        this->addListWidgetItem(row, column++, "WWS", enIndicatorWellesWilderSmoothing, showIndicators.contains(enIndicatorWellesWilderSmoothing));
    }

    void MainIndicatorListWidget::addListWidgetItem(int row, int column, QString indicatorName, int indicatorType, bool isChecked)
    {
        this->setColumnWidth(column, this->width()/(this->_horizontalScreen ? 10 : 5));
        ItemWidget* itemWidget = new ItemWidget(row, column, indicatorType, indicatorName, isChecked, this);
        connect(itemWidget, SIGNAL(checkItemSignal(int , int , int , bool)), this, SLOT(checkItemSlot(int , int , int , bool)));
        this->setCellWidget(row, column, itemWidget);
    }

    void MainIndicatorListWidget::checkItemSlot(int row, int column, int indicatorType, bool isChecked)
    {
        emit checkIndicatorSignal(indicatorType, isChecked, true);
    }


    //-----------------------SecondaryIndicatorTableWidget------------------------//
    SecondaryIndicatorTableWidget::SecondaryIndicatorTableWidget(int height, int width, QList<enIndicatorType> showIndicators, bool horizontalScreen, QWidget *parent) : QTableWidget(parent)
    {
        this->setFixedSize(width, height);
        this->_horizontalScreen = horizontalScreen;
        this->_checkIndicatorType = -1;

        this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        this->setContentsMargins(0, 0, 0, 0);
        this->setFrameShape(QFrame::NoFrame);
        this->setShowGrid(false);
        this->setSelectionMode(QAbstractItemView::SingleSelection);
        this->setDragEnabled(false);
        this->setDragDropMode(QAbstractItemView::NoDragDrop);
        this->setEditTriggers(QAbstractItemView::NoEditTriggers);
        this->setFocusPolicy(Qt::NoFocus);
#ifndef RUNTIME_OS_MACX
        this->verticalHeader()->hide();
        this->horizontalHeader()->hide();
#endif
        QScroller::grabGesture(this, QScroller::TouchGesture);
        int itemHeight;
        if(horizontalScreen)
        {
            this->setRowCount(3);
            this->setColumnCount(10);
            itemHeight = height/3;
        }
        else
        {
            this->setRowCount(6);
            this->setColumnCount(5);
            itemHeight = height/5;
        }
        int column = 0;
        int row = 0;
        this->setRowHeight(row, itemHeight);
        this->addTableWidgetItem(row, column++, "MACD", enIndicatorMACD, showIndicators.contains(enIndicatorMACD));
        this->addTableWidgetItem(row, column++, "SD", enIndicatorStandardDeviation, showIndicators.contains(enIndicatorStandardDeviation));
        this->addTableWidgetItem(row, column++, "VIDYA", enIndicatorVIDYA, showIndicators.contains(enIndicatorVIDYA));
        this->addTableWidgetItem(row, column++, "AN", enIndicatorAroon, showIndicators.contains(enIndicatorAroon));
        this->addTableWidgetItem(row, column++, "AO", enIndicatorAroonOscillator, showIndicators.contains(enIndicatorAroonOscillator));
        if(!horizontalScreen)
        {
            row++;
            column = 0;
            this->setRowHeight(row, itemHeight);
        }
        this->addTableWidgetItem(row, column++, "CV", enIndicatorChaikinVolatility, showIndicators.contains(enIndicatorChaikinVolatility));
        this->addTableWidgetItem(row, column++, "CMO", enIndicatorChandeMomentumOscillator, showIndicators.contains(enIndicatorChandeMomentumOscillator));
        this->addTableWidgetItem(row, column++, "CCI", enIndicatorCommodityChannelIndex, showIndicators.contains(enIndicatorCommodityChannelIndex));
        this->addTableWidgetItem(row, column++, "DPO", enIndicatorDetrendedPriceOscillator, showIndicators.contains(enIndicatorDetrendedPriceOscillator));
        this->addTableWidgetItem(row, column++, "FCO", enIndicatorFractalChaosOscillator, showIndicators.contains(enIndicatorFractalChaosOscillator));
        row++;
        column = 0;
        this->setRowHeight(row, itemHeight);
        this->addTableWidgetItem(row, column++, "MO", enIndicatorMomentumOscillator, showIndicators.contains(enIndicatorMomentumOscillator));
        this->addTableWidgetItem(row, column++, "PO", enIndicatorPriceOscillator, showIndicators.contains(enIndicatorPriceOscillator));
        this->addTableWidgetItem(row, column++, "RO", enIndicatorRainbowOscillator, showIndicators.contains(enIndicatorRainbowOscillator));
        this->addTableWidgetItem(row, column++, "RSI", enIndicatorRelativeStrengthIndex, showIndicators.contains(enIndicatorRelativeStrengthIndex));
        this->addTableWidgetItem(row, column++, "SKD", enIndicatorStochasticOscillator, showIndicators.contains(enIndicatorStochasticOscillator));
        if(!horizontalScreen)
        {
            row++;
            column = 0;
            this->setRowHeight(row, itemHeight);
        }
        this->addTableWidgetItem(row, column++, "TRIX", enIndicatorTRIX, showIndicators.contains(enIndicatorTRIX));
        this->addTableWidgetItem(row, column++, "TR", enIndicatorTrueRange, showIndicators.contains(enIndicatorTrueRange));
        this->addTableWidgetItem(row, column++, "UO", enIndicatorUltimateOscillator, showIndicators.contains(enIndicatorUltimateOscillator));
        this->addTableWidgetItem(row, column++, "VHF", enIndicatorVerticalHorizontalFilter, showIndicators.contains(enIndicatorVerticalHorizontalFilter));
        this->addTableWidgetItem(row, column++, "WAD", enIndicatorWilliamsAccumulationDistribution, showIndicators.contains(enIndicatorWilliamsAccumulationDistribution));
        row++;
        column = 0;
        this->setRowHeight(row, itemHeight);
        this->addTableWidgetItem(row, column++, "W&R", enIndicatorWilliamsR, showIndicators.contains(enIndicatorWilliamsR));
        this->addTableWidgetItem(row, column++, "ASI", enIndicatorAccumulativeSwingIndex, showIndicators.contains(enIndicatorAccumulativeSwingIndex));
        this->addTableWidgetItem(row, column++, "HML", enIndicatorHighMinusLow, showIndicators.contains(enIndicatorHighMinusLow));
        this->addTableWidgetItem(row, column++, "LRRS", enIndicatorLinearRegressionRSquared, showIndicators.contains(enIndicatorLinearRegressionRSquared));
        this->addTableWidgetItem(row, column++, "LRS", enIndicatorLinearRegressionSlope, showIndicators.contains(enIndicatorLinearRegressionSlope));
        if(!horizontalScreen)
        {
            row++;
            column = 0;
            this->setRowHeight(row, itemHeight);
        }
        this->addTableWidgetItem(row, column++, "MI", enIndicatorMassIndex, showIndicators.contains(enIndicatorMassIndex));
        this->addTableWidgetItem(row, column++, "PI", enIndicatorPerformanceIndex, showIndicators.contains(enIndicatorPerformanceIndex));
        this->addTableWidgetItem(row, column++, "PROC", enIndicatorPriceROC, showIndicators.contains(enIndicatorPriceROC));
        this->addTableWidgetItem(row, column++, "SI", enIndicatorSwingIndex, showIndicators.contains(enIndicatorSwingIndex));
    }

    void SecondaryIndicatorTableWidget::addTableWidgetItem(int row, int column, QString indicatorName, int indicatorType, bool isChecked)
    {
        if(isChecked)
            this->_checkIndicatorType = indicatorType;
        this->setColumnWidth(column, this->width()/(this->_horizontalScreen ? 10 : 5));
        ItemWidget* itemWidget = new ItemWidget(row, column, indicatorType, indicatorName, isChecked, this);
        connect(itemWidget, SIGNAL(checkItemSignal(int , int , int , bool)), this, SLOT(checkItemSlot(int , int , int , bool)));
        this->setCellWidget(row, column, itemWidget);
        this->_items.insert(QString("%1_%2").arg(QString::number(row), QString::number(column)), itemWidget);
    }

    void SecondaryIndicatorTableWidget::checkItemSlot(int row, int column, int indicatorType, bool isChecked)
    {        
        if(isChecked)
        {
            QString key = QString("%1_%2").arg(QString::number(row), QString::number(column));
            QMapIterator<QString, ItemWidget*> item(this->_items);
             while (item.hasNext()) {
                 item.next();
                 if(item.key() != key)
                 {
                     this->_items[item.key()]->setChecked(false);
                 }
             }
             this->_checkIndicatorType = indicatorType;
        }
        emit checkIndicatorSignal(indicatorType, isChecked, false);
    }

    void SecondaryIndicatorTableWidget::hideIndicator()
    {
        QMapIterator<QString, ItemWidget*> item(this->_items);
         while (item.hasNext()) {
             item.next();
             this->_items[item.key()]->setChecked(false, true);
         }
    }

    void SecondaryIndicatorTableWidget::showIndicator()
    {
        QMapIterator<QString, ItemWidget*> item(this->_items);
         while (item.hasNext()) {
             item.next();
             this->_items[item.key()]->setChecked(item.key() == "0_0", true);
         }
    }

    int SecondaryIndicatorTableWidget::getCheckIndicatorType()
    {
        return this->_checkIndicatorType;
    }


    //-----------------------IndicatorSettingDialog------------------------//
    IndicatorSettingDialog::IndicatorSettingDialog(double pixelSizeRatio, HistoricalChart* pView, bool horizontalScreen, QWidget *parent) : QDialog(parent),
        m_ui(new Ui::IndicatorSetting), _pixelSizeRatio(pixelSizeRatio), _pView(pView), _isExistLayout(false), _horizontalScreen(horizontalScreen)
    {
        this->setWindowFlags(Qt::FramelessWindowHint);

        QVector<Indicator*> indicators;
        indicators.append(pView->m_pricePanel->getIndicators());//Main image indicators
        foreach(IndicatorPanel* panel, pView->m_indicatorPanels)//Sub-picture index
        {
            indicators.append(panel->getIndicators());
        }

        foreach(Indicator* indicatorItem, indicators)
        {
            if(indicatorItem->getIndicatorSettings().m_type == enIndicatorMovingAverages)
            {
                if(this->_showIndicatorTypes.contains(enIndicatorMovingAverages))
                {
                    if(!this->_showIndicators[indicatorItem->getIndicatorSettings().m_type]->m_MAIndicators.contains(indicatorItem))
                        this->_showIndicators[indicatorItem->getIndicatorSettings().m_type]->m_MAIndicators.append(indicatorItem);
                }
                else
                {
                    this->_showIndicatorTypes.append(indicatorItem->getIndicatorSettings().m_type);
                    this->_showIndicators.insert(indicatorItem->getIndicatorSettings().m_type, indicatorItem);
                    if(!this->_showIndicators[indicatorItem->getIndicatorSettings().m_type]->m_MAIndicators.contains(indicatorItem))
                        this->_showIndicators[indicatorItem->getIndicatorSettings().m_type]->m_MAIndicators.append(indicatorItem);
                }
            }
            else
            {
                this->_showIndicatorTypes.append(indicatorItem->getIndicatorSettings().m_type);
                this->_showIndicators.insert(indicatorItem->getIndicatorSettings().m_type, indicatorItem);
            }
        }
        this->_currentIndicatorTypeIndex = 0;
        if(this->_showIndicatorTypes.count() > 0)
        {
            this->_currentIndicator = this->_showIndicators[this->_showIndicatorTypes[this->_currentIndicatorTypeIndex]];
            this->m_settings = this->_currentIndicator->getIndicatorSettings();
        }

        QScreen *screen = QApplication::primaryScreen();
        this->setFixedSize(screen->size().width(), screen->size().height());
        qDebug() << "IndicatorSettingDialog: width:" << screen->size().width() << " width:" << screen->size().height();

        this->setObjectName("IndicatorSettingDialog");
        this->setStyleSheet("#IndicatorSettingDialog{ background-color:rgba(0,0,0,150); }");
        m_ui->setupUi(this);

        int hMargin = (horizontalScreen ? 5 : 40) * pixelSizeRatio;
        int moveY = (horizontalScreen ? 25 : 40) * pixelSizeRatio;
#if defined (RUNTIME_OS_IPHONE)
        moveY = (horizontalScreen ? 25 : 40) * pixelSizeRatio;
#endif
        int contentHeight = screen->size().height() - 2*hMargin - (horizontalScreen ? 6*hMargin: 0);
        int wMagrin = 10 * pixelSizeRatio;
        int contentWidth = screen->size().width() - 2*wMagrin;
        int contentMagrin = 10;
        m_ui->IS_Content_GroupBox->setGeometry(wMagrin, moveY, contentWidth, contentHeight);

        int itemHeight = (contentHeight)/(horizontalScreen ? 8 : 11);
        moveY = itemHeight;

        m_ui->IS_MainChart_GroupBox->move(0, 0);
        m_ui->IS_MainChart_GroupBox->setFixedSize(contentWidth, itemHeight);
        m_ui->IS_MainChart_Widget->move(0, moveY);
        m_ui->IS_MainChart_Widget->setFixedSize(contentWidth, itemHeight*(horizontalScreen ? 3 : 5));
        m_ui->IS_Secondary_Widget->move(0, moveY);
        m_ui->IS_Secondary_Widget->setFixedSize(contentWidth, itemHeight*(horizontalScreen ? 3 : 5));
        this->_mainIndicatorListWidget = new MainIndicatorListWidget(itemHeight*(horizontalScreen ? 3 : 5), contentWidth, this->_showIndicatorTypes, this->_horizontalScreen, m_ui->IS_MainChart_Widget);
        this->_secondaryIndicatorTableWidget = new SecondaryIndicatorTableWidget(itemHeight*(horizontalScreen ? 3 : 5), contentWidth, this->_showIndicatorTypes, this->_horizontalScreen, m_ui->IS_Secondary_Widget);

        int indicatorType = this->_secondaryIndicatorTableWidget->getCheckIndicatorType();
        if(indicatorType!=-1)
            this->_secondaryIndicatorType = (enIndicatorType)indicatorType;

        moveY = moveY + itemHeight*(horizontalScreen ? 3 : 5);

        m_ui->IS_Parameter_GroupBox->move(contentMagrin, moveY);
        m_ui->IS_Parameter_GroupBox->setFixedSize(contentWidth - 2 * contentMagrin, itemHeight);
        moveY = moveY + itemHeight;
        m_ui->IS_Parameter_Widget->move(0, moveY);
        m_ui->IS_Parameter_Widget->setFixedSize(contentWidth, itemHeight*(horizontalScreen ? 3 : 4));


        qDebug() << " itemHeight:" << itemHeight;

        int buttonHeight = (horizontalScreen ? itemHeight - 4 : 40  * pixelSizeRatio);
        int marginTop = (itemHeight - buttonHeight)/2;        
        int bWidth = (contentWidth - 3 * contentMagrin)/5;
        int moveX = contentMagrin;
        m_ui->IS_MainChartTab_Button->move(moveX, marginTop);
        m_ui->IS_MainChartTab_Button->setFixedSize(bWidth*2, buttonHeight);
        moveX += bWidth*2;
        m_ui->IS_SecondaryTab_Button->move(moveX, marginTop);
        m_ui->IS_SecondaryTab_Button->setFixedSize(bWidth*2, buttonHeight);
        moveX += bWidth*2 + contentMagrin;
        m_ui->IS_Close_Button->move(moveX, marginTop);
        m_ui->IS_Close_Button->setFixedSize(bWidth, buttonHeight);

        m_ui->IS_Parameter_Label->move(0, marginTop);
        m_ui->IS_Parameter_Label->setFixedSize((contentWidth/6)*2 - contentMagrin, buttonHeight);
        m_ui->IS_Parameter_Button->move(m_ui->IS_Parameter_Label->width(), marginTop);
        m_ui->IS_Parameter_Button->setFixedSize((contentWidth/6)*4 - contentMagrin, buttonHeight);

        m_ui->IS_MainChartTab_Button->setText(SR::get("MainChartIndicator"));
        m_ui->IS_SecondaryTab_Button->setText(SR::get("SecondaryIndicator"));
        m_ui->IS_Close_Button->setText(SR::get("Close"));
        m_ui->IS_Parameter_Label->setText(SR::get("ParameterModify"));

        if(pView->m_language == "ENG")
        {
            QFont font = m_ui->IS_MainChartTab_Button->font();
            font.setPointSize(13);
            m_ui->IS_MainChartTab_Button->setFont(font);
            m_ui->IS_SecondaryTab_Button->setFont(font);
            m_ui->IS_Close_Button->setFont(font);
            m_ui->IS_Parameter_Label->setFont(font);
        }

        connect(m_ui->IS_Close_Button, SIGNAL(clicked()), this, SLOT(closeSlot()));
        connect(m_ui->IS_MainChartTab_Button, SIGNAL(clicked()), this, SLOT(changeMainCharTabSlot()));
        connect(m_ui->IS_SecondaryTab_Button, SIGNAL(clicked()), this, SLOT(changeSecondaryTabSlot()));
        connect(m_ui->IS_Parameter_Button, SIGNAL(clicked()), this, SLOT(changeSelectChartIndicatorSlot()));
        connect(this->_mainIndicatorListWidget, SIGNAL(checkIndicatorSignal(int, bool, bool)), this, SLOT(checkIndicatorSlot(int, bool, bool)));
        connect(this->_secondaryIndicatorTableWidget, SIGNAL(checkIndicatorSignal(int, bool, bool)), this, SLOT(checkIndicatorSlot(int, bool, bool)));

        this->changeMainCharTabSlot();
        this->changeSelectChartIndicatorSlot();
    }

    IndicatorSettingDialog::~IndicatorSettingDialog()
    {
        qDebug()<< " IndicatorSettingDialog::~IndicatorSettingDialog start";
        delete _secondaryIndicatorTableWidget;
        delete _mainIndicatorListWidget;
        delete m_ui;
         qDebug()<< " IndicatorSettingDialog::~IndicatorSettingDialog end";
    }

    void IndicatorSettingDialog::showDialog()
    {
        this->show();
        this->raise();
        this->activateWindow();
    }

    bool IndicatorSettingDialog::event(QEvent *event)
    {
        if (event->type() == QEvent::ShortcutOverride)
        {
            this->accepted();
            return false;
        }
        return QWidget::event(event);
    }

    void IndicatorSettingDialog::closeSlot()
    {
        this->rejected();
        this->deleteLater();
    }

    void IndicatorSettingDialog::changeMainCharTabSlot()
    {
        m_ui->IS_MainChartTab_Button->setStyleSheet("background-color: #b99f37; color: #ffffff; border:1px solid  #b99f37; border-top-left-radius:10px;");
        m_ui->IS_SecondaryTab_Button->setStyleSheet("background-color: transparent; border:1px solid #cfcfcf; color: #000000; border-top-right-radius:10px;");
        m_ui->IS_MainChart_Widget->setVisible(true);
        m_ui->IS_Secondary_Widget->setVisible(false);
    }

    void IndicatorSettingDialog::changeSecondaryTabSlot()
    {
        m_ui->IS_MainChartTab_Button->setStyleSheet("background-color: transparent; border:1px solid #cfcfcf; color: #000000; border-top-left-radius:10px;");
        m_ui->IS_SecondaryTab_Button->setStyleSheet("background-color: #b99f37; color: #ffffff; border:1px solid  #b99f37; border-top-right-radius:10px;");
        m_ui->IS_MainChart_Widget->setVisible(false);
        m_ui->IS_Secondary_Widget->setVisible(true);

    }

    void IndicatorSettingDialog::changeSelectChartIndicatorSlot()
    {
        if(this->_showIndicatorTypes.count() > 0)
        {
            int maxIndex = this->_showIndicatorTypes.count() -1;
            this->_currentIndicatorTypeIndex++;
            if(this->_currentIndicatorTypeIndex > maxIndex)
                this->_currentIndicatorTypeIndex = 0;
            this->_currentIndicator = this->_showIndicators[this->_showIndicatorTypes[this->_currentIndicatorTypeIndex]];
            this->m_settings = this->_currentIndicator->getIndicatorSettings();
            m_ui->IS_Parameter_Button->setText(IndicatorCore::getIndicatorName(this->_showIndicatorTypes[this->_currentIndicatorTypeIndex]));
            if(_horizontalScreen)
                this->generateHorizontalScreenIndicatorProperty();
            else
               this->generateIndicatorProperty();
        }
    }

    void IndicatorSettingDialog::checkIndicatorSlot(int indicatorType, bool isChecked, bool isMainChart)
    {
        qDebug() << " IndicatorSettingDialog::checkIndicatorSlot indicatorType:" << indicatorType << " isChecked:" << isChecked;
        enIndicatorType indicatorTypeEnum = (enIndicatorType)indicatorType;
        if(isChecked)
         {
             if(indicatorTypeEnum == enIndicatorMovingAverages)
             {
                 this->_showIndicatorTypes.append(indicatorTypeEnum);
                 this->_currentIndicatorTypeIndex = this->_showIndicatorTypes.indexOf(indicatorTypeEnum);
                 this->_currentIndicator = this->_pView->AddIndicator(indicatorType);
                 this->_showIndicators.insert(indicatorTypeEnum, this->_currentIndicator);
                 QHash<QString,int>::const_iterator it = this->_currentIndicator->m_settings.m_hashInputIntData.begin();
                 for(it; it != this->_currentIndicator->m_settings.m_hashInputIntData.end(); ++it)
                 {
                     this->_currentIndicator->m_settings.m_hashInputIntData[it.key()] = 5;
                     this->_currentIndicator->m_settings.m_vectLineSettings[0].m_lineColor = Qt::darkCyan;
                     this->_currentIndicator->m_settings.m_vectLineSettings[0].m_lineWidth = 1.5;
                 }
                 this->_currentIndicator->getCalculator()->beginCalculateAll(this->_currentIndicator);
                 this->_currentIndicator->getCalculator()->addEndTask();   // always call addEndTask() to refresh UI.
                 this->_currentIndicator->m_MAIndicators.append(this->_currentIndicator);

                 Indicator* indicator10 = this->_pView->AddIndicator(indicatorType);
                 QHash<QString,int>::const_iterator it10 = indicator10->m_settings.m_hashInputIntData.begin();
                 for(it10; it10 != indicator10->m_settings.m_hashInputIntData.end(); ++it10)
                 {
                     indicator10->m_settings.m_hashInputIntData[it10.key()] = 10;
                     indicator10->m_settings.m_vectLineSettings[0].m_lineColor = Qt::yellow;
                     indicator10->m_settings.m_vectLineSettings[0].m_lineWidth = 1.5;
                 }
                 indicator10->getCalculator()->beginCalculateAll(indicator10);
                 indicator10->getCalculator()->addEndTask();   // always call addEndTask() to refresh UI.
                 this->_currentIndicator->m_MAIndicators.append(indicator10);

                 Indicator* indicator20 = this->_pView->AddIndicator(indicatorType);
                 QHash<QString,int>::const_iterator it20 = indicator20->m_settings.m_hashInputIntData.begin();
                 for(it20; it20 != indicator20->m_settings.m_hashInputIntData.end(); ++it20)
                 {
                     indicator20->m_settings.m_hashInputIntData[it20.key()] = 20;
                     indicator20->m_settings.m_vectLineSettings[0].m_lineColor = Qt::darkGreen;
                     indicator20->m_settings.m_vectLineSettings[0].m_lineWidth = 1.5;
                 }
                 indicator20->getCalculator()->beginCalculateAll(indicator20);
                 indicator20->getCalculator()->addEndTask();   // always call addEndTask() to refresh UI.
                 this->_currentIndicator->m_MAIndicators.append(indicator20);
#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
                 this->_pView->AddIndicatorToIndicatorSettingPanel(true, enIndicatorMovingAverages);
#endif
             }
             else
             {
                 if(!isMainChart && this->_showIndicatorTypes.contains(this->_secondaryIndicatorType))
                 {
                    this->_showIndicatorTypes.removeOne(this->_secondaryIndicatorType);
                    this->_showIndicators.remove(this->_secondaryIndicatorType);
                 }
                 this->_showIndicatorTypes.append(indicatorTypeEnum);
                 this->_currentIndicatorTypeIndex = this->_showIndicatorTypes.indexOf(indicatorTypeEnum);
                 this->_currentIndicator = this->_pView->AddIndicator(indicatorType);
                 this->_showIndicators.insert(indicatorTypeEnum, this->_currentIndicator);
                 if (this->_currentIndicator->m_settings.isDataValid())
                 {
                     for (int i = 0; i < this->_currentIndicator->m_settings.m_plotNum; ++i)
                     {
                         this->_currentIndicator->m_settings.m_vectLineSettings[i].m_lineWidth = 1.5;
                     }
                 }
                 this->refreshIndicator();
#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
                 this->_pView->AddIndicatorToIndicatorSettingPanel(isMainChart, indicatorTypeEnum);
#endif
                 if(!isMainChart)
                     this->_secondaryIndicatorType = indicatorTypeEnum;
             }
             this->m_settings = this->_currentIndicator->getIndicatorSettings();

             if(this->_showIndicatorTypes.count() > 0)
                 m_ui->IS_Parameter_Button->setText(IndicatorCore::getIndicatorName(this->_showIndicatorTypes[this->_currentIndicatorTypeIndex]));

             if(_horizontalScreen)
                 this->generateHorizontalScreenIndicatorProperty();
             else
                this->generateIndicatorProperty();
         }
         else
         {
             int newIndex = this->_showIndicatorTypes.indexOf(indicatorTypeEnum);
             if(this->_currentIndicatorTypeIndex >= newIndex)
             {
                 this->_currentIndicatorTypeIndex = 0;
             }
             this->_showIndicatorTypes.removeOne(indicatorTypeEnum);
             Indicator* indicator = this->_showIndicators[indicatorTypeEnum];
             if(indicatorTypeEnum == enIndicatorMovingAverages)
             {
                 foreach (Indicator* maIndicator, indicator->m_MAIndicators)
                    this->_pView->m_pricePanel->removeIndicator(maIndicator);
                 this->_pView->m_pricePanel->removeIndicator(indicator);
             }
             else
             {
                 if(isMainChart)
                     this->_pView->m_pricePanel->removeIndicator(indicator);
                 else
                    this->_pView->m_indicatorPanels[0]->removeIndicator(indicator);
             }
             this->_showIndicators.remove(indicatorTypeEnum);
#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
             this->_pView->RemoveIndicatorToIndicatorSettingPanel(isMainChart, indicatorTypeEnum);
#endif
             if(this->_showIndicatorTypes.count() > 0)
             {
                 this->changeSelectChartIndicatorSlot();
             }
             else
             {
                 if(_horizontalScreen)
                     this->generateHorizontalScreenIndicatorProperty();
                 else
                    this->generateIndicatorProperty();
             }
         }

        qDebug() << "IndicatorSettingDialog::checkIndicatorSlot end";
    }

    void IndicatorSettingDialog::handleStandColorDialogSlot()
    {
        QToolButton * button = dynamic_cast<QToolButton*>(sender());
        if(button == NULL) return;
        QString strBackground = "border:1px solid #cfcfcf; border-radius:5px; background-color:%1";
        QString oldStyle = button->styleSheet();
        qDebug() <<"IndicatorSettingDialog::handleStandColorDialogSlot oldStyle:"<< oldStyle;
        QString oldColor =  oldStyle.mid(oldStyle.indexOf("background-color:")+QString("background-color:").length());
        ColorComboxWidget* comboxWidget = new ColorComboxWidget(this->_horizontalScreen, this->_pixelSizeRatio, oldColor, this);
        connect(comboxWidget, &ColorComboxWidget::selectChangeSignal, [=](QString colorName)
        {
            button->setStyleSheet(strBackground.arg(colorName));
            this->updateIndicatorProperty();            
            delete comboxWidget;
        });
        connect(comboxWidget, &ColorComboxWidget::cancelSignal, [=]()
        {
            delete comboxWidget;
        });
        comboxWidget->showDialog();
    }

    void IndicatorSettingDialog::editingFinishedSlot()
    {
        InputLineEdit* subLineEdit = (InputLineEdit*)sender();
        if(subLineEdit->text().length() > 0)
            this->updateIndicatorProperty();
    }

    void IndicatorSettingDialog::generateIndicatorProperty()
    {
        if(_isExistLayout)
        {
            this->m_vecIntInputLineEdit.clear();
            this->m_vecDoubleInputLineEdit.clear();
            this->m_vecColorPushButton.clear();
            this->m_MAvecIntInputLineEdit.clear();
            this->m_MAvecDoubleInputLineEdit.clear();
            this->m_MAvecColorPushButton.clear();

            QFrame *fm = m_ui->IS_Parameter_Widget->findChild<QFrame*>("IS_ParameterContent_Frame");
            m_ui->verticalLayout->removeWidget(fm);
            delete fm;
        }
        this->update();

        if(this->_showIndicators.count() == 0) {
            m_ui->IS_Parameter_Button->setText("");
            return;
        }

        QFrame * parameterContent_Frame = new QFrame(m_ui->IS_Parameter_Widget);          //Initialize a custom control class
        parameterContent_Frame->setObjectName(tr("IS_ParameterContent_Frame"));    //Set the name of the new object
        m_ui->verticalLayout->addWidget(parameterContent_Frame);

        parameterContent_Frame->move(0, 0);
        parameterContent_Frame->setFixedSize(m_ui->IS_Parameter_Widget->width()-20, m_ui->IS_Parameter_Widget->height());
        QVBoxLayout* parameterContentLayout = new QVBoxLayout(parameterContent_Frame);
        parameterContentLayout->setSpacing(4);
        parameterContentLayout->setContentsMargins(0, 0, 0, 0);
        //int oneAliquotWidth = m_ui->IS_Parameter_Button->width()/4;
        int oneAliquotHeight = (m_ui->IS_Parameter_Widget->height()- 28)/6;
        int remainderHeight = m_ui->IS_Parameter_Widget->height();
        QFont font = m_ui->IS_Parameter_Label->font();
        int margin = 4;
        if(m_settings.m_type == enIndicatorMovingAverages)
        {
            foreach (Indicator* maIndicator, this->_currentIndicator->m_MAIndicators) {
                IndicatorSettings settings = maIndicator->getIndicatorSettings();

                // input Parameter
                //int type Parameter
                {
                    QHash<QString,int>::const_iterator it = settings.m_hashInputIntData.begin();
                    for(it; it != settings.m_hashInputIntData.end(); ++it)
                    {
                        QLabel* label = new QLabel(SR::get(it.key()) + ": ", parameterContent_Frame);
                        label->setFont(font);
                        InputLineEdit* subLineEdit = new InputLineEdit(QString::number(it.value()), parameterContent_Frame);
                        subLineEdit->setDataString(it.key());
                        int min, max;
                        _currentIndicator->getParameterRange(it.key(), min, max);
                        subLineEdit->setValidator(new QIntValidator(min, max, subLineEdit));
                        subLineEdit->setOldValue(QString::number(it.value()));
                        QObject::connect(subLineEdit, &InputLineEdit::textChanged, this, [=] (const QString & value) {
                            int newValue = value.toInt();
                            if(newValue<min || newValue>max)
                            {
                                newValue = subLineEdit->getOldValue().toInt();
                                subLineEdit->setOldValue(QString::number(newValue));
                                subLineEdit->setText(QString::number(newValue));
                            }
                            else
                                subLineEdit->setOldValue(QString::number(newValue));
                        });
                        label->setBuddy(subLineEdit);

                        label->setMinimumHeight(oneAliquotHeight);
                        subLineEdit->setMinimumHeight(oneAliquotHeight);
                        label->setMaximumHeight(oneAliquotHeight);
                        subLineEdit->setMaximumHeight(oneAliquotHeight);
                        remainderHeight = remainderHeight - oneAliquotHeight - margin;

                        QHBoxLayout* horizentalLayout = new QHBoxLayout(parameterContent_Frame);
                        horizentalLayout->setSpacing(0);
                        horizentalLayout->setContentsMargins(10, 0, 0, 0);
                        horizentalLayout->addWidget(label, 2);
                        horizentalLayout->addWidget(subLineEdit, 4);
                        parameterContentLayout->addLayout(horizentalLayout);
                        m_MAvecIntInputLineEdit.insert(maIndicator, subLineEdit);

                        connect(subLineEdit, SIGNAL(editingFinished()), this, SLOT(editingFinishedSlot()));

                        qDebug() << " remainderHeight:" << remainderHeight;
                    }
                }

                // double  type Parameter
                {
                    QHash<QString,double>::const_iterator it = settings.m_hashInputDoubleData.begin();
                    for(it; it != settings.m_hashInputDoubleData.end(); ++it)
                    {
                        QLabel* label = new QLabel(it.key() + ": ", parameterContent_Frame);
                        label->setFont(font);
                        InputLineEdit* subLineEdit = new InputLineEdit(QString::number(it.value()), parameterContent_Frame);
                        subLineEdit->setDataString(it.key());
                        double min, max, decimals;
                        _currentIndicator->getParameterRange(it.key(), min, max, decimals);
                        subLineEdit->setValidator(new QDoubleValidator(min, max, decimals, subLineEdit));
                        label->setBuddy(subLineEdit);

                        label->setMinimumHeight(oneAliquotHeight);
                        subLineEdit->setMinimumHeight(oneAliquotHeight);
                        label->setMaximumHeight(oneAliquotHeight);
                        subLineEdit->setMaximumHeight(oneAliquotHeight);
                        remainderHeight = remainderHeight - oneAliquotHeight - margin;

                        QHBoxLayout* horizentalLayout = new QHBoxLayout(parameterContent_Frame);
                        horizentalLayout->setSpacing(0);
                        horizentalLayout->setContentsMargins(10, 0, 0, 0);
                        horizentalLayout->addWidget(label, 2);
                        horizentalLayout->addWidget(subLineEdit, 4);
                        parameterContentLayout->addLayout(horizentalLayout);
                        m_MAvecDoubleInputLineEdit.insert(maIndicator, subLineEdit);

                        connect(subLineEdit, SIGNAL(editingFinished()), this, SLOT(editingFinishedSlot()));

                        qDebug() << " remainderHeight:" << remainderHeight;
                    }
                }

                // color，lineWidth,lineType
                if (settings.isDataValid())
                {
                    const QString& strBackground = "border:1px solid #cfcfcf; border-radius:5px; background-color:";
                    for (int i = 0; i < settings.m_plotNum; ++i)
                    {
                        QString str = settings.m_vectLineSettings[i].m_lineName + ": ";
                        QLabel* label = new QLabel(str, parameterContent_Frame);
                        label->setFont(font);
                        QToolButton* colorButton = new QToolButton(parameterContent_Frame);
                        colorButton->setStyleSheet(strBackground + settings.m_vectLineSettings[i].m_lineColor.name());
                        connect(colorButton,SIGNAL(clicked()),this,SLOT(handleStandColorDialogSlot()));

                        label->setMinimumHeight(oneAliquotHeight);
                        colorButton->setMaximumSize(m_ui->IS_Parameter_Button->width(), oneAliquotHeight);
                        label->setMaximumHeight(oneAliquotHeight);
                        colorButton->setMaximumSize(m_ui->IS_Parameter_Button->width(), oneAliquotHeight);
                        remainderHeight = remainderHeight - oneAliquotHeight - margin;

                        QHBoxLayout* horizentalLayout = new QHBoxLayout(parameterContent_Frame);
                        horizentalLayout->setSpacing(0);
                        horizentalLayout->setContentsMargins(10, 0, 0, 0);
                        horizentalLayout->addWidget(label, 2);
                        horizentalLayout->addWidget(colorButton, 4);
                        parameterContentLayout->addLayout(horizentalLayout);

                        m_MAvecColorPushButton.insert(maIndicator, colorButton);

                        qDebug() << " remainderHeight:" << remainderHeight;

                    }
                }


            }
        }
        else
        {
            // input Parameter
            //int  type Parameter
            {
                QHash<QString,int>::const_iterator it = m_settings.m_hashInputIntData.begin();
                for(it; it != m_settings.m_hashInputIntData.end(); ++it)
                {
                    QLabel* label = new QLabel(SR::get(it.key()) + ": ", parameterContent_Frame);
                    label->setFont(font);
                    InputLineEdit* subLineEdit = new InputLineEdit(QString::number(it.value()), parameterContent_Frame);
                    subLineEdit->setDataString(it.key());
                    int min, max;
                    _currentIndicator->getParameterRange(it.key(), min, max);
                    subLineEdit->setValidator(new QIntValidator(min, max, subLineEdit));
                    subLineEdit->setOldValue(QString::number(it.value()));
                    QObject::connect(subLineEdit, &InputLineEdit::textChanged, this, [=] (const QString & value) {
                        int newValue = value.toInt();
                        if(newValue<min || newValue>max)
                        {
                            newValue = subLineEdit->getOldValue().toInt();
                            subLineEdit->setOldValue(QString::number(newValue));
                            subLineEdit->setText(QString::number(newValue));
                        }
                        else
                            subLineEdit->setOldValue(QString::number(newValue));
                    });
                    label->setBuddy(subLineEdit);

                    label->setMinimumHeight(oneAliquotHeight);
                    subLineEdit->setMinimumHeight(oneAliquotHeight);
                    label->setMaximumHeight(oneAliquotHeight);
                    subLineEdit->setMaximumHeight(oneAliquotHeight);
                    remainderHeight = remainderHeight - oneAliquotHeight - margin;

                    QHBoxLayout* horizentalLayout = new QHBoxLayout(parameterContent_Frame);
                    horizentalLayout->setSpacing(0);
                    horizentalLayout->setContentsMargins(10, 0, 0, 0);
                    horizentalLayout->addWidget(label, 2);
                    horizentalLayout->addWidget(subLineEdit, 4);
                    parameterContentLayout->addLayout(horizentalLayout);
                    m_vecIntInputLineEdit.append(subLineEdit);

                    connect(subLineEdit, SIGNAL(editingFinished()), this, SLOT(editingFinishedSlot()));

                    qDebug() << " remainderHeight:" << remainderHeight;
                }
            }

            // double type Parameter
            {
                QHash<QString,double>::const_iterator it = m_settings.m_hashInputDoubleData.begin();
                for(it; it != m_settings.m_hashInputDoubleData.end(); ++it)
                {
                    QLabel* label = new QLabel(it.key() + ": ", parameterContent_Frame);
                    label->setFont(font);
                    InputLineEdit* subLineEdit = new InputLineEdit(QString::number(it.value()), parameterContent_Frame);
                    subLineEdit->setDataString(it.key());
                    double min, max, decimals;
                    _currentIndicator->getParameterRange(it.key(), min, max, decimals);
                    subLineEdit->setValidator(new QDoubleValidator(min, max, decimals, subLineEdit));
                    label->setBuddy(subLineEdit);

                    label->setMinimumHeight(oneAliquotHeight);
                    subLineEdit->setMinimumHeight(oneAliquotHeight);
                    label->setMaximumHeight(oneAliquotHeight);
                    subLineEdit->setMaximumHeight(oneAliquotHeight);
                    remainderHeight = remainderHeight - oneAliquotHeight - margin;

                    QHBoxLayout* horizentalLayout = new QHBoxLayout(parameterContent_Frame);
                    horizentalLayout->setSpacing(0);
                    horizentalLayout->setContentsMargins(10, 0, 0, 0);
                    horizentalLayout->addWidget(label, 2);
                    horizentalLayout->addWidget(subLineEdit, 4);
                    parameterContentLayout->addLayout(horizentalLayout);
                    m_vecDoubleInputLineEdit.append(subLineEdit);

                    connect(subLineEdit, SIGNAL(editingFinished()), this, SLOT(editingFinishedSlot()));

                    qDebug() << " remainderHeight:" << remainderHeight;
                }
            }

            // color，lineWidth,lineType
            if (m_settings.isDataValid())
            {
                const QString& strBackground = "border:1px solid #cfcfcf; border-radius:5px; background-color:";
                for (int i = 0; i < m_settings.m_plotNum; ++i)
                {
                    QString str = m_settings.m_vectLineSettings[i].m_lineName + ": ";
                    QLabel* label = new QLabel(str, parameterContent_Frame);
                    label->setFont(font);
                    QToolButton* colorButton = new QToolButton(parameterContent_Frame);
                    colorButton->setStyleSheet(strBackground + m_settings.m_vectLineSettings[i].m_lineColor.name());
                    connect(colorButton,SIGNAL(clicked()),this,SLOT(handleStandColorDialogSlot()));

                    label->setMinimumHeight(oneAliquotHeight);
                    colorButton->setMaximumSize(m_ui->IS_Parameter_Button->width(), oneAliquotHeight);
                    label->setMaximumHeight(oneAliquotHeight);
                    colorButton->setMaximumSize(m_ui->IS_Parameter_Button->width(), oneAliquotHeight);
                    remainderHeight = remainderHeight - oneAliquotHeight - margin;

                    QHBoxLayout* horizentalLayout = new QHBoxLayout(parameterContent_Frame);
                    horizentalLayout->setSpacing(0);
                    horizentalLayout->setContentsMargins(10, 0, 0, 0);
                    horizentalLayout->addWidget(label, 2);
                    horizentalLayout->addWidget(colorButton, 4);
                    parameterContentLayout->addLayout(horizentalLayout);

                    m_vecColorPushButton.push_back(colorButton);

                    qDebug() << " remainderHeight:" << remainderHeight;

                }
            }
        }

        QSpacerItem* spacerItem = new QSpacerItem(m_ui->IS_Parameter_Button->width(), remainderHeight, QSizePolicy::Fixed, QSizePolicy::Expanding);
        parameterContentLayout->addItem(spacerItem);
        this->_isExistLayout = true;


        qDebug() << " generateIndicatorProperty333";
        parameterContent_Frame->setLayout(parameterContentLayout);        
        qDebug() << " generateIndicatorProperty444";
        this->update();
        qDebug() << " generateIndicatorProperty555";
    }

    void IndicatorSettingDialog::generateHorizontalScreenIndicatorProperty()
    {
        if(_isExistLayout)
        {
            this->m_vecIntInputLineEdit.clear();
            this->m_vecDoubleInputLineEdit.clear();
            this->m_vecColorPushButton.clear();
            this->m_MAvecIntInputLineEdit.clear();
            this->m_MAvecDoubleInputLineEdit.clear();
            this->m_MAvecColorPushButton.clear();

            QFrame *fm = m_ui->IS_Parameter_Widget->findChild<QFrame*>("IS_ParameterContent_Frame");
            m_ui->verticalLayout->removeWidget(fm);
            delete fm;
        }
        this->update();

        if(this->_showIndicators.count() == 0) return;


        QFrame * parameterContent_Frame = new QFrame(m_ui->IS_Parameter_Widget);          //Initialize a custom control class
        parameterContent_Frame->setObjectName(tr("IS_ParameterContent_Frame"));    //Set the name of the new object
        m_ui->verticalLayout->addWidget(parameterContent_Frame);

        parameterContent_Frame->move(0, 0);
        parameterContent_Frame->setFixedSize(m_ui->IS_Parameter_Widget->width()-20, m_ui->IS_Parameter_Widget->height());
        QGridLayout* parameterContentLayout = new QGridLayout(parameterContent_Frame);
        parameterContentLayout->setVerticalSpacing(2);
        parameterContentLayout->setHorizontalSpacing(5);
        parameterContentLayout->setContentsMargins(10, 0, 0, 10);
        int oneAliquotHeight = (m_ui->IS_Parameter_Widget->height()- 30)/3;
        int remainderHeight = m_ui->IS_Parameter_Widget->height();
        int margin = 2;
        int row = 0;
        int column = 0;
        QFont font = m_ui->IS_Parameter_Label->font();
        if(m_settings.m_type == enIndicatorMovingAverages)
        {
            foreach (Indicator* maIndicator, this->_currentIndicator->m_MAIndicators) {
                IndicatorSettings settings = maIndicator->getIndicatorSettings();

                // input Parameter
                //int type Parameter
                {
                    QHash<QString,int>::const_iterator it = settings.m_hashInputIntData.begin();
                    for(it; it != settings.m_hashInputIntData.end(); ++it)
                    {
                        QLabel* label = new QLabel(SR::get(it.key()) + ": ", parameterContent_Frame);
                        label->setFont(font);
                        InputLineEdit* subLineEdit = new InputLineEdit(QString::number(it.value()), parameterContent_Frame);
                        subLineEdit->setDataString(it.key());
                        int min, max;
                        _currentIndicator->getParameterRange(it.key(), min, max);
                        subLineEdit->setValidator(new QIntValidator(min, max, subLineEdit));
                        label->setBuddy(subLineEdit);

                        label->setMinimumHeight(oneAliquotHeight);
                        subLineEdit->setMinimumHeight(oneAliquotHeight);
                        label->setMaximumHeight(oneAliquotHeight);
                        subLineEdit->setMaximumHeight(oneAliquotHeight);


                        QHBoxLayout* horizentalLayout = new QHBoxLayout(parameterContent_Frame);
                        horizentalLayout->setSpacing(0);
                        horizentalLayout->setContentsMargins(0, 0, 0, 0);
                        horizentalLayout->addWidget(label, 2);
                        horizentalLayout->addWidget(subLineEdit, 3);
                        parameterContentLayout->addLayout(horizentalLayout, row, column);
                        qDebug() << "1111111 row:" << row << " column:" << column;
                        if(column == 0)
                        {
                            column++;
                        }
                        else
                        {
                            remainderHeight = remainderHeight - oneAliquotHeight - margin;
                            row++;
                            column = 0;
                        }
                        m_MAvecIntInputLineEdit.insert(maIndicator, subLineEdit);

                        connect(subLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(textChangedSlot(const QString &)));

                        qDebug() << " remainderHeight:" << remainderHeight;
                    }
                }

                // double type Parameter
                {
                    QHash<QString,double>::const_iterator it = settings.m_hashInputDoubleData.begin();
                    for(it; it != settings.m_hashInputDoubleData.end(); ++it)
                    {
                        QLabel* label = new QLabel(it.key() + ": ", parameterContent_Frame);
                        label->setFont(font);
                        InputLineEdit* subLineEdit = new InputLineEdit(QString::number(it.value()), parameterContent_Frame);
                        subLineEdit->setDataString(it.key());
                        double min, max, decimals;
                        _currentIndicator->getParameterRange(it.key(), min, max, decimals);
                        subLineEdit->setValidator(new QDoubleValidator(min, max, decimals, subLineEdit));
                        label->setBuddy(subLineEdit);

                        label->setMinimumHeight(oneAliquotHeight);
                        subLineEdit->setMinimumHeight(oneAliquotHeight);
                        label->setMaximumHeight(oneAliquotHeight);
                        subLineEdit->setMaximumHeight(oneAliquotHeight);

                        QHBoxLayout* horizentalLayout = new QHBoxLayout(parameterContent_Frame);
                        horizentalLayout->setSpacing(0);
                        horizentalLayout->setContentsMargins(0, 0, 0, 0);
                        horizentalLayout->addWidget(label, 2);
                        horizentalLayout->addWidget(subLineEdit, 3);
                        parameterContentLayout->addLayout(horizentalLayout, row, column);
                        qDebug() << "222222222 row:" << row << " column:" << column;
                        if(column == 0)
                        {
                            column++;
                        }
                        else
                        {
                            remainderHeight = remainderHeight - oneAliquotHeight - margin;
                            row++;
                            column = 0;
                        }
                        m_MAvecDoubleInputLineEdit.insert(maIndicator, subLineEdit);

                        connect(subLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(textChangedSlot(const QString &)));

                        qDebug() << " remainderHeight:" << remainderHeight;
                    }
                }

                // color，lineWidth,lineType
                if (settings.isDataValid())
                {
                    const QString& strBackground = "border:1px solid #cfcfcf; border-radius:5px; background-color:";
                    for (int i = 0; i < settings.m_plotNum; ++i)
                    {
                        QString str = settings.m_vectLineSettings[i].m_lineName + ": ";
                        QLabel* label = new QLabel(str, parameterContent_Frame);
                        label->setFont(font);
                        QToolButton* colorButton = new QToolButton(parameterContent_Frame);
                        colorButton->setStyleSheet(strBackground + settings.m_vectLineSettings[i].m_lineColor.name());
                        connect(colorButton,SIGNAL(clicked()),this,SLOT(handleStandColorDialogSlot()));

                        label->setMinimumHeight(oneAliquotHeight);
                        colorButton->setMaximumSize(m_ui->IS_Parameter_Button->width(), oneAliquotHeight);
                        label->setMaximumHeight(oneAliquotHeight);
                        colorButton->setMaximumSize(m_ui->IS_Parameter_Button->width(), oneAliquotHeight);

                        QHBoxLayout* horizentalLayout = new QHBoxLayout(parameterContent_Frame);
                        horizentalLayout->setSpacing(0);
                        horizentalLayout->setContentsMargins(0, 0, 0, 0);
                        horizentalLayout->addWidget(label, 2);
                        horizentalLayout->addWidget(colorButton, 3);
                        parameterContentLayout->addLayout(horizentalLayout, row, column);
                        qDebug() << "33333333 row:" << row << " column:" << column;
                        if(column == 0)
                        {
                            column++;
                        }
                        else
                        {
                            remainderHeight = remainderHeight - oneAliquotHeight - margin;
                            row++;
                            column = 0;
                        }

                        m_MAvecColorPushButton.insert(maIndicator, colorButton);

                        qDebug() << " remainderHeight:" << remainderHeight;

                    }
                }


            }
        }
        else
        {
            // input Parameter
            //int type Parameter
            {
                QHash<QString,int>::const_iterator it = m_settings.m_hashInputIntData.begin();
                for(it; it != m_settings.m_hashInputIntData.end(); ++it)
                {
                    QLabel* label = new QLabel(SR::get(it.key()) + ": ", parameterContent_Frame);
                    label->setFont(font);
                    InputLineEdit* subLineEdit = new InputLineEdit(QString::number(it.value()), parameterContent_Frame);
                    subLineEdit->setDataString(it.key());
                    int min, max;
                    _currentIndicator->getParameterRange(it.key(), min, max);
                    subLineEdit->setValidator(new QIntValidator(min, max, subLineEdit));
                    label->setBuddy(subLineEdit);

                    label->setMinimumHeight(oneAliquotHeight);
                    subLineEdit->setMinimumHeight(oneAliquotHeight);
                    label->setMaximumHeight(oneAliquotHeight);
                    subLineEdit->setMaximumHeight(oneAliquotHeight);

                    QHBoxLayout* horizentalLayout = new QHBoxLayout(parameterContent_Frame);
                    horizentalLayout->setSpacing(0);
                    horizentalLayout->setContentsMargins(0, 0, 0, 0);
                    horizentalLayout->addWidget(label, 2);
                    horizentalLayout->addWidget(subLineEdit, 3);
                    parameterContentLayout->addLayout(horizentalLayout, row, column);
                    qDebug() << "4444444 row:" << row << " column:" << column;
                    if(column == 0)
                    {
                        column++;
                    }
                    else
                    {
                        remainderHeight = remainderHeight - oneAliquotHeight - margin;
                        row++;
                        column = 0;
                    }
                    m_vecIntInputLineEdit.append(subLineEdit);

                    connect(subLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(textChangedSlot(const QString &)));

                    qDebug() << " remainderHeight:" << remainderHeight;
                }
            }

            // double type Parameter
            {
                QHash<QString,double>::const_iterator it = m_settings.m_hashInputDoubleData.begin();
                for(it; it != m_settings.m_hashInputDoubleData.end(); ++it)
                {
                    QLabel* label = new QLabel(it.key() + ": ", parameterContent_Frame);
                    label->setFont(font);
                    InputLineEdit* subLineEdit = new InputLineEdit(QString::number(it.value()), parameterContent_Frame);
                    subLineEdit->setDataString(it.key());
                    double min, max, decimals;
                    _currentIndicator->getParameterRange(it.key(), min, max, decimals);
                    subLineEdit->setValidator(new QDoubleValidator(min, max, decimals, subLineEdit));
                    label->setBuddy(subLineEdit);

                    label->setMinimumHeight(oneAliquotHeight);
                    subLineEdit->setMinimumHeight(oneAliquotHeight);
                    label->setMaximumHeight(oneAliquotHeight);
                    subLineEdit->setMaximumHeight(oneAliquotHeight);

                    QHBoxLayout* horizentalLayout = new QHBoxLayout(parameterContent_Frame);
                    horizentalLayout->setSpacing(0);
                    horizentalLayout->setContentsMargins(0, 0, 0, 0);
                    horizentalLayout->addWidget(label, 2);
                    horizentalLayout->addWidget(subLineEdit, 3);
                    parameterContentLayout->addLayout(horizentalLayout, row, column);
                    qDebug() << "555555555 row:" << row << " column:" << column;
                    if(column == 0)
                    {
                        column++;
                    }
                    else
                    {
                        remainderHeight = remainderHeight - oneAliquotHeight - margin;
                        row++;
                        column = 0;
                    }
                    m_vecDoubleInputLineEdit.append(subLineEdit);

                    connect(subLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(textChangedSlot(const QString &)));

                    qDebug() << " remainderHeight:" << remainderHeight;
                }
            }

            // color，lineWidth,lineType
            if (m_settings.isDataValid())
            {
                const QString& strBackground = "border:1px solid #cfcfcf; border-radius:5px; background-color:";
                for (int i = 0; i < m_settings.m_plotNum; ++i)
                {
                    QString str = m_settings.m_vectLineSettings[i].m_lineName + ": ";
                    QLabel* label = new QLabel(str, parameterContent_Frame);
                    label->setFont(font);
                    QToolButton* colorButton = new QToolButton(parameterContent_Frame);
                    colorButton->setStyleSheet(strBackground + m_settings.m_vectLineSettings[i].m_lineColor.name());
                    connect(colorButton,SIGNAL(clicked()),this,SLOT(handleStandColorDialogSlot()));

                    label->setMinimumHeight(oneAliquotHeight);
                    colorButton->setMaximumSize(m_ui->IS_Parameter_Button->width(), oneAliquotHeight);
                    label->setMaximumHeight(oneAliquotHeight);
                    colorButton->setMaximumSize(m_ui->IS_Parameter_Button->width(), oneAliquotHeight);

                    QHBoxLayout* horizentalLayout = new QHBoxLayout(parameterContent_Frame);
                    horizentalLayout->setSpacing(0);
                    horizentalLayout->setContentsMargins(0, 0, 0, 0);
                    horizentalLayout->addWidget(label, 2);
                    horizentalLayout->addWidget(colorButton, 3);
                    parameterContentLayout->addLayout(horizentalLayout, row, column);
                    qDebug() << "6666666666 row:" << row << " column:" << column;
                    if(column == 0)
                    {
                        column++;
                    }
                    else
                    {
                        remainderHeight = remainderHeight - oneAliquotHeight - margin;
                        row++;
                        column = 0;
                    }

                    m_vecColorPushButton.push_back(colorButton);

                    qDebug() << " remainderHeight:" << remainderHeight;

                }
            }
        }
        if(column != 0) row++;

        QSpacerItem* spacerItem = new QSpacerItem(m_ui->IS_Parameter_Button->width(), remainderHeight, QSizePolicy::Fixed, QSizePolicy::Expanding);
        parameterContentLayout->addItem(spacerItem, row, 0, 1, 2);
        this->_isExistLayout = true;

        parameterContent_Frame->setLayout(parameterContentLayout);
        this->update();
    }


    void IndicatorSettingDialog::updateIndicatorProperty()
    {
        if(m_settings.m_type == enIndicatorMovingAverages)
        {
            QMapIterator<Indicator*, InputLineEdit*> intInputItem(m_MAvecIntInputLineEdit);
            while (intInputItem.hasNext()) {
              intInputItem.next();
              InputLineEdit* subLineEdit = m_MAvecIntInputLineEdit[intInputItem.key()];
              if(subLineEdit == NULL) continue;
              intInputItem.key()->m_settings.m_hashInputIntData[subLineEdit->getDataString()] = subLineEdit->text().toInt();
            }

            QMapIterator<Indicator*, InputLineEdit*> doubleInputItem(m_MAvecDoubleInputLineEdit);
            while (doubleInputItem.hasNext()) {
              doubleInputItem.next();
              InputLineEdit* subLineEdit = m_MAvecDoubleInputLineEdit[doubleInputItem.key()];
              if(subLineEdit == NULL) continue;
              doubleInputItem.key()->m_settings.m_hashInputDoubleData[subLineEdit->getDataString()] = subLineEdit->text().toDouble();
            }


            QMapIterator<Indicator*, QToolButton*> colorItem(m_MAvecColorPushButton);
            while (colorItem.hasNext()) {
              colorItem.next();
              if (colorItem.key()->m_settings.isDataValid())
              {
                  const QString& strBackground = "border:1px solid #cfcfcf; border-radius:5px; background-color:";
                  colorItem.key()->m_settings.m_vectLineSettings[0].m_lineColor = m_MAvecColorPushButton[colorItem.key()]->styleSheet().section(strBackground, 1).section(';', 0, 1).trimmed();
                  colorItem.key()->m_settings.m_vectLineSettings[0].m_lineWidth = 1.5;
              }
              colorItem.key()->m_settings.updateTitle();

              //refresh
              colorItem.key()->getCalculator()->beginCalculateAll(this->_currentIndicator);
              colorItem.key()->getCalculator()->addEndTask();   // always call addEndTask() to refresh UI.
            }


        }
        else
        {
            for(int i = 0; i < m_vecIntInputLineEdit.size(); ++i)
            {
                InputLineEdit* subLineEdit = m_vecIntInputLineEdit[i];
                if(subLineEdit == NULL) continue;
                m_settings.m_hashInputIntData[subLineEdit->getDataString()] = subLineEdit->text().toInt();
            }

            for(int i = 0; i < m_vecDoubleInputLineEdit.size(); ++i)
            {
                InputLineEdit* subLineEdit = m_vecDoubleInputLineEdit[i];
                if(subLineEdit == NULL) continue;
                m_settings.m_hashInputDoubleData[subLineEdit->getDataString()] = subLineEdit->text().toDouble();
            }

            if (m_settings.isDataValid() &&
                m_vecColorPushButton.size() == m_settings.m_plotNum)
            {
                const QString& strBackground = "background-color:";
                for (int i = 0; i < m_settings.m_plotNum; ++i)
                {
                    m_settings.m_vectLineSettings[i].m_lineColor = m_vecColorPushButton[i]->styleSheet().section(strBackground, 1).section(';', 0, 1).trimmed();
                    m_settings.m_vectLineSettings[i].m_lineWidth = 1.5;
                }
            }
            m_settings.updateTitle();
            this->_currentIndicator->setIndicatorSettings(m_settings);

            this->refreshIndicator();
        }
#if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
        this->_pView->ChangeMainIndicatorInfoSlot();
        this->_pView->ChangeSecondaryIndicatorInfoSlot();
#endif
    }

    void IndicatorSettingDialog::refreshIndicator()
    {
        this->_currentIndicator->getCalculator()->beginCalculateAll(this->_currentIndicator);
        this->_currentIndicator->getCalculator()->addEndTask();   // always call addEndTask() to refresh UI.
    }

}

#include "colorcomboxwidget.h"
#include "ui_chartcombox.h"
#include "SR.h"
#include <QDebug>
#include <QScroller>
#include <QApplication>
#include <QScreen>

namespace HistoricalChartSpace
{
    ColorComboxWidget::ColorComboxWidget(bool horizontalScreen, double pixelSizeRatio,
                               QString defaultKeyName,
                               QWidget *parent) : QDialog(parent),
                               m_ui(new Ui::ChartCombox)
    {
        this->setObjectName("ColorComboxWidget");
        this->setStyleSheet("#ColorComboxWidget{ background-color:rgba(0,0,0,150); }");

        QScreen *screen = QApplication::primaryScreen();
        this->setFixedSize(screen->size().width(), screen->size().height());
        m_ui->setupUi(this);

        int width = 0;
        int height = 0;
        #if defined (RUNTIME_OS_IPHONE)
            width = screen->size().width() - 40;
            height = screen->size().height() - (horizontalScreen ? 40 : 100);
            m_ui->CB_Content_GroupBox->setGeometry(20, (horizontalScreen ? 20 : 30), width, height);
        #else
             width = screen->size().width() - 100;
             height = screen->size().height() - (horizontalScreen ? (45 * pixelSizeRatio + 10) : 140);
             m_ui->CB_Content_GroupBox->setGeometry(50, (horizontalScreen ? 20 : 70), width, height);
        #endif

        int bottonHeight = 40 * pixelSizeRatio;
        int listWidgetHeight = height - 5 - bottonHeight -10;
        int TopY = 0;
        m_ui->CB_line->move(0, TopY);
        m_ui->CB_line->setFixedSize(width, 5);
        TopY = TopY + 5;

        m_ui->CB_Content_Widget->move(0, TopY);
        m_ui->CB_Content_Widget->setFixedSize(width, listWidgetHeight);

        TopY = TopY + listWidgetHeight + 5;
        m_ui->CB_OK_Button->move(0, TopY);
        m_ui->CB_OK_Button->setFixedSize((width -5)/2, bottonHeight);

        m_ui->CB_Close_Button->move((width -5)/2 + 5, TopY);
        m_ui->CB_Close_Button->setFixedSize((width -5)/2, bottonHeight);

        m_ui->CB_OK_Button->setText(SR::get("OK"));
        m_ui->CB_Close_Button->setText(SR::get("Close"));

        int rowHeight = 40 * pixelSizeRatio;
        this->_colorView = new ColorTableView(m_ui->CB_Content_Widget);
        this->_colorView->move(0, 0);
        this->_colorView->setFixedSize(width, listWidgetHeight);
        int newRadioHeight = 32 * pixelSizeRatio;
        int outerRingMargin = (rowHeight - newRadioHeight)/2;
        int innerRingMargin = outerRingMargin*2;
        qDebug() << "rowHeight:" << rowHeight;
#if defined (RUNTIME_OS_IPHONE)
        newRadioHeight = 26 * pixelSizeRatio;
        outerRingMargin = (rowHeight - newRadioHeight)/2;
        innerRingMargin = 8 * pixelSizeRatio;
#endif
        this->_colorView->setCustomModel(defaultKeyName, rowHeight, outerRingMargin, innerRingMargin, rowHeight, width/2-rowHeight, width/2);

        connect(m_ui->CB_OK_Button, SIGNAL(clicked()), this, SLOT(okSlot()));
        connect(m_ui->CB_Close_Button, SIGNAL(clicked()), this, SLOT(cancelSlot()));
    }


    void ColorComboxWidget::showDialog()
    {
        this->show();
        this->raise();
        this->activateWindow();
    #if defined (RUNTIME_OS_IPHONE)
        this->move(0, 0);
    #endif
    }

    void ColorComboxWidget::okSlot()
    {
        this->accepted();
        emit selectChangeSignal(this->_colorView->getCurrentColorName());
    }

    void ColorComboxWidget::cancelSlot()
    {
        this->rejected();
        emit cancelSignal();
    }
}

#include "MobileComboxWidget.h"
#include "Indicator/IndicatorCore.h"
#include <QToolBar>
#include <QMenu>
#include <QDebug>
#include <QVBoxLayout>
#include "SR.h"

namespace HistoricalChartSpace
{
    MobileComboxWidget::MobileComboxWidget(int buttonheight, int height, int width, QMap<int, int> dataSourceIndex, QMap<int, QString> dataSource, QWidget *parent) : QWidget(parent)
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->setSpacing(0);
        mainLayout->setMargin(0);

        QWidget* buttonWidget = new QWidget();
        buttonWidget->setMinimumSize(width, buttonheight);
        buttonWidget->setStyleSheet("background-color: rgb(79, 79, 79);");
        QHBoxLayout *layout = new QHBoxLayout();
        layout->setSpacing(0);
        layout->setMargin(0);
        this->m_okButton = new QPushButton();
        this->m_okButton->setText(SR::get("OK"));
        this->m_cancelButton = new QPushButton(SR::get("Cancel"));
        this->m_cancelButton->setText(SR::get("Cancel"));
        this->m_okButton->setMinimumSize(width/4.5, buttonheight-3);
        this->m_cancelButton->setMinimumSize(width/4.5, buttonheight-3);
        this->m_okButton->setStyleSheet("QPushButton{border:0px; border-width: 0px;color: #ffffff;font-size:16pt;}QPushButton::pressed{border:0px; border-width: 0px;}QPushButton::hover{border:0px; border-width: 0px;}");
        this->m_cancelButton->setStyleSheet("QPushButton{border:0px;border-width: 0px;color: #ffffff;font-size:16pt;}QPushButton::pressed{border:0px; border-width: 0px;}QPushButton::hover{border:0px; border-width: 0px;}");
        layout->addWidget(this->m_okButton);
        layout->addWidget(this->m_cancelButton);
        layout->addSpacerItem(new QSpacerItem(20, buttonheight, QSizePolicy::Expanding));
        buttonWidget->setLayout(layout);

        mainLayout->addWidget(buttonWidget, 1, Qt::AlignCenter);
        this->m_rollingbox = new RollingBox(height-buttonheight, width);
        this->m_rollingbox->setDataSource(dataSourceIndex, dataSource);
        mainLayout->addWidget(this->m_rollingbox, 6, Qt::AlignCenter);
        this->setLayout(mainLayout);


        connect(this->m_okButton, SIGNAL(clicked()), this, SLOT(OkSlot()));
        connect(this->m_cancelButton, SIGNAL(clicked()), this, SLOT(CancelSlot()));
    }

    void MobileComboxWidget::OkSlot()
    {
        int index = this->m_rollingbox->readValue();
        emit SelectChangeSignal(index);
    }

    void MobileComboxWidget::CancelSlot()
    {
        delete this->m_rollingbox;
        emit CancelSignal();
    }

}

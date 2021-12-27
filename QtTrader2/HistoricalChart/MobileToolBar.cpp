#include "MobileToolBar.h"
#include "SR.h"
#include "historicalchart.h"
#include <QLayout>
#include <QLayoutItem>
#include <QMetaObject>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include <QWidgetAction>
#include "MobileWidgetAction.h"


namespace HistoricalChartSpace
{
    MobileToolBar::MobileToolBar(bool isBlackTheme, bool orientation, QWidget *parent)
        : QWidget(parent), m_isBlackTheme(isBlackTheme),  m_orientation(orientation)
    {
        this->setStyleSheet("background-color: #ffffff;");
	}

    MobileToolBar::~MobileToolBar()
	{
	}

    void MobileToolBar::createMobileToolBar(const QList<Frequency>& frequencies)
    {
        QHBoxLayout *layout = new QHBoxLayout;
        layout->setSpacing(0);
        layout->setMargin(0);
        layout->setContentsMargins(0,0,0,0);
        QScreen *screen = QGuiApplication::primaryScreen();
        this->m_screenWidth = screen->size().width();
        this->m_screenHeight = screen->size().height();
        int index  = 0;
        if(this->m_orientation)
        {
            this->m_Frequencies = frequencies;
            this->m_frequencyButton = new MenuPushButton(index, MenuFrequency, this);
            this->m_frequencyButton->setText(SR::get("Frequency"));
            layout->addWidget(this->m_frequencyButton, 0, Qt::AlignHCenter);
        }
        else
        {
            foreach(Frequency frequency, frequencies)
            {
                MenuPushButton *button = new MenuPushButton(index, MenuFrequency, this);
                button->setText(SR::get(frequency.getName()));
                m_frequencyMenuPushButtons.append(button);
                layout->addWidget(button, 0, Qt::AlignHCenter);
                QObject::connect(button, SIGNAL(clicked()), this, SLOT(ClickFrequencySlot()));
               index++;
            }
        }
        this->m_graphTypeButton = new MenuPushButton(index, MenuGraphType, this);
        this->m_graphTypeButton->setText(SR::get("GraphType"));
        layout->addWidget(this->m_graphTypeButton, 0, Qt::AlignHCenter);

        this->m_indicatorButton = new MenuPushButton(index, MenuIndicator, this);
        this->m_indicatorButton->setText(SR::get("Indicator"));
        layout->addWidget(this->m_indicatorButton, 0, Qt::AlignHCenter);


        int averageHeight = (this->m_orientation ? 35 : 30) * (screen->logicalDotsPerInch()/72);
        int averageWidth = this->m_screenWidth/(m_frequencyMenuPushButtons.count()+ 2 + (this->m_orientation ? 1 : 0));
        for(int i=0; i< m_frequencyMenuPushButtons.count(); i++)
        {
            m_frequencyMenuPushButtons[i]->setMinimumSize(averageWidth, averageHeight);
        }
        if(this->m_orientation)
        {
            this->m_frequencyButton->setMinimumSize(averageWidth, averageHeight);
            QObject::connect(this->m_frequencyButton, SIGNAL(clicked()), this, SLOT(FrequencyClickSlot()));
        }
        this->m_graphTypeButton->setMinimumSize(averageWidth, averageHeight);
        this->m_indicatorButton->setMinimumSize(averageWidth, averageHeight);
        this->setLayout(layout);
        QObject::connect(this->m_graphTypeButton, SIGNAL(clicked()), this, SLOT(GraphTypeClickSlot()));
        QObject::connect(this->m_indicatorButton, SIGNAL(clicked()), this, SLOT(IndicatorClickSlot()));
    }

    void MobileToolBar::ClickFrequencySlot()
    {
        MenuPushButton *button = (MenuPushButton*)sender();
        emit SetFrequencySignal(button->getIndex());
    }

    void MobileToolBar::FrequencyClickSlot()
    {
        this->m_frequencyMenu = new QMenu((QWidget *)this->parent());
        this->m_frequencyMenu->setStyleSheet(" QMenu { background-color: white; border: 1px solid white;}QMenu::item {background-color: transparent;border-bottom:1px solid #DBDBDB;margin:4px 4px;} QMenu::item:selected { background-color: #2dabf9;}");

        QMap<int, QString> dataSource;
        QMap<int, int> dataSourceIndex;
        for(int index=0; index < this->m_Frequencies.count(); index ++)
        {
            Frequency frequency = this->m_Frequencies[index];
            dataSourceIndex.insert(index, index);
            dataSource.insert(index, SR::get(frequency.getName()));
        }

        MobileWidgetAction* mpActionList = new MobileWidgetAction(this->height(), this->height()*10, this->m_screenWidth, dataSourceIndex, dataSource, this->m_frequencyMenu);
        connect(mpActionList , SIGNAL(CheckSignal(int)), this, SLOT(CheckFrequencyMenuSlot(int)));
        connect(mpActionList , SIGNAL(CancelSignal()), this, SLOT(FrequencyMenuCancelSlot()));
        this->m_frequencyMenu->addAction(mpActionList);

        auto pos = this->m_frequencyButton->mapToGlobal(this->m_frequencyButton->rect().topRight());
        pos.setY(this->m_screenHeight - this->m_frequencyButton->height()*10);
        pos.setX(0);
        this->m_frequencyMenu->exec(pos);
        delete this->m_frequencyMenu;
    }

    void MobileToolBar::GraphTypeClickSlot()
    {
        this->m_graphTypeMenu = new QMenu((QWidget *)this->parent());
        this->m_graphTypeMenu->setStyleSheet(" QMenu { background-color: white; border: 1px solid white;}QMenu::item {background-color: transparent;border-bottom:1px solid #DBDBDB;margin:4px 4px;} QMenu::item:selected { background-color: #2dabf9;}");

        QMap<int, QString> dataSource;
        QMap<int, int> dataSourceIndex;
        dataSourceIndex.insert(0, 0);
        dataSource.insert(0, SR::get("Bar"));
        dataSourceIndex.insert(1, 1);
        dataSource.insert(1, SR::get("Line"));
        dataSourceIndex.insert(2, 2);
        dataSource.insert(2, SR::get("Candlestick"));

        MobileWidgetAction* mpActionList = new MobileWidgetAction(this->height(), this->height()*10, this->m_screenWidth, dataSourceIndex, dataSource, this->m_graphTypeMenu);
        connect(mpActionList , SIGNAL(CheckSignal(int)), this, SLOT(CheckGraphTypeMenuSlot(int)));
        connect(mpActionList , SIGNAL(CancelSignal()), this, SLOT(GraphTypeMenuCancelSlot()));
        this->m_graphTypeMenu->addAction(mpActionList);

        auto pos = this->m_graphTypeButton->mapToGlobal(this->m_graphTypeButton->rect().topRight());
        pos.setY(this->m_screenHeight - this->m_graphTypeButton->height()*10);
        pos.setX(0);
        this->m_graphTypeMenu->exec(pos);
        delete this->m_graphTypeMenu;
    }

    void MobileToolBar::IndicatorClickSlot()
    {
        this->m_indicatormenu = new QMenu((QWidget *)this->parent());
        this->m_indicatormenu->setStyleSheet(" QMenu { background-color: white; border: 1px solid white;}QMenu::item {background-color: transparent;} QMenu::item:selected { background-color: #2dabf9;}QToolBar{background-color: transparent;} ");
        this->m_indicatormenu->setFixedSize(this->m_screenWidth, this->height()*10);

        QMap<int, QString> dataSource;
        QMap<int, int> dataSourceIndex;
        int index = 0;
        for (int i = 0; i < enIndicatorTrendEnd; ++i)
        {
            dataSourceIndex.insert(index, i);
            dataSource.insert(i, IndicatorCore::getIndicatorName((enIndicatorType)i));
            index++;
        }
        for (int i = enIndicatorTrendEnd + 1; i < enIndicatorOscillatorsEnd; ++i)
        {
            dataSourceIndex.insert(index, i);
            dataSource.insert(i, IndicatorCore::getIndicatorName((enIndicatorType)i));
            index++;
        }
        for (int i = enIndicatorVolumesEnd + 1; i < enIndicatorEnd; ++i)
        {
            dataSourceIndex.insert(index, i);
            dataSource.insert(i, IndicatorCore::getIndicatorName((enIndicatorType)i));
            index++;
        }

        MobileWidgetAction* mpActionList = new MobileWidgetAction(this->height(), this->height()*10, this->m_screenWidth, dataSourceIndex, dataSource, this->m_indicatormenu);
        connect(mpActionList , SIGNAL(CheckSignal(int)), this, SLOT(CheckIndicatorMenuSlot(int)));
        connect(mpActionList , SIGNAL(CancelSignal()), this, SLOT(IndicatorMenuCancelSlot()));
        this->m_indicatormenu->addAction(mpActionList);

        auto pos = this->m_indicatorButton->mapToGlobal(this->m_indicatorButton->rect().topRight());
        pos.setY(this->m_screenHeight - this->m_indicatorButton->height()*10);
        pos.setX(0);
        this->m_indicatormenu->exec(pos);
        delete this->m_indicatormenu;
    }

    void MobileToolBar::CheckIndicatorMenuSlot(int index)
    {
        emit SetIndicatorSignal(index);
        this->m_indicatormenu->close();
    }

    void MobileToolBar::IndicatorMenuCancelSlot()
    {
        this->m_indicatormenu->close();
    }

    void MobileToolBar::CheckGraphTypeMenuSlot(int index)
    {
        emit SetPriceChartSignal(index);
        this->m_graphTypeMenu->close();
    }

    void MobileToolBar::GraphTypeMenuCancelSlot()
    {
        this->m_graphTypeMenu->close();
    }

    void MobileToolBar::CheckFrequencyMenuSlot(int index)
    {
        emit SetFrequencySignal(index);
        this->m_frequencyMenu->close();
    }

    void MobileToolBar::FrequencyMenuCancelSlot()
    {
        this->m_frequencyMenu->close();
    }
}

#include "MobileWidgetAction.h"
#include "Indicator/IndicatorCore.h"
#include <QToolBar>
#include <QMenu>
#include <QDebug>

namespace HistoricalChartSpace
{
    MobileWidgetAction::MobileWidgetAction(int buttonheight, int height, int width, QMap<int, int> dataSourceIndex, QMap<int, QString> dataSource, QWidget *parent) : QWidgetAction(parent)
    {
        this->m_mobileComboxWidget = new MobileComboxWidget(buttonheight, height, width, dataSourceIndex, dataSource);
        connect(this->m_mobileComboxWidget, SIGNAL(SelectChangeSignal(int)), this, SLOT(SelectChangeSlot(int)));
        connect(this->m_mobileComboxWidget, SIGNAL(CancelSignal()), this, SLOT(CancelSlot()));
    }

    QWidget * MobileWidgetAction::createWidget(QWidget *parent)
    {
        this->m_mobileComboxWidget->setParent(parent);
        return this->m_mobileComboxWidget;
    }

    void MobileWidgetAction::SelectChangeSlot(int index)
    {
        emit CheckSignal(index);
        delete this->m_mobileComboxWidget;
    }

    void MobileWidgetAction::CancelSlot()
    {
        emit CancelSignal();
        delete this->m_mobileComboxWidget;
    }

}

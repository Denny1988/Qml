#ifndef MOBILEWIDGETACTION_H
#define MOBILEWIDGETACTION_H
#include <QDialog>
#include <QObject>
#include <QWidgetAction>
#include <QMap>
#include "MobileComboxWidget.h"


namespace HistoricalChartSpace
{
    class MobileWidgetAction : public QWidgetAction
    {
        Q_OBJECT
    public:
        MobileWidgetAction(int buttonheight, int height, int width, QMap<int, int> dataSourceIndex, QMap<int, QString> dataSource, QWidget *parent = Q_NULLPTR);

    private:
        MobileComboxWidget* m_mobileComboxWidget;

    protected:
        QWidget * createWidget(QWidget *parent);

    signals:
        void CheckSignal(int index);
        void CancelSignal();

    public slots:
        void SelectChangeSlot(int index);
        void CancelSlot();

    };
}
#endif // MOBILEWIDGETACTION_H

#ifndef MOBILECOMBOXWIDGET_H
#define MOBILECOMBOXWIDGET_H
#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QMap>
#include "rollingbox.h"
#include <QPushButton>


namespace HistoricalChartSpace
{
    class MobileComboxWidget : public QWidget
    {
        Q_OBJECT
    public:
        MobileComboxWidget(int buttonheight, int height, int width, QMap<int, int> dataSourceIndex, QMap<int, QString> dataSource, QWidget *parent = Q_NULLPTR);

    private:
        RollingBox* m_rollingbox;
        QPushButton* m_okButton;
        QPushButton* m_cancelButton;

    signals:
        void SelectChangeSignal(int index);
        void CancelSignal();

     public slots:
        void OkSlot();
        void CancelSlot();

    };
}
#endif // MOBILECOMBOXWIDGET_H

#ifndef COLORCOMBOXWIDGET_H
#define COLORCOMBOXWIDGET_H

#include <QWidget>
#include <QMap>
#include <QVariant>
#include <QDialog>
#include "ColorSelectionView/colortableview.h"

namespace Ui {
    class ChartCombox;
}
namespace HistoricalChartSpace
{
    class ColorComboxWidget : public QDialog
    {
        Q_OBJECT
    public:
        explicit ColorComboxWidget(bool horizontalScreen, double pixelSizeRatio,
                              QString defaultKeyName,
                              QWidget *parent = nullptr);
        void showDialog();

    signals:
        void selectChangeSignal(QString showValue);
        void cancelSignal();

    public slots:
        //void selectChangeSlot(QVariant keyValue, QString showValue);
        void okSlot();
        void cancelSlot();

    private:
        Ui::ChartCombox* m_ui;
        ColorTableView* _colorView;
    };
}
#endif // COMBOXWIDGET_H

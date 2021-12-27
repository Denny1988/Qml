#ifndef COLORTABLEVIEW_H
#define COLORTABLEVIEW_H
#include "qtcustomwidget_global.h"
#include <QTableView>
#include <QScroller>
#include <QModelIndex>

class ColorModel;
class ColorDelegate;
class RadioDelegate;
class QTCUSTOMWIDGET_EXPORT ColorTableView : public QTableView
{
    Q_OBJECT
public:
    ColorTableView(QWidget *parent = Q_NULLPTR);
    ~ColorTableView();
    void setCustomModel(QString defaultColorName, int rowHeight, int outerRingMargin, int innerRingMargin,
                        int column1Width, int column2Width, int column3Width);
    QString getCurrentColorName();

public slots:
    void mouseClickSlot(const QModelIndex & index);

private:
    QScrollerProperties _scrPrt;
    QScroller* _scroller;
    ColorModel* _model;
    ColorDelegate* _colorDelegate;
    RadioDelegate* _radioDelegate;
    QModelIndex _index;
};

#endif // COLORTABLEVIEW_H

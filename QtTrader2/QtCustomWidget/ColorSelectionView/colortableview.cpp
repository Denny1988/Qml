#include "colortableview.h"
#include "radiodelegate.h"
#include "colordelegate.h"
#include "colormodel.h"
#include <QHeaderView>
#include <QScroller>
#include <QColor>

ColorTableView::ColorTableView(QWidget *parent) : QTableView(parent)
{
    this->setObjectName("ColorTableView");
    this->verticalHeader()->setVisible(false);
    this->horizontalHeader()->setVisible(false);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setContentsMargins(0, 0, 0, 0);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setDragEnabled(false);
    this->setDragDropMode(QAbstractItemView::NoDragDrop);
    this->setFocusPolicy(Qt::NoFocus);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setMouseTracking(true);
    this->setShowGrid(false);
    this->setStyleSheet("QTableView#ColorTableView {border:0px;color:black;background-color: #e7e7e7;} "
                        "QTableView#ColorTableView::item {border: 1px solid #c2c2c2;color:black;border-left-width: 0px;border-top-width: 0px;border-right-width: 0px;}");


     this->_scroller = QScroller::scroller(this);
     this->_scroller->grabGesture(this, QScroller::LeftMouseButtonGesture);
     this->_scrPrt = _scroller->scrollerProperties();
     //0.1=100/1000  总共1000个像素，只允许拖拽100个像素
     this->_scrPrt.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor,0.1);
     this->_scroller->setScrollerProperties(this->_scrPrt);
}

ColorTableView::~ColorTableView()
{
    delete _colorDelegate;
    delete _radioDelegate;
    delete _model;
}

void ColorTableView::setCustomModel(QString defaultColorName, int rowHeight, int outerRingMargin, int innerRingMargin, int column1Width, int column2Width, int column3Width)
{
    QMap<QVariant, QString> dataSource;
    QStringList colorNameList = QColor::colorNames();
    int i = 0;
    QVariant defaultKey = QVariant(0);
    bool isNoColorName = defaultColorName.contains("#");
    foreach(QString colorName, colorNameList)
    {
        if(isNoColorName)
        {
            if(QColor(colorName).name() == defaultColorName)
                defaultKey = QVariant(i);
        }
        else
        {
            if(colorName == defaultColorName)
                defaultKey = QVariant(i);
        }
        dataSource.insert(i, colorName);
        i++;
    }

    this->_model = new ColorModel;
    this->_model->setMap(dataSource, defaultKey);
    this->setModel(this->_model);

    QMap<int, QString> itemColors;
    QMap<int, bool> itemChecks;
    QMapIterator<QVariant, QString> iter(dataSource);
    while (iter.hasNext()) {
        iter.next();
        itemChecks.insert(iter.key().toInt(), iter.key() == defaultKey);
        itemColors.insert(iter.key().toInt(), iter.value());
        this->setRowHeight(iter.key().toInt(), rowHeight);
    }
    this->setColumnWidth(0, column1Width);
    this->setColumnWidth(1, column2Width);
    this->setColumnWidth(2, column3Width);

    QModelIndex index = this->_model->index(defaultKey.toInt(), 2, QModelIndex());
    this->setCurrentIndex(index);

    this->_colorDelegate = new ColorDelegate(itemColors, this);
    this->_radioDelegate = new RadioDelegate(outerRingMargin, innerRingMargin, itemChecks, this);
    connect(this->_colorDelegate, SIGNAL(mouseClickSignal(const QModelIndex&)), this, SLOT(mouseClickSlot(const QModelIndex&)));
    connect(this->_radioDelegate, SIGNAL(mouseClickSignal(const QModelIndex&)), this, SLOT(mouseClickSlot(const QModelIndex&)));
    this->setItemDelegateForColumn(0, this->_radioDelegate);
    this->setItemDelegateForColumn(1, this->_colorDelegate);
    connect(this, SIGNAL(clicked(const QModelIndex &)), this, SLOT(mouseClickSlot(const QModelIndex &)));
}

QString ColorTableView::getCurrentColorName()
{
    return this->_model->getCurrentColorName(this->_index.row());
}

void ColorTableView::mouseClickSlot(const QModelIndex & index)
{
    this->_index = index;
    this->_radioDelegate->mouseClickSlot(index);
    this->update();
}

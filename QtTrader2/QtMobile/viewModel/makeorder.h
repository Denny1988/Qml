#ifndef MAKEORDER_H
#define MAKEORDER_H

#include <QObject>

class MakeOrder: public QObject
{
    Q_OBJECT
public:
    MakeOrder(QObject *parent = nullptr);
    ~MakeOrder();
private :
    QString _instrument ="";



public slots:
    QString getInstrument();
    void setInstrument(QString ins);
};

#endif // MAKEORDER_H

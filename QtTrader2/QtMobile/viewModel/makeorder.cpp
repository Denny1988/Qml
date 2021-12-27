#include "makeorder.h"

MakeOrder::MakeOrder(QObject *parent) : QObject(parent)
{

}
MakeOrder::~MakeOrder(){

}


QString MakeOrder::getInstrument()
{
    return this->_instrument;
}

 void MakeOrder::setInstrument(QString ins)
 {
    this->_instrument = ins;
 }

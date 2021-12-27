#include "instrument.h"

Instrument::Instrument(QObject *parent) : QObject(parent)
{
    m_id = QUuid::createUuid();
    m_code = "TestCode0";
}

Instrument::~Instrument()
{

}

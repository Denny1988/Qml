#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <QObject>
#include <QUuid>

class Instrument : public QObject
{
    Q_OBJECT
public:
    Instrument(QObject *parent = nullptr);
    ~Instrument();
    QString m_code;
    QUuid m_id;

signals:

public slots:
};

#endif // INSTRUMENT_H

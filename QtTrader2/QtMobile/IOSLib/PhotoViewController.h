#ifndef PHOTOVIEWCONTROLLER_H
#define PHOTOVIEWCONTROLLER_H

#include <QObject>
#include <QImage>

class PhotoViewController: public QObject
{
    Q_OBJECT

public:
    static PhotoViewController *g_currentImagePicker;

public slots:
    void show(void);

signals:
    void imageSelected(const QImage image);
    void cancelSignal();
};

#endif//PHOTOVIEWCONTROLLER_H

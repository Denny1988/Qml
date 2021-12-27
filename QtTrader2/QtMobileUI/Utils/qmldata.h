#ifndef QMLDATA_H
#define QMLDATA_H
#include <QObject>

class QmlData: public QObject
{
    Q_OBJECT

public:
    QmlData(QObject *parent = nullptr);
    ~QmlData();
    Q_INVOKABLE int getViewX();
    Q_INVOKABLE int getViewY();
    Q_INVOKABLE int getQmlWidth();
    Q_INVOKABLE int getQmlHeight();
    Q_INVOKABLE void getCloseQmlPage();
    Q_INVOKABLE QString getTitle();
    Q_INVOKABLE int getTitleBarHeight();
    Q_INVOKABLE QString getTitleBarBgColor();
    Q_INVOKABLE QString getTitleBarColor();
    Q_INVOKABLE int getTitleBackHeight();
    Q_INVOKABLE int getTitleBackTop();
    Q_INVOKABLE QString getTitleBackPicPath();
    Q_INVOKABLE QString getTitleBackText();
    Q_INVOKABLE int getTitleBarPixelSize();
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged);

    QString url() const;
    void setUrl(QString url);
    void setPosition(int x, int y);
    void setTitle(QString value);

signals:
    void dataChanged();
    void urlChanged();
    void closeQmlPage();
    void getDeliveryTimeSignal(QString jsonStr);

public slots:
    void loadFinsh();
    void jscallme(QString jsonStr);

private:
    QString m_url;
    int m_view_x;
    int m_view_y;
    QString m_title;

};

#endif // QMLDATA_H

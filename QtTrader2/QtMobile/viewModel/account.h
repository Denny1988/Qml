#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <QObject>

class Account : public QObject
{
    Q_OBJECT
    Q_ENUMS( AlertLevel)

public:
    Account(QObject *parent = nullptr);
    ~Account();
    Q_INVOKABLE double getBanlance();
    Q_PROPERTY(bool isLogined READ isLogined)
    Q_PROPERTY(AlertLevel alertLevel READ alertLevel NOTIFY alertLevelChanged)
    Q_PROPERTY(double banlance READ getBanlance NOTIFY banlanceChanged)
    enum AlertLevel { Level_0, Level_1, Level_2, Level_3};

    AlertLevel alertLevel() const;
    bool isLogined() const;

signals:
    void loginStateChanged(bool isSuccess);
    void alertLevelChanged(AlertLevel alertLevel);
    void banlanceChanged();

public slots:
    void startLogin(QString loginName, QString password);

private:

    QString m_loginName;

    QString m_password;

    bool m_loginSuccess;

    QString m_name;

    QString m_code;

    double m_banlance;

    AlertLevel m_alertLevel;
};

#endif // ACCOUNT_H

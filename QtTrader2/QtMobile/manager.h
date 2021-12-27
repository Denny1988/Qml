#ifndef MANAGER_H
#define MANAGER_H

#include <QList>

#include "traderapp.h"
#include "viewModel/account.h"
#include "viewModel/instrument.h"
#include "Common/Result/Result.h"
#include <QSharedPointer>
#include <QObject>

class Manager : public QObject
{
    Q_OBJECT
public:
    static Manager* getInstance();
    static void deleteInstance();
    void init();
    void clear();

    Account* getAccount();

private:
    Manager();
    static Manager* m_manager;
    Account* m_account;
    QList<Instrument*> m_instrumentList;
    QSharedPointer<Common::Result::LoginResult> m_LoginResult;

signals:
    void closeSignal();
    void startGetInitData();

public slots:
    void loginResultCompletedSlot();


};

#endif // MANAGER_H

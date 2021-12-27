#include "account.h"
#include "log.h"


Account::Account(QObject *parent) : QObject(parent)
{
    this->m_loginName = "Test01";
    this->m_password = "12345678";
    this->m_loginSuccess = false;
    this->m_banlance = 50000.0 ;
    this->m_alertLevel = Level_0;
    //this->data = data;
}

Account::~Account()
{

}

void Account::startLogin(QString loginName, QString password)
{
    Log::d("startLogin");
    this->m_loginName = loginName;
    this->m_password = password;
    this->m_loginSuccess = (password == "12345678");
    emit loginStateChanged(this->m_loginSuccess);
}

double Account::getBanlance()
{
    return this->m_banlance;
}

bool Account::isLogined() const
{
    return this->m_loginSuccess;
}

Account::AlertLevel Account::alertLevel() const
{
    return m_alertLevel;
}

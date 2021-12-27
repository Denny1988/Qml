#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QDialog>
#include <QTimer>
#include "Common/downloadmanager.h"
#include <QMovie>
#include <QLabel>

namespace Ui {
    class SplashScreen;
}

enum SplashState
{
    None = 0,
    LoadSuccess = 1,
    LoadFailed = 2,
    Timeout = 3,
    Finish = 100,
};

class MerchantDialog;
class SplashScreen : public QDialog
{
    Q_OBJECT

public:
    explicit SplashScreen(QWidget *parent = nullptr);
    ~SplashScreen();
    void finish();    
    void start();

signals:
    void startSignal();
    void animationSignal();
    void finishSignal(); // SplashScreen finish
    void startEnableGroupUserDownSignal();

public slots:
    void startSlot();
    void quitSlot();
    void showMerchantSlot();
    void skipClickedSlot();
    void checkMerchantStateSlot(bool needSetMerchant, bool needUpdateResouce);
    void setMerchantSlot(QString merchantCode);
    void startEnableGroupUserDownSlot();
    void enableGroupUserDownSlot(bool isSuccess, QString fileName);


private slots:
    void executeUpdateSlot();//running back, 相关文件更新，后台执行
    void executeAnimationSlot();//
    void settingMerchantFinishSlot();

private:
    bool m_isUpdated;
    MerchantDialog* m_merchantPage;
    bool m_isNewEnterMerchant;
    QTimer* m_enableGroupUserDownTimer;
    int m_enableGroupUserDownfailedCount;
    bool m_endProcessBar;
    QMovie* m_movie;
    QLabel* m_gif;
    bool m_isUseFirstLine;

private:
    void showMerchantSettingDialog();
    void startEnableGroupUserDown(bool isUseFirstLine = true);
};

#endif // SPLASHSCREEN_H

#include "autoupdatedialog.h"
#include "ui_autoupdatewidget.h"
#include "Common/LoginSetting.h"
#include "stdafx.h"

AutoUpdateDialog::AutoUpdateDialog(QString currentVersion, QString newVersion, bool isNeedDirectUpdate, QWidget *parent) : QWidget(parent), m_ui(new Ui::AutoUpdateWidget)
{    
    int width = g_ViewInfo->getScreenWidth();
    this->setFixedSize(width, g_ViewInfo->getScreenHeight());
    m_ui->setupUi(this);
    this->setObjectName("AutoUpdateWidget");
    this->setStyleSheet("#AutoUpdateWidget { border-image: url(:/Resources/Images/backgroud.png);} ");

    int buttonHeight = g_ViewInfo->getButtonHeight();
    int cmargin = buttonHeight/2;
    int vmargin = buttonHeight/3;
    int contentHeight = 2*cmargin + 3*buttonHeight + vmargin;
    m_ui->Content_Frame->move(20, (this->height() - contentHeight)/2);
    m_ui->Content_Frame->setFixedSize(width-40, contentHeight);
    m_ui->Content_Layout->setContentsMargins(cmargin, cmargin, cmargin, cmargin);
    m_ui->Content_Layout->setSpacing(vmargin);

    m_ui->versionUpdatePrompt_Label->setFixedHeight(2*buttonHeight);
    QString currentLanguage = Common::LoginSetting::getInstance()->getLanguage();
    if(currentLanguage == "CHS")
    {
        QString info = QString("发现版本: %1, 当前版本: %2\r\n是否更新？").arg(newVersion, currentVersion);
        m_ui->versionUpdatePrompt_Label->setText(info);
        m_ui->Update_Button->setText("立即更新");
        m_ui->NotUpdate_Button->setText("暂不更新");
    }
    else if(currentLanguage == "CHT")
    {
        QString info = QString("發現版本: %1, 當前版本: %2\r\n是否更新？").arg(newVersion, currentVersion);
        m_ui->versionUpdatePrompt_Label->setText(info);
        m_ui->Update_Button->setText("立即更新");
        m_ui->NotUpdate_Button->setText("暫不更新");
    }
    else
    {
        QString info = QString("Found Version: %1, Current Version: %2 Update or Not?").arg(newVersion, currentVersion);
        m_ui->versionUpdatePrompt_Label->setText(info);
        m_ui->Update_Button->setText("Update now");
        m_ui->NotUpdate_Button->setText("Next Time");
    }

    if(isNeedDirectUpdate)
    {
        m_ui->NotUpdate_Button->setVisible(false);
        m_ui->Update_Button->setFixedSize((width-40 - 2*cmargin), buttonHeight);
    }
    else
    {
        int bWidth = (width-40 - 2*cmargin - 10)/2;
        m_ui->Update_Button->setFixedSize(bWidth, buttonHeight);
        m_ui->NotUpdate_Button->setFixedSize(bWidth, buttonHeight);
    }
    connect(m_ui->Update_Button, SIGNAL(clicked()), this, SLOT(updateClickSlot()));
    connect(m_ui->NotUpdate_Button, SIGNAL(clicked()), this, SLOT(notUpdateClickSlot()));
}

void AutoUpdateDialog::updateClickSlot()
{
    emit startUpdateSignal();
}

void AutoUpdateDialog::notUpdateClickSlot()
{
    emit closeSignal();
}

#include "merchantdialog.h"
#include "ui_merchantwidget.h"
#include "Common/LoginSetting.h"
#include "stdafx.h"

MerchantDialog::MerchantDialog(bool isShowClose, QWidget *parent) : QWidget(parent), m_ui(new Ui::MerchantWidget)
{
    int width = g_ViewInfo->getScreenWidth();
    this->setFixedSize(width, g_ViewInfo->getScreenHeight());
    m_ui->setupUi(this);
    //this->setStyleSheet("#MerchantWidget { border-image: url(:/Resources/Images/backgroud.png);} ");
    int buttonHeight = g_ViewInfo->getButtonHeight();
    int cmargin = buttonHeight/2;
    int vmargin = buttonHeight/3;
    int contentHeight = 2*cmargin + 2*buttonHeight + vmargin;
    m_ui->MW_Content_Frame->move(20, (this->height() - contentHeight)/2);
    m_ui->MW_Content_Frame->setFixedSize(width-40, contentHeight);
    m_ui->Content_Layout->setContentsMargins(cmargin, cmargin, cmargin, cmargin);
    m_ui->Content_Layout->setSpacing(vmargin);
    if(isShowClose)
    {
        m_ui->MW_Close_Button->setFixedHeight(g_ViewInfo->getButtonHeight());
        connect(m_ui->MW_Close_Button, SIGNAL(clicked()), this, SIGNAL(closeSignal()));
        int itemWidth = (width-40 - 2*cmargin -5)/2;
        m_ui->MW_Close_Button->setFixedWidth(itemWidth);
        m_ui->MW_Ok_Button->setFixedWidth(itemWidth);
    }
    else
    {
        m_ui->MW_Close_Button->setVisible(false);
        m_ui->MW_Ok_Button->setFixedWidth(width-40 - 2*cmargin);
    }
    //m_ui->MW_Ok_Button->setFixedHeight(buttonHeight);
    //m_ui->MW_Code_LineEdit->setFixedHeight(buttonHeight);
    connect(m_ui->MW_Ok_Button, SIGNAL(clicked()), this, SLOT(okClickSlot()));

    QString currentLanguage = Common::LoginSetting::getInstance()->getLanguage();
    if(currentLanguage == "CHS")
    {
        m_ui->MW_Code_Label->setText("请输入组号编号：");
        m_ui->MW_Ok_Button->setText("确定");
        m_ui->MW_Close_Button->setText("取消");
    }
    else if(currentLanguage == "CHT")
    {
        m_ui->MW_Code_Label->setText("請輸入組號編號：");
        m_ui->MW_Ok_Button->setText("確定");
        m_ui->MW_Close_Button->setText("取消");
    }
    else
    {
        m_ui->MW_Code_Label->setText("Please enter Group ID:");
        m_ui->MW_Ok_Button->setText("OK");
        m_ui->MW_Close_Button->setText("Cancel");
    }
}

void MerchantDialog::okClickSlot()
{
    if(m_ui->MW_Code_LineEdit->text().length() > 0)
    {
        emit setMerchantSignal(m_ui->MW_Code_LineEdit->text());
    }
}

bool MerchantDialog::event(QEvent *event)
{
    if (event->type() == QEvent::ShortcutOverride)
    {
        this->okClickSlot();
        return false;
    }
    return QWidget::event(event);
}

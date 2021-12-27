#include "stdafx.h"
#include <QApplication>
#include <QScreen>
#include <QDebug>
#include <QDesktopWidget>
#include "QsLog.h"
#include "Common/Constants/LogKeyConstants.h"
#include "Common/TraderBaseInfo.h"

ViewInfo* ViewInfo::_instance=Q_NULLPTR;
ViewInfo* ViewInfo::getInstance()
{
    if (Q_NULLPTR==_instance)
    {
        _instance = new ViewInfo;
    }
    return _instance;
}

ViewInfo::ViewInfo()
{
    QScreen *screen = QApplication::primaryScreen();
    this->_screenHeight = screen->availableGeometry().height();
    this->_screenWidth = screen->availableGeometry().width();
    int logicalDotsPerInch = static_cast<int>(screen->logicalDotsPerInch());
    this->_pixelSizeRatio = logicalDotsPerInch / 72;
    if(this->_screenHeight > 1600 && this->_pixelSizeRatio < 3)
        this->_pixelSizeRatio = 3;
    else if(this->_screenHeight > 1300 && this->_pixelSizeRatio < 2)
        this->_pixelSizeRatio = 2;
    this->_headerTitleMenuHeight = 40 * this->_pixelSizeRatio;
    this->_footerTabMenuHeight = 45 * this->_pixelSizeRatio;
    this->_mainPageContentHeight = this->_screenHeight - this->_headerTitleMenuHeight - this->_footerTabMenuHeight - ((this->_headerTitleMenuHeight/3)*2);
    this->_childPageContentHeight = this->_screenHeight - this->_headerTitleMenuHeight;
    this->_defaultPixelSize = (16 * logicalDotsPerInch) / 72;
#if defined(RUNTIME_OS_ANDROID)
    this->_menuIconHeight = 25*this->_pixelSizeRatio;
#else
    this->_menuIconHeight = 22*this->_pixelSizeRatio;
#endif
    this->_buttonHeight = 40*this->_pixelSizeRatio;
    this->_listViewRowHeight = 40*this->_pixelSizeRatio;
    this->_accountListViewRowHeight = 30*this->_pixelSizeRatio;
    this->_marginHeight = 6*this->_pixelSizeRatio;
    this->_accountFloatButtonHeight = 40 * this->_pixelSizeRatio;
    this->_toolButtonIcoHeight = 32 * this->_pixelSizeRatio;
    this->_fullScreenHeight = screen->virtualSize().height();
    this->_checkBoxIndicatorHeight = 18*this->_pixelSizeRatio;

    qDebug() << " screen virtualSize height:" << screen->virtualSize().height()
             << " screen virtualSize width:" << screen->virtualSize().width()
             << " screen available height:" << screen->availableGeometry().height()
             << " screen available width:" << screen->availableGeometry().width()
             << "screen height:" << this->_screenHeight
             << "screen width:" << this->_screenWidth
             << " headerTitleMenuHeight:" << this->_headerTitleMenuHeight
             << " footerTabMenuHeight:" << this->_footerTabMenuHeight
             << " mainPageContentHeight:" << this->_mainPageContentHeight
             << " childPageContentHeight:" << this->_childPageContentHeight
             << " _defaultPixelSize:" << this->_defaultPixelSize
             << " _pixelSizeRatio:" << this->_pixelSizeRatio
             << " _menuIconHeight:" << this->_menuIconHeight
             << " _buttonHeight:" << this->_buttonHeight
             << " _listViewRowHeight:" << this->_listViewRowHeight
             << " devicePixelRatio:" << screen->devicePixelRatio()
             << " logicalDotsPerInch:" << screen->logicalDotsPerInch()
             << " physicalDotsPerInch:" << screen->physicalDotsPerInch()
             << " availablevirtualSize height:" << screen->availableVirtualSize().height()
             << " availablevirtualSize width:" << screen->availableVirtualSize().width()
             << " physicalSize height:" << screen->physicalSize().height()
             << " physicalSize width:" << screen->physicalSize().width()
             << " physicalDotsPerInchX:" << screen->physicalDotsPerInchX()
             << " physicalDotsPerInchY:" << screen->physicalDotsPerInchY()
             << " logicalDotsPerInchX:" << screen->logicalDotsPerInchX()
             << " logicalDotsPerInchY:" << screen->logicalDotsPerInchY();
}

ViewInfo::~ViewInfo()
{

}

void ViewInfo::printParameterLog()
{
    QScreen *screen = QApplication::primaryScreen();
    QLOG_INFO(Common::Constants::LogKeyConstants::Application)
            << " screen virtualSize height:" << screen->virtualSize().height()
             << " screen virtualSize width:" << screen->virtualSize().width()
             << " screen available height:" << screen->availableGeometry().height()
             << " screen available width:" << screen->availableGeometry().width()
             << "screen height:" << this->_screenHeight
             << "screen width:" << this->_screenWidth
             << " headerTitleMenuHeight:" << this->_headerTitleMenuHeight
             << " footerTabMenuHeight:" << this->_footerTabMenuHeight
             << " mainPageContentHeight:" << this->_mainPageContentHeight
             << " childPageContentHeight:" << this->_childPageContentHeight
             << " _defaultPixelSize:" << this->_defaultPixelSize
             << " _pixelSizeRatio:" << this->_pixelSizeRatio
             << " _menuIconHeight:" << this->_menuIconHeight
             << " _buttonHeight:" << this->_buttonHeight
             << " _listViewRowHeight:" << this->_listViewRowHeight
             << " devicePixelRatio:" << screen->devicePixelRatio()
             << " logicalDotsPerInch:" << screen->logicalDotsPerInch()
             << " physicalDotsPerInch:" << screen->physicalDotsPerInch()
             << " availablevirtualSize height:" << screen->availableVirtualSize().height()
             << " availablevirtualSize width:" << screen->availableVirtualSize().width()
             << " physicalSize height:" << screen->physicalSize().height()
             << " physicalSize width:" << screen->physicalSize().width();
}


int ViewInfo::getDefaultPixelSize(){
    int pixelSize = this->_defaultPixelSize;
    if(Common::TraderBaseInfo::getInstance()->getLanguage() == "ENG")
        pixelSize = static_cast<int>(pixelSize* 0.85);
//    qDebug() << " pixelSize:" << pixelSize
//             << " defaultPixelSize:" << this->_defaultPixelSize;
    return pixelSize;
}


int ViewInfo::getScrollBarWidth()
{
#if defined (RUNTIME_OS_ANDROID)
    return 8;
#else
    return 4;
#endif
}

int ViewInfo::getSmallPixelSize()
{
    return static_cast<int>(this->_defaultPixelSize* 0.85);
}


QString ViewInfo::toString()
{
    QScreen *screen = QApplication::primaryScreen();
    int logicalDotsPerInch = static_cast<int>(screen->logicalDotsPerInch());
    QString info = QString("screen virtualSize height:");
    info.append(QString::number(screen->virtualSize().height()));
    info.append(" screen virtualSize width:");
    info.append(QString::number(screen->virtualSize().width()));
    info.append(" screen available height:");
    info.append(QString::number(screen->availableGeometry().height()));
    info.append(" screen available width:");
    info.append(QString::number(screen->availableGeometry().width()));
    info.append(" screen height:");
    info.append(QString::number(this->_screenHeight));
    info.append(" screen width:");
    info.append(QString::number(this->_screenWidth));
    info.append(" headerTitleMenuHeight:");
    info.append(QString::number(this->_headerTitleMenuHeight));
    info.append(" footerTabMenuHeight:");
    info.append(QString::number(this->_footerTabMenuHeight));
    info.append(" mainPageContentHeight:");
    info.append(QString::number(this->_mainPageContentHeight));
    info.append(" childPageContentHeight:");
    info.append(QString::number(this->_childPageContentHeight));
    info.append(" _defaultPixelSize:");
    info.append(QString::number(this->_defaultPixelSize));
    info.append(" _pixelSizeRatio:");
    info.append(QString::number(this->_pixelSizeRatio));
    info.append(" _menuIconHeight:");
    info.append(QString::number(this->_menuIconHeight));
    info.append(" _buttonHeight:");
    info.append(QString::number(this->_buttonHeight));
    info.append(" _listViewRowHeight:");
    info.append(QString::number(this->_listViewRowHeight));
    info.append(" devicePixelRatio:");
    info.append(QString::number(screen->devicePixelRatio()));
    info.append(" logicalDotsPerInch:");
    info.append(QString::number(screen->logicalDotsPerInch()));
    info.append(" physicalDotsPerInch:");
    info.append(QString::number(screen->physicalDotsPerInch()));
    info.append(" availablevirtualSize height:");
    info.append(QString::number(screen->availableVirtualSize().height()));
    info.append(" availablevirtualSize width:");
    info.append(QString::number(screen->availableVirtualSize().width()));
    info.append(" physicalSize height:");
    info.append(QString::number(screen->physicalSize().height()));
    info.append(" physicalSize width: ");
    info.append(QString::number(screen->physicalSize().width()));
    info.append(" logicalDotsPerInch:");
    info.append(QString::number(logicalDotsPerInch));
    info.append(" physicalDotsPerInch:");
    info.append(QString::number(screen->physicalDotsPerInch()));



    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    double devicePixelRatio = QGuiApplication::primaryScreen()->devicePixelRatio();//设备无关像素值与像素的单位比值
    int screenW = screenRect.width();//设备无关像素值宽度。在手机设备上，这里得到的是设备无关像素值(如：在1080x1920像素分辨率的手机上，这里得到的宽高可能类似于393x698这样的设备无关像素值，宽度为393)
    int screenH = screenRect.height();//设备无关像素值高度
    int screenResolutionWidth = int(screenW*devicePixelRatio);//手机屏幕真正的像素分辨率宽度
    QSizeF physicalSize = QGuiApplication::primaryScreen()->physicalSize();//手机屏幕物理尺寸，单位:毫米
    double physicalScreenWidthInch = double(physicalSize.width())/10*0.3937008;//手机屏幕英寸宽度
    int dpi = int(screenResolutionWidth/physicalScreenWidthInch);//像素宽度除以英寸宽度=像素密度
    info.append(" dpi:");
    info.append(QString::number(dpi));
    return info;
}

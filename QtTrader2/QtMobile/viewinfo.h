#ifndef VIEW_VIEWINFO_H
#define VIEW_VIEWINFO_H
#include <QString>
#include <QFont>
#include <QObject>

class ViewInfo : public QObject
{
    Q_OBJECT
    public:
        ViewInfo();
        ~ViewInfo();
        void printParameterLog();
        Q_INVOKABLE int getDefaultPixelSize();
        Q_INVOKABLE int getScrollBarWidth();
        Q_INVOKABLE int getSmallPixelSize();
        QString toString();

    public:
        static ViewInfo*	getInstance();
        Q_INVOKABLE int getScreenWidth(){
            return this->_screenWidth;
        }
        Q_INVOKABLE int getScreenHeight(){
            return this->_screenHeight;
        }
        Q_INVOKABLE int getFullScreenHeight(){
            return this->_fullScreenHeight;
        }
        Q_INVOKABLE int getHeaderTitleMenuHeight(){
            return this->_headerTitleMenuHeight;
        }
        Q_INVOKABLE int getFooterTabMenuHeight(){
            return this->_footerTabMenuHeight;
        }
        Q_INVOKABLE int getMainPageContentHeight(){
            return this->_mainPageContentHeight;
        }
        Q_INVOKABLE int getChildPageContentHeight(){
            return this->_childPageContentHeight;
        }

        Q_INVOKABLE int getOriginalDefaultPixelSize()
        {
             return this->_defaultPixelSize;
        }

        Q_INVOKABLE int getPixelSizeRatio(){
            return this->_pixelSizeRatio;
        }
        Q_INVOKABLE int getMenuIconHeight(){
            return this->_menuIconHeight;
        }

        Q_INVOKABLE int getButtonHeight(){
            return this->_buttonHeight;
        }

        Q_INVOKABLE int getListViewRowHeight(){
            return this->_listViewRowHeight;
        }

        Q_INVOKABLE int getAccountListViewRowHeight(){
            return this->_accountListViewRowHeight;
        }

        Q_INVOKABLE int getMarginHeight(){
            return this->_marginHeight;
        }

        Q_INVOKABLE int getAccountFloatButtonHeight(){
            return this->_accountFloatButtonHeight;
        }

        Q_INVOKABLE int getToolButtonIcoHeight(){
            return this->_toolButtonIcoHeight;
        }

        Q_INVOKABLE int getCheckBoxIndicatorHeight()
        {
            return this->_checkBoxIndicatorHeight;
        }

        Q_INVOKABLE int getSystemBarHeight()
        {
            return this->_fullScreenHeight - this->_screenHeight;
        }
    
        Q_INVOKABLE int getiPhoneXBottomSystemBarHeight()
        {
            return 34;
        }

        Q_INVOKABLE QFont getSplashScreenFont()
        {
            return _splashScreenFont;
        }

        Q_INVOKABLE void setSplashScreenFont(QFont value)
        {
            _splashScreenFont = value;
        }

    private:
        static	ViewInfo*	_instance;
        int _screenWidth;
        int _screenHeight;
        int _fullScreenHeight;
        int _headerTitleMenuHeight;
        int _footerTabMenuHeight;
        int _mainPageContentHeight;
        int _childPageContentHeight;
        int _defaultPixelSize;
        int _pixelSizeRatio;
        int _menuIconHeight;
        int _buttonHeight;
        int _listViewRowHeight;
        int _accountListViewRowHeight;
        int _marginHeight;
        int _accountFloatButtonHeight;
        int _toolButtonIcoHeight;
        int _checkBoxIndicatorHeight;
        QFont _splashScreenFont;
};
#endif	  

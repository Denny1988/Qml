#ifndef VIEW_VIEWINFO_H
#define VIEW_VIEWINFO_H
#include <QString>
#include <QFont>

class ViewInfo
{
    public:
        ViewInfo();
        ~ViewInfo();
        void printParameterLog();
        int getDefaultPixelSize();
        int getScrollBarWidth();
        int getSmallPixelSize();
        QString toString();

    public:
        static ViewInfo*	getInstance();
        int getScreenWidth(){
            return this->_screenWidth;
        }
        int getScreenHeight(){
            return this->_screenHeight;
        }
        int getFullScreenHeight(){
            return this->_fullScreenHeight;
        }
        int getHeaderTitleMenuHeight(){
            return this->_headerTitleMenuHeight;
        }
        int getFooterTabMenuHeight(){
            return this->_footerTabMenuHeight;
        }
        int getMainPageContentHeight(){
            return this->_mainPageContentHeight;
        }
        int getChildPageContentHeight(){
            return this->_childPageContentHeight;
        }

        int getOriginalDefaultPixelSize()
        {
             return this->_defaultPixelSize;
        }

        int getPixelSizeRatio(){
            return this->_pixelSizeRatio;
        }
        int getMenuIconHeight(){
            return this->_menuIconHeight;
        }

        int getButtonHeight(){
            return this->_buttonHeight;
        }

        int getListViewRowHeight(){
            return this->_listViewRowHeight;
        }

        int getAccountListViewRowHeight(){
            return this->_accountListViewRowHeight;
        }

        int getMarginHeight(){
            return this->_marginHeight;
        }

        int getAccountFloatButtonHeight(){
            return this->_accountFloatButtonHeight;
        }

        int getToolButtonIcoHeight(){
            return this->_toolButtonIcoHeight;
        }

        int getCheckBoxIndicatorHeight()
        {
            return this->_checkBoxIndicatorHeight;
        }

        int getSystemBarHeight()
        {
            return this->_fullScreenHeight - this->_screenHeight;
        }
    
        int getiPhoneXBottomSystemBarHeight()
        {
            return 34;
        }

        QFont getSplashScreenFont()
        {
            return _splashScreenFont;
        }

        void setSplashScreenFont(QFont value)
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

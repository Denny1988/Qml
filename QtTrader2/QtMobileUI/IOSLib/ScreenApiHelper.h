#ifndef SCREENAPIHELPER_H
#define SCREENAPIHELPER_H

class ScreenApiHelper {

    public:
    ScreenApiHelper();
    ~ScreenApiHelper();

    void setVerticalHorizontalScreenChart(bool isHorizontal);
    void changeStatusBarStyle(bool isBlackFont);
    bool getIsSleepStatus();
    const char * getAppversionName();
    const char * getSerialNumber();
    const char * getDeviceModel();
    const char * getOSType();
    const char * getOSLanguage();
    const char * getOSVersionCode();
    bool getIsIPhoneX();
    void openUrl(const char * c_Url);
    void copyTxtToSysytemClipboard(const char * c_Content);
    void playNotifySound(const char * c_SoundType, const char * c_SoundPath);
};
#endif

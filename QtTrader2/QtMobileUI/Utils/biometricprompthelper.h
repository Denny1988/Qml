#ifndef UTILS_BIOMETRICPROMPTHELPER_H
#define UTILS_BIOMETRICPROMPTHELPER_H
#include <QString>

class BiometricPromptHelper
{
public:
    BiometricPromptHelper();
    static QString openFingerLogin();
    static QString fingerLogin();
    static QString checkHardware();
    static int getOpenFingerLoginStatus();
    static int getFingerLoginStatus();
    static void resetOpenFingerLoginStatus();
    static void resetFingerLoginStatus();
    static QString getBiometricPromptIV();
    static void setBiometricPromptIV(QString iv);

    static QString getFingerLoginStatusStr(int status);
};

#endif // UTILS_WXHELPER_H

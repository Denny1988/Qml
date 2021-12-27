#ifndef BIOMETRICPROMPTAPIHELPER_H
#define BIOMETRICPROMPTAPIHELPER_H
#include <QString>

class BiometricPromptApiHelper {

public:
    BiometricPromptApiHelper();
    ~BiometricPromptApiHelper();

    int getFingerLoginStatus();
    void fingerLogin();
    void resetFingerLoginStatus();



};
#endif

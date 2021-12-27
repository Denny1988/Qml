#import "BiometricPromptApiHelper.h"
#import "BiometricPromptController.h"
#include <QDebug>

BiometricPromptController* _myView;
BiometricPromptApiHelper::BiometricPromptApiHelper()
{
    _myView = [[BiometricPromptController alloc] init];
}

BiometricPromptApiHelper::~BiometricPromptApiHelper()
{
    NSLog(@"BiometricPromptApiHelper::~BiometricPromptApiHelper");
    [_myView release];
    _myView = nil;
}

int BiometricPromptApiHelper::getFingerLoginStatus()
{    
    int status = [_myView getFingerLoginStatus];
    qDebug() << "BiometricPromptApiHelper::getFingerLoginStatus :" << status;
    return status;
}

void BiometricPromptApiHelper::fingerLogin()
{
    [_myView fingerLogin];
}

void BiometricPromptApiHelper::resetFingerLoginStatus()
{
    [_myView resetFingerLoginStatus];
}


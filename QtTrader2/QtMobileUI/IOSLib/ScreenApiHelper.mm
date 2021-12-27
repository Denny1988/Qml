#import "ScreenApiHelper.h"
#import "ScreenController.h"
#import "ScreenLockStateController.h"
#import <WXApiManager.h>

ScreenLockStateController* _myScreenLockStateView;
ScreenApiHelper::ScreenApiHelper()
{
    _myScreenLockStateView = [[ScreenLockStateController alloc] init];
}

ScreenApiHelper::~ScreenApiHelper()
{
    NSLog(@"ScreenApiHelper::~ScreenApiHelper");
    [_myScreenLockStateView release];
    _myScreenLockStateView = nil;
}

void ScreenApiHelper::setVerticalHorizontalScreenChart(bool isHorizontal)
{
    [ScreenController setVerticalHorizontalScreenChart:isHorizontal];
}

void ScreenApiHelper::changeStatusBarStyle(bool isBlackFont)
{
    [ScreenController changeStatusBarStyle:isBlackFont];
}

bool ScreenApiHelper::getIsSleepStatus()
{
    BOOL status = [_myScreenLockStateView getIsSleepStatus];
    return status;
}

const char * ScreenApiHelper::getAppversionName()
{
    NSDictionary *infoDictionary = [[NSBundle mainBundle] infoDictionary];
    CFShow(infoDictionary);
    NSString *app_Version = [infoDictionary objectForKey:@"CFBundleShortVersionString"];
    if(app_Version != nil)
    {
        return [app_Version UTF8String];
    }
    return nil;
}

const char * ScreenApiHelper::getSerialNumber()
{
    NSString *serialNumber = [[UIDevice currentDevice] identifierForVendor].UUIDString;
    if(serialNumber != nil)
    {
        return [serialNumber UTF8String];
    }
    return nil;
}

const char * ScreenApiHelper::getDeviceModel()
{
    NSString *deviceModel = [[UIDevice currentDevice] model];
    NSLog(@"deviceModel--------- : %@", deviceModel);
    if(deviceModel != nil)
    {
        return [deviceModel UTF8String];
    }
    return nil;
}

const char * ScreenApiHelper::getOSType()
{
    NSString* systemName = [ScreenController deviceVersion];
    NSLog(@"languageCode : %@", systemName);
    //NSString *systemName = [UIDevice currentDevice].systemName;
    if(systemName != nil)
    {
        return [systemName UTF8String];
    }
    return nil;
}

const char * ScreenApiHelper::getOSLanguage()
{
    NSString *languageCode = [NSLocale preferredLanguages][0];// 返回的也是国际通用语言Code+国际通用国家地区代码
    NSString *countryCode = [NSString stringWithFormat:@"-%@", [[NSLocale currentLocale] objectForKey:NSLocaleCountryCode]];
    if (languageCode) {
        languageCode = [languageCode stringByReplacingOccurrencesOfString:countryCode withString:@""];
    }
    NSLog(@"languageCode : %@", languageCode);
    if(languageCode != nil)
    {
        return [languageCode UTF8String];
    }
    return nil;
}

const char * ScreenApiHelper::getOSVersionCode()
{
    NSString *systemName = [UIDevice currentDevice].systemVersion;
    if(systemName != nil)
    {
        return [systemName UTF8String];
    }
    return nil;
}

bool ScreenApiHelper::getIsIPhoneX()
{
    BOOL isIPhoneX = [ScreenController getIsIPhoneX];
    return isIPhoneX;
}

void ScreenApiHelper::openUrl(const char * c_Url)
{
    [ScreenController openUrl:c_Url];
}

void ScreenApiHelper::copyTxtToSysytemClipboard(const char * c_Content)
{
    [ScreenController copyTxtToSysytemClipboard:c_Content];
}

void ScreenApiHelper::playNotifySound(const char * c_SoundType, const char * c_SoundPath)
{
    [ScreenController playNotifySound:c_SoundType:c_SoundPath];
}

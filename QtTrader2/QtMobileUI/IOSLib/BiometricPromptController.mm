#import "BiometricPromptController.h"
#import <LocalAuthentication/LocalAuthentication.h>


#pragma mark - BiometricPromptController
@implementation BiometricPromptController
{

}

-(void)setFingerLoginStatus:(int)status
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setInteger:status forKey:@"fingerLoginStatus"];
    [defaults synchronize];
}

-(void)resetFingerLoginStatus
{
    [self setFingerLoginStatus:0];
}

-(int)getFingerLoginStatus
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSInteger fingerLoginStatus = [defaults integerForKey:@"fingerLoginStatus"];
    return (int)fingerLoginStatus;
}

-(void)fingerLogin
{
    if (NSFoundationVersionNumber < NSFoundationVersionNumber_iOS_8_0) {
        NSLog(@"系统版本不支持TouchID");
        [self setFingerLoginStatus:-1];
        return;
    }
    if (@available(iOS 11.0, *)) {
            // 利用safeAreaInsets.bottom > 0.0来判断是否是iPhone X以上设备。
            UIWindow * window = [[[UIApplication sharedApplication] delegate] window];
            if (window.safeAreaInsets.bottom > 0.0) {
               NSLog(@"iPhone X以上设备不支持TouchID");
               [self setFingerLoginStatus:-1];
               return;
            }
        }

        LAContext *myContext = [[LAContext alloc] init];
    // 这个属性是设置指纹输入失败之后的弹出框的选项
    //myContext.localizedFallbackTitle = @"忘记密码";

    NSError *authError = nil;
    NSString *myLocalizedReasonString = @"Please press and hold the Home button to complete the verification";
    NSUserDefaults* defs = [NSUserDefaults standardUserDefaults];
    NSArray* languages = [defs objectForKey:@"AppleLanguages"];
    NSString* language = [languages objectAtIndex:0];
    if ([language containsString:@"zh-Hans"])
        myLocalizedReasonString = @"请按住Home键完成验证";
    else if([language containsString:@"zh-Hant"])
        myLocalizedReasonString = @"請按住Home鍵完成驗證";
    // MARK: 判断设备是否支持指纹识别
    if ([myContext canEvaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics error:&authError])
    {
        [myContext evaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics localizedReason:myLocalizedReasonString reply:^(BOOL success, NSError * _Nullable error) {
            if(success)
            {
                NSLog(@"指纹认证成功");
                [self setFingerLoginStatus:1];
            }
            else
            {
                //weakSelf.helper.isAppCurrentLoginState = NO;
                NSLog(@"指纹认证失败，%@",error.description);

                NSLog(@"%ld", (long)error.code); // 错误码 error.code
                switch (error.code)
                {
                    case LAErrorAuthenticationFailed: // Authentication was not successful, because user failed to provide valid credentials
                    {
                        NSLog(@"授权失败"); // -2 连续三次指纹识别错误
                        [self setFingerLoginStatus:-2];
                    }
                    break;
                    case LAErrorUserCancel: // Authentication was canceled by user (e.g. tapped Cancel button)
                    {
                        NSLog(@"用户取消验证Touch ID"); // -3 在TouchID对话框中点击了取消按钮
                        [self setFingerLoginStatus:-3];
                    }
                    break;
                    case LAErrorUserFallback: // Authentication was canceled, because the user tapped the fallback button (Enter Password)
                    {
                        [[NSOperationQueue mainQueue] addOperationWithBlock:^{
                            NSLog(@"用户选择输入密码，切换主线程处理"); // -4 在TouchID对话框中点击了输入密码按钮
                        }];
                        [self setFingerLoginStatus:-4];
                    }
                    break;
                    case LAErrorSystemCancel: // Authentication was canceled by system (e.g. another application went to foreground)
                    {
                        NSLog(@"取消授权，如其他应用切入，用户自主"); // -5 TouchID对话框被系统取消，例如按下Home或者电源键
                        [self setFingerLoginStatus:-5];
                    }
                    break;
                    case LAErrorPasscodeNotSet: // Authentication could not start, because passcode is not set on the device.
                    {
                        NSLog(@"设备系统未设置密码"); // -6
                        [self setFingerLoginStatus:-6];
                    }
                    break;
                    case LAErrorTouchIDNotAvailable: // Authentication could not start, because Touch ID is not available on the device
                    {
                        NSLog(@"设备未设置Touch ID"); // -7
                        [self setFingerLoginStatus:-7];
                    }
                    break;
                    case LAErrorTouchIDNotEnrolled: // Authentication could not start, because Touch ID has no enrolled fingers
                    {
                        NSLog(@"用户未录入指纹"); // -8
                        [self setFingerLoginStatus:-8];
                    }
                    break;

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_9_0
                    case LAErrorTouchIDLockout: //Authentication was not successful, because there were too many failed Touch ID attempts and Touch ID is now locked. Passcode is required to unlock Touch ID, e.g. evaluating LAPolicyDeviceOwnerAuthenticationWithBiometrics will ask for passcode as a prerequisite 用户连续多次进行Touch ID验证失败，Touch ID被锁，需要用户输入密码解锁，先Touch ID验证密码
                    {
                        NSLog(@"Touch ID被锁，需要用户输入密码解锁"); // -9 连续五次指纹识别错误，TouchID功能被锁定，下一次需要输入系统密码
                        [self setFingerLoginStatus:-9];
                    }
                    break;
                    case LAErrorAppCancel: // Authentication was canceled by application (e.g. invalidate was called while authentication was in progress) 如突然来了电话，电话应用进入前台，APP被挂起啦");
                    {
                        NSLog(@"用户不能控制情况下APP被挂起"); // -10
                        [self setFingerLoginStatus:-10];
                    }
                    break;
                    case LAErrorInvalidContext: // LAContext passed to this call has been previously invalidated.
                    {
                        NSLog(@"LAContext传递给这个调用之前已经失效"); // -11
                        [self setFingerLoginStatus:-11];
                    }
                    break;
#else
#endif
                    default:
                    {
                        [[NSOperationQueue mainQueue] addOperationWithBlock:^{
                            NSLog(@"其他情况，切换主线程处理");
                        }];
                        [self setFingerLoginStatus:-12];
                        break;
                    }
                }
            }
        }];
    }
    else
    {
        NSLog(@"设备不支持指纹");
        NSLog(@"%ld", (long)authError.code);
        switch (authError.code)
        {
            case LAErrorTouchIDNotEnrolled:
            {
                NSLog(@"Authentication could not start, because Touch ID has no enrolled fingers");
                [self setFingerLoginStatus:-13];
                break;
            }
            case LAErrorPasscodeNotSet:
            {
                NSLog(@"Authentication could not start, because passcode is not set on the device");
                [self setFingerLoginStatus:-14];
                break;
            }
            default:
            {
                NSLog(@"TouchID not available");
                [self setFingerLoginStatus:-15];
                break;
            }
        }
    }
}
@end


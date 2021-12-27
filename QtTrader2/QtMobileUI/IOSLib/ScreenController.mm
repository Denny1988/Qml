#import "ScreenController.h"
#import <sys/utsname.h>
#import <AudioToolbox/AudioToolbox.h>

@interface ScreenController ()
@end

@implementation ScreenController
{

}

#pragma mark - ScreenController
+(void)setVerticalHorizontalScreenChart:(BOOL)isHorizontal
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setBool:isHorizontal forKey:@"IsLandscapeRight"];
    [defaults synchronize];
    if(isHorizontal)
    {
        //如果强制横屏,打开这段代码,并且将手机设备的竖排方向锁定打开即可
        if ([[UIDevice currentDevice]respondsToSelector:@selector(setOrientation:)]) {
            [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationNone];
            NSNumber *orientationTarget = [NSNumber numberWithInt:UIInterfaceOrientationLandscapeRight];
            [[UIDevice currentDevice] setValue:orientationTarget forKey:@"orientation"];
          //[self orientationToPortrait:UIInterfaceOrientationLandscapeRight];
        }
    }
    else
    {
        if ([[UIDevice currentDevice]respondsToSelector:@selector(setOrientation:)]) {
            [[UIApplication sharedApplication] setStatusBarHidden:NO withAnimation:UIStatusBarAnimationNone];
            NSNumber *orientationTarget = [NSNumber numberWithInt:UIInterfaceOrientationPortrait];
            [[UIDevice currentDevice] setValue:orientationTarget forKey:@"orientation"];
            //[self orientationToPortrait:UIInterfaceOrientationPortrait];
        }
    }
}

+(void)changeStatusBarStyle:(BOOL)isBlackFont
{
    [[UIApplication sharedApplication] setStatusBarHidden:NO withAnimation:UIStatusBarAnimationNone];
    if(isBlackFont)
    {
        [UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleDarkContent;
    }
    else
    {
        [UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleLightContent;
    }
}

+(void)orientationToPortrait:(UIInterfaceOrientation)orientation
{
    NSLog(@"NAV orientationToPortrait old:%ld",(long)[UIDevice currentDevice].orientation);
    NSLog(@"NAV orientationToPortrait new:%ld",(long)orientation);
    SEL selector = NSSelectorFromString(@"setOrientation:");
    NSInvocation * invocation = [NSInvocation invocationWithMethodSignature:[UIDevice instanceMethodSignatureForSelector:selector]];
    [invocation setSelector:selector];
    [invocation setTarget:[UIDevice currentDevice]];
    int val = orientation;
    [invocation setArgument:&val atIndex:2];
    [invocation invoke];

    
}

+(BOOL)getIsIPhoneX
{
    if (@available(iOS 11.0, *)) {
            UIWindow *window = [UIApplication sharedApplication].delegate.window;
            if (window.safeAreaInsets.bottom > 0.0) {
                // 是机型iPhoneX/iPhoneXR/iPhoneXS/iPhoneXSMax
                return true;
            }
    }
    return false;
}

+ (NSString*)deviceVersion
{
    struct utsname systemInfo;
    uname(&systemInfo);

    NSString *platform = [NSString stringWithCString:systemInfo.machine encoding:NSASCIIStringEncoding];

    if ([platform isEqualToString:@"iPhone5,1"]) return @"iPhone 5";

    if ([platform isEqualToString:@"iPhone5,2"]) return @"iPhone 5";

    if ([platform isEqualToString:@"iPhone5,3"]) return @"iPhone 5c";

    if ([platform isEqualToString:@"iPhone5,4"]) return @"iPhone 5c";

    if ([platform isEqualToString:@"iPhone6,1"]) return @"iPhone 5s";

    if ([platform isEqualToString:@"iPhone6,2"]) return @"iPhone 5s";

    if ([platform isEqualToString:@"iPhone7,1"]) return @"iPhone 6 Plus";

    if ([platform isEqualToString:@"iPhone7,2"]) return @"iPhone 6";

    if ([platform isEqualToString:@"iPhone8,1"]) return @"iPhone 6s";

    if ([platform isEqualToString:@"iPhone8,2"]) return @"iPhone 6s Plus";

    if ([platform isEqualToString:@"iPhone8,4"]) return @"iPhone SE";

    if ([platform isEqualToString:@"iPhone9,1"]) return @"iPhone 7";

    if ([platform isEqualToString:@"iPhone9,3"]) return @"iPhone 7";

    if ([platform isEqualToString:@"iPhone9,2"]) return @"iPhone 7 Plus";

    if ([platform isEqualToString:@"iPhone9,4"]) return @"iPhone 7 Plus";

    if ([platform isEqualToString:@"iPhone10,1"]) return @"iPhone 8";

    if ([platform isEqualToString:@"iPhone10,4"]) return @"iPhone 8";

    if ([platform isEqualToString:@"iPhone10,2"]) return @"iPhone 8 Plus";

    if ([platform isEqualToString:@"iPhone10,5"]) return @"iPhone 8 Plus";

    if ([platform isEqualToString:@"iPhone10,3"]) return @"iPhone X";

    if ([platform isEqualToString:@"iPhone10,6"]) return @"iPhone X";

    if ([platform isEqualToString:@"iPhone11,8"]) return @"iPhone XR";

    if ([platform isEqualToString:@"iPhone11,2"]) return @"iPhone XS";

    if ([platform isEqualToString:@"iPhone11,6"]) return @"iPhone XS Max";

    if ([platform isEqualToString:@"iPhone11,4"]) return @"iPhone XS Max";
    
    if ([platform isEqualToString:@"iPhone12,1"])   return @"iPhone 11";
    
    if ([platform isEqualToString:@"iPhone12,3"])   return @"iPhone 11 Pro";
    
    if ([platform isEqualToString:@"iPhone12,5"])   return @"iPhone 11 Pro Max";
    
    if ([platform isEqualToString:@"iPhone12,8"])   return @"iPhone SE (2nd generation)";
    
    if ([platform isEqualToString:@"iPhone13,1"])   return @"iPhone 12 mini";
    
    if ([platform isEqualToString:@"iPhone13,2"])   return @"iPhone 12";
    
    if ([platform isEqualToString:@"iPhone13,3"])   return @"iPhone 12 Pro";
    
    if ([platform isEqualToString:@"iPhone13,4"])   return @"iPhone 12 Pro Max";
    
    if ([platform isEqualToString:@"iPhone14,4"])   return @"iPhone 13 mini";
    
    if ([platform isEqualToString:@"iPhone14,5"])   return @"iPhone 13";
    
    if ([platform isEqualToString:@"iPhone14,2"])   return @"iPhone 13 Pro";
    
    if ([platform isEqualToString:@"iPhone14,3"])   return @"iPhone 13 Pro Max";
    
    if([platform isEqualToString:@"iPad1,1"])  return@"iPad 1G";

    if([platform isEqualToString:@"iPad2,1"])  return@"iPad 2";

    if([platform isEqualToString:@"iPad2,2"])  return@"iPad 2";

    if([platform isEqualToString:@"iPad2,3"])  return@"iPad 2";

    if([platform isEqualToString:@"iPad2,4"])  return@"iPad 2";

    if([platform isEqualToString:@"iPad2,5"])  return@"iPad Mini 1G";

    if([platform isEqualToString:@"iPad2,6"])  return@"iPad Mini 1G";

    if([platform isEqualToString:@"iPad2,7"])  return@"iPad Mini 1G";

    if([platform isEqualToString:@"iPad3,1"])  return@"iPad 3";

    if([platform isEqualToString:@"iPad3,2"])  return@"iPad 3";

    if([platform isEqualToString:@"iPad3,3"])  return@"iPad 3";

    if([platform isEqualToString:@"iPad3,4"])  return@"iPad 4";

    if([platform isEqualToString:@"iPad3,5"])  return@"iPad 4";

    if([platform isEqualToString:@"iPad3,6"])  return@"iPad 4";

    if([platform isEqualToString:@"iPad4,1"])  return@"iPad Air";

    if([platform isEqualToString:@"iPad4,2"])  return@"iPad Air";

    if([platform isEqualToString:@"iPad4,3"])  return@"iPad Air";

    if([platform isEqualToString:@"iPad4,4"])  return@"iPad Mini 2G";

    if([platform isEqualToString:@"iPad4,5"])  return@"iPad Mini 2G";

    if([platform isEqualToString:@"iPad4,6"])  return@"iPad Mini 2G";

    if([platform isEqualToString:@"iPad4,7"])  return@"iPad Mini 3";

    if([platform isEqualToString:@"iPad4,8"])  return@"iPad Mini 3";

    if([platform isEqualToString:@"iPad4,9"])  return@"iPad Mini 3";

    if([platform isEqualToString:@"iPad5,1"])  return@"iPad Mini 4";

    if([platform isEqualToString:@"iPad5,2"])  return@"iPad Mini 4";

    if([platform isEqualToString:@"iPad5,3"])  return@"iPad Air 2";

    if([platform isEqualToString:@"iPad5,4"])  return@"iPad Air 2";

    if([platform isEqualToString:@"iPad6,3"])  return@"iPad Pro (9.7-inch)";

    if([platform isEqualToString:@"iPad6,4"])  return@"iPad Pro (9.7-inch)";

    if([platform isEqualToString:@"iPad6,7"])  return@"iPad Pro (12.9-inch)";

    if([platform isEqualToString:@"iPad6,8"])  return@"iPad Pro (12.9-inch)";
    
    if([platform isEqualToString:@"iPad7,1"])  return@"iPad Pro (12.9-inch) (2nd generation)";
    
    if([platform isEqualToString:@"iPad7,2"])  return@"iPad Pro (12.9-inch) (2nd generation)";
    
    if([platform isEqualToString:@"iPad7,3"])  return@"iPad Pro (10.5-inch)";
    
    if([platform isEqualToString:@"iPad7,4"])  return@"iPad Pro (10.5-inch)";
    
    if([platform isEqualToString:@"iPad8,1"])  return@"iPad Pro (11-inch)";
    
    if([platform isEqualToString:@"iPad8,2"])  return@"iPad Pro (11-inch)";
    
    if([platform isEqualToString:@"iPad8,3"])  return@"iPad Pro (11-inch)";
    
    if([platform isEqualToString:@"iPad8,4"])  return@"iPad Pro (11-inch)";
    
    if([platform isEqualToString:@"iPad8,5"])  return@"iPad Pro (12.9-inch) (3rd generation)";
    
    if([platform isEqualToString:@"iPad8,6"])  return@"iPad Pro (12.9-inch) (3rd generation)";
    
    if([platform isEqualToString:@"iPad8,7"])  return@"iPad Pro (12.9-inch) (3rd generation)";
    
    if([platform isEqualToString:@"iPad8,8"])  return@"iPad Pro (12.9-inch) (3rd generation)";

		if([platform isEqualToString:@"iPad8,9"])  return@"iPad Pro (11-inch) (2nd generation)";

		if([platform isEqualToString:@"iPad8,10"])  return@"iPad Pro (11-inch) (2nd generation)";
		
		if([platform isEqualToString:@"iPad8,11"])  return@"iPad Pro (12.9-inch) (4th generation)";
		
		if([platform isEqualToString:@"iPad8,12"])  return@"iPad Pro (12.9-inch) (4th generation)";
		
		if([platform isEqualToString:@"iPad13,4"])  return@"iPad Pro (11-inch) (3rd generation)";
		
		if([platform isEqualToString:@"iPad13,5"])  return@"iPad Pro (11-inch) (3rd generation)";
		
		if([platform isEqualToString:@"iPad13,6"])  return@"iPad Pro (11-inch) (3rd generation)";
		
		if([platform isEqualToString:@"iPad13,7"])  return@"iPad Pro (11-inch) (3rd generation)";
		
		if([platform isEqualToString:@"iPad13,8"])  return@"iPad Pro (12.9-inch) (5th generation)";
		
		if([platform isEqualToString:@"iPad13,9"])  return@"iPad Pro (12.9-inch) (5th generation)";
		
		if([platform isEqualToString:@"iPad13,10"])  return@"iPad Pro (12.9-inch) (5th generation)";
		
		if([platform isEqualToString:@"iPad13,11"])  return@"iPad Pro (12.9-inch) (5th generation)";		

    if([platform isEqualToString:@"i386"])      return@"iPhone Simulator";

    if ([platform isEqualToString:@"x86_64"])    return @"iPhone Simulator";


    return platform;

}

+(void)openUrl:(const char *)c_Url
{
    NSStringEncoding encoding = NSUTF8StringEncoding;
    NSString *urlAddress= [[NSString alloc] initWithCString:c_Url encoding:encoding];
    NSLog(@"openUrl url: %@", urlAddress);
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:urlAddress]];
}

+(void)copyTxtToSysytemClipboard:(const char *)c_Content
{
    NSStringEncoding encoding = NSUTF8StringEncoding;
    NSString * content= [[NSString alloc] initWithCString:c_Content encoding:encoding];
    NSLog(@"openUrl url: %@", content);
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    pasteboard.string = content;
}


+(void)playNotifySound:(const char *)c_soundType:(const char *)c_soundPath
{
    NSStringEncoding encoding = NSUTF8StringEncoding;
    NSString * soundPath = [[NSString alloc] initWithCString:c_soundPath encoding:encoding];
    //判断路径是否存在
    if (soundPath) {
        NSLog(@"playNotifySound soundPath: %@", soundPath);
        SystemSoundID soundID = 1000;
        OSStatus error = AudioServicesCreateSystemSoundID((__bridge CFURLRef _Nonnull)([NSURL fileURLWithPath:soundPath]), &soundID);
        //判断是否有错误
        if (error != kAudioServicesNoError) {
            NSLog(@"playNotifySound error: %d",(int)error);
        }
        else{
            //只播放声音，没振动
            AudioServicesPlaySystemSound(soundID);
        }
    }
    else{
        NSString * soundType =[[NSString alloc]initWithFormat:@"Sound.bundle/%@%@",soundType, [[NSString alloc] initWithCString:c_soundType encoding:encoding]];
        NSString *path = [[NSBundle mainBundle] pathForResource:soundType ofType:nil];

        //定义一个带振动的SystemSoundID
        SystemSoundID soundID = 1000;
        //判断路径是否存在
        if (path) {
            //创建一个音频文件的播放系统声音服务器
            OSStatus error = AudioServicesCreateSystemSoundID((__bridge CFURLRef _Nonnull)([NSURL fileURLWithPath:path]), &soundID);
            //判断是否有错误
            if (error != kAudioServicesNoError) {
                NSLog(@"%d",(int)error);
            }
            else{
                //只播放声音，没振动
                AudioServicesPlaySystemSound(soundID);
            }
        }
    }
}


@end

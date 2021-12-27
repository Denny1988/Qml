#import "MyAppDelegate.h"
//#import "MainViewController.h"
#include <QWindow>

#include "WXApiManager.h"
#include "WXApi.h"

#import "MyViewController.h"

@interface QIOSApplicationDelegate ()

@end

@implementation QIOSApplicationDelegate

@synthesize window = _window;
@synthesize viewController = _viewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    NSLog(@"MyAppDelegate application:didFinishLaunchingWithOptions begin...");
    [UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleLightContent;
    
    BOOL result = YES;   
    
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    NSLog(@"self.window : %@", self.window);
    self.viewController = [[MyViewController alloc] init];
    NSLog(@"self.viewController : %@", self.viewController);
    [WXApiManager sharedManager].myViewController = self.viewController;
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
    
    [WXApi startLogByLevel:WXLogLevelNormal logBlock:^(NSString *log) {
        NSLog(@"log : %@", log);
   }];
    
    //向微信注册
    //[WXApi registerApp:@"wx1b38b9c7037a66ab" enableMTA:NO];
    //[WXApi registerApp:@"wx1b38b9c7037a66ab" universalLink:@"https://help.wechat.com"];
    //test
    [WXApi registerApp:@"wxd930ea5d5a258f4f" universalLink:@"https://help.wechat.com/sdksample/"];

    //向微信注册支持的文件类型
    //UInt64 typeFlag = MMAPP_SUPPORT_TEXT | MMAPP_SUPPORT_PICTURE | MMAPP_SUPPORT_WEBPAGE | MMAPP_SUPPORT_DOC;
    //[WXApi registerAppSupportContentFlag:typeFlag];
    
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setBool:false forKey:@"IsLandscapeRight"];
    [defaults synchronize];

    NSLog(@"application:didFinishLaunchingWithOptions end");
    return result;
}

- (UIInterfaceOrientationMask)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    BOOL isLandscapeRight = [defaults integerForKey:@"IsLandscapeRight"];
    if(isLandscapeRight){
        return UIInterfaceOrientationMaskAll;
    }
    else
    {
        return UIInterfaceOrientationMaskPortrait;
    }
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
    NSLog(@"application:applicationWillResignActive");
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    NSLog(@"application:applicationDidEnterBackground");
    [self setIsSleepStatus:true];
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
    NSLog(@"application:applicationWillEnterForeground");
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
    NSLog(@"application:applicationDidBecomeActive");
    [self setIsSleepStatus:false];
}


- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    NSLog(@"application:applicationWillTerminate");
}

- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
    BOOL result = NO;
    result = [WXApi handleOpenURL:url delegate:[WXApiManager sharedManager]];
    NSString *stringURL =[url absoluteString];
    if(!result){
       NSLog(@"application handleOpenURL Failed: %@", stringURL);
    }
    return result;
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
    BOOL result = NO;
    result =  [WXApi handleOpenURL:url delegate:[WXApiManager sharedManager]];
    NSString *stringURL =[url absoluteString];
    NSLog(@"application openURL : %@", stringURL);
    return result;
}

-(void)setIsSleepStatus:(BOOL)status
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setBool:status forKey:@"lockScreenStatus"];
    [defaults synchronize];
}

@end


#import <UIKit/UIKit.h>
#import "WXApi.h"

@class MyViewController;

@interface QIOSApplicationDelegate : UIResponder <UIApplicationDelegate,WXApiDelegate>
@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) MyViewController *viewController;
@end


@interface QIOSApplicationDelegate (MyAppDelegate)
- (void) someMethod;
@end



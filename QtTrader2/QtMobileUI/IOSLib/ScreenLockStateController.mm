#import "ScreenLockStateController.h"

#pragma mark - ScreenLockStateController
@implementation ScreenLockStateController
{

}

-(BOOL)getIsSleepStatus
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    BOOL fingerLoginStatus = [defaults integerForKey:@"lockScreenStatus"];
    return fingerLoginStatus;
}


@end


#import <UIKit/UIKit.h>

@interface BiometricPromptController : UIViewController

   -(void)setFingerLoginStatus:(int)status;
   -(void)resetFingerLoginStatus;
   -(int)getFingerLoginStatus;
   -(void)fingerLogin;
@end


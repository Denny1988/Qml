#import "MyViewController.h"
#import "MyAppDelegate.h"

@interface MyViewController()
@end

@implementation MyViewController
{
}

- (void)viewDidLoad {
    [super viewDidLoad];
    NSLog(@"MyViewController:viewDidLoad");
}

- (id)initWithQIOSScreen:(UIScreen*)scrren{

    return self;
}

- (void)lockedOrientation{
    NSLog(@"MyViewController:lockedOrientation");
}

- (void)viewWillDisappear:(BOOL)animated{
    NSLog(@"MyViewController:viewWillDisappear");
}
- (void)viewWillAppear:(BOOL)animated{
    NSLog(@"MyViewController:viewWillAppear");
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    NSLog(@"MyViewController:didReceiveMemoryWarning");
}

- (void)managerDidRecvMessageResponse:(SendMessageToWXResp *)response {
    NSString *strTitle = [NSString stringWithFormat:@"发送媒体消息结果"];
    NSString *strMsg = [NSString stringWithFormat:@"errcode:%d", response.errCode];
    //[UIAlertView showWithTitle:strTitle message:strMsg sure:nil];
}

- (void)managerDidRecvAuthResponse:(SendAuthResp *)response {
    NSString *strTitle = [NSString stringWithFormat:@"Auth结果"];
    NSString *strMsg = [NSString stringWithFormat:@"code:%@,state:%@,errcode:%d", response.code, response.state, response.errCode];

    //[UIAlertView showWithTitle:strTitle message:strMsg sure:nil];
}

//成功登录
- (void)onAuthFinish:(int)errCode AuthCode:(NSString *)authCode
{
    NSLog(@"onAuthFinish");

//    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"onAuthFinish"
//                                                    message:[NSString stringWithFormat:@"authCode:%@ errCode:%d", authCode, errCode]
//                                                   delegate:self
//                                          cancelButtonTitle:@"OK"
//                                          otherButtonTitles:nil, nil];
//    [alert show];

}



@end


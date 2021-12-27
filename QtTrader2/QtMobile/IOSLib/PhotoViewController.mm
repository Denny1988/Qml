#include "PhotoViewController.h"

// iOS lib import
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


@interface APLViewController: UIViewController <UINavigationControllerDelegate, UIImagePickerControllerDelegate>{
}
@property (strong, nonatomic) NSString * imagePickerStatus;
@end

@implementation APLViewController

-(int)showImagePickerForPhotoPicker
{
    self.imagePickerStatus = @"";
    [self showImagePickerForSourceType:UIImagePickerControllerSourceTypePhotoLibrary];
    return 5;
}

-(void)showImagePickerForSourceType:(UIImagePickerControllerSourceType)sourceType
{
    UIImagePickerController *imagePickerController = [[UIImagePickerController alloc] init];
    imagePickerController.modalPresentationStyle = UIModalPresentationCurrentContext;
    imagePickerController.sourceType = sourceType;
    imagePickerController.delegate = (id)self;

    UIViewController *rootCtrl = [UIApplication sharedApplication].keyWindow.rootViewController;

    [rootCtrl presentViewController:imagePickerController animated:YES completion:nil];
}

-(void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    if([self.imagePickerStatus isEqualToString:@"Selected"])
        return;
    self.imagePickerStatus = @"Selected";
    NSLog(@"PhotoViewController imagePickerController begin...");
    
    UIImage *image = [info valueForKey:UIImagePickerControllerOriginalImage];
    NSData *data = UIImagePNGRepresentation(image);

    const auto &&image2 = QImage::fromData((const unsigned char *)[data bytes], [data length]);

    [picker dismissViewControllerAnimated:YES completion:NULL];

    emit PhotoViewController::g_currentImagePicker->imageSelected(image2);
    PhotoViewController::g_currentImagePicker = NULL;
}

-(void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [picker dismissViewControllerAnimated:YES completion:NULL];
    emit PhotoViewController::g_currentImagePicker->cancelSignal();
}

@end

PhotoViewController *PhotoViewController::g_currentImagePicker = NULL;

void PhotoViewController::show(void)
{
    PhotoViewController::g_currentImagePicker = this;
    void *context = [[APLViewController alloc] init];
    [(id) context showImagePickerForPhotoPicker];
}

CONFIG += precompile_header
PRECOMPILED_HEADER = $$PWD/stable.h

message("UI:Source_start")
SOURCES += $$PWD/main.cpp \
    $$PWD/manager.cpp \
    $$PWD/log.cpp \
    $$PWD/traderapp.cpp \
    $$PWD/viewModel/account.cpp \
    $$PWD/viewModel/instrument.cpp \
    $$PWD/viewModel/orderTreeModel.cpp \
    $$PWD/viewModel/ordertreeitem.cpp \
    $$PWD/viewModel/makeorder.cpp \
    $$PWD/Common/downloadmanager.cpp \
    $$PWD/Common/sortmanager.cpp \
    $$PWD/Common/uploadpicture.cpp \
    $$PWD/Common/viewconstants.cpp \
    $$PWD/Common/ViewHelper.cpp \
    $$PWD/Merchant/merchant.cpp \
    $$PWD/Merchant/merchantdialog.cpp \
    $$PWD/Merchant/merchantmanager.cpp \
    $$PWD/Merchant/splashscreen.cpp \
    $$PWD/Utils/biometricprompthelper.cpp \
    $$PWD/Utils/commonapihelper.cpp \
    $$PWD/Utils/galleryhelper.cpp \
    $$PWD/Utils/notifierhelper.cpp \
    $$PWD/Utils/qmldata.cpp \
    $$PWD/Utils/screenmanager.cpp \
    $$PWD/Utils/websocketclientwrapper.cpp \
    $$PWD/Utils/websockettransport.cpp \
    $$PWD/Language/LanguageItem.cpp \
    $$PWD/Language/qmutilanguages.cpp \
    $$PWD/AutoUpdate/autoupdatedialog.cpp \
    $$PWD/AutoUpdate/autoupdatemanager.cpp \
    $$PWD/Theme/themesetting.cpp \
    $$PWD/Theme/thememanager.cpp \
    $$PWD/viewinfo.cpp

message("UI:resources_start")
RESOURCES += \
    $$PWD/resources.qrc

message("UI:header_start")
HEADERS += \
    $$PWD/manager.h \
    $$PWD/log.h \
    $$PWD/traderapp.h \
    $$PWD/viewModel/account.h \
    $$PWD/viewModel/instrument.h \
    $$PWD/viewModel/orderTreeModel.h \
    $$PWD/viewModel/ordertreeitem.h \
    $$PWD/viewModel/makeorder.h \
    $$PWD/Common/downloadmanager.h \
    $$PWD/Common/sortmanager.h \
    $$PWD/Common/uploadpicture.h \
    $$PWD/Common/viewconstants.h \
    $$PWD/Common/ViewHelper.h \
    $$PWD/Common/viewstruct.h  \
    $$PWD/Merchant/merchant.h \
    $$PWD/Merchant/merchantdialog.h \
    $$PWD/Merchant/merchantmanager.h \
    $$PWD/Merchant/splashscreen.h \
    $$PWD/Utils/biometricprompthelper.h \
    $$PWD/Utils/commonapihelper.h \
    $$PWD/Utils/galleryhelper.h \
    $$PWD/Utils/notifierhelper.h \
    $$PWD/Utils/qmldata.h \
    $$PWD/Utils/screenmanager.h \
    $$PWD/Utils/websocketclientwrapper.h \
    $$PWD/Utils/websockettransport.h \
    $$PWD/Language/LanguageItem.h \
    $$PWD/Language/qmutilanguages.h \
    $$PWD/AutoUpdate/autoupdatedialog.h \
    $$PWD/AutoUpdate/autoupdatemanager.h \
    $$PWD/Theme/themesetting.h \
    $$PWD/Theme/thememanager.h \
    $$PWD/stable.h \
    $$PWD/stdafx.h \
    $$PWD/viewinfo.h

message("UI:form_start")
FORMS += \
    $$PWD/AutoUpdate/autoupdatewidget.ui \
    $$PWD/Merchant/merchantwidget.ui \
    $$PWD/Merchant/SplashScreen.ui

message("UI:DISTFILES_start")
DISTFILES += \
    $$PWD/Resources/Language/Chinese(Mainland).json \
    $$PWD/Resources/Language/Chinese(Taiwan).json \
    $$PWD/Resources/Language/English(US).json \
    $$PWD/Resources/Language/MutiLanguagesConfig.xml \
    $$PWD/Resources/Config/ServerConfig.xml \
    $$PWD/Resources/Theme/BlackViewItemColor.xml \
    $$PWD/Resources/Theme/ThemeSettings.xml \
    $$PWD/Resources/Theme/WhiteViewItemColor.xml \
    $$PWD/Resources/Theme/Black.qss \
    $$PWD/Resources/Theme/White.qss

unix:!mac {
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
    DISTFILES += \
        $$PWD/android/AndroidManifest.xml \
        $$PWD/android/gradle/wrapper/gradle-wrapper.jar \
        $$PWD/android/gradlew \
        $$PWD/android/res/values/libs.xml \
        $$PWD/android/build.gradle \
        $$PWD/android/gradle/wrapper/gradle-wrapper.properties \
        $$PWD/android/gradlew.bat \
        $$PWD/android/res/values/libs.xml \
        $$PWD/qml/main.qml \
        $$PWD/qml/LoginPage.qml \
        $$PWD/qml/MainPage.qml \
        $$PWD/images/background_loading_page.png \
        $$PWD/images/bottom_bar_icon_search_normal.png \
        $$PWD/images/bottom_bar_icon_set_normal.png \
        $$PWD/images/bottom_bar_icon_team_normal.png \
        $$PWD/images/button_default.png \
        $$PWD/images/button_pressed.png \
        $$PWD/images/cb_normal.png \
        $$PWD/images/cb_select.png \
        $$PWD/images/lemanager_account_security.png \
        $$PWD/images/lemanager_deal_record.png \
        $$PWD/images/lemanager_fix_manager.png \
        $$PWD/images/lemanager_highzone.png \
        $$PWD/images/lemanager_my_bankcard.png \
        $$PWD/images/lemanager_statistics_analysis.png \
        $$PWD/images/main_lecurrent0914.png \
        $$PWD/images/main_lecurrent0914_f.png \
        $$PWD/images/main_lediscovery0914.png \
        $$PWD/images/main_lediscovery0914_f.png \
        $$PWD/images/main_lefinancial0914.png \
        $$PWD/images/main_lefinancial0914_f.png \
        $$PWD/images/main_lemanager0914.png \
        $$PWD/images/main_lemanager0914_f.png \
        $$PWD/images/navigation_next_item.png \
        $$PWD/images/navigation_previous_item.png \
        $$PWD/images/tab_selected.png \
        $$PWD/images/tabs_standard.png \
        $$PWD/images/textinput.png \
        $$PWD/images/toolbar.png \
        $$PWD/android/src/hk/com/omnicare/iexchange4/android/biometricprompt/ACache.java \
        $$PWD/android/src/hk/com/omnicare/iexchange4/android/biometricprompt/BiometricActivity.java \
        $$PWD/android/src/hk/com/omnicare/iexchange4/android/biometricprompt/BiometricPromptApi23.java \
        $$PWD/android/src/hk/com/omnicare/iexchange4/android/biometricprompt/BiometricPromptApi28.java \
        $$PWD/android/src/hk/com/omnicare/iexchange4/android/biometricprompt/BiometricPromptDialog.java \
        $$PWD/android/src/hk/com/omnicare/iexchange4/android/biometricprompt/BiometricPromptManager.java \
        $$PWD/android/src/hk/com/omnicare/iexchange4/android/biometricprompt/IBiometricPromptImpl.java \
        $$PWD/android/src/hk/com/omnicare/iexchange4/android/biometricprompt/KeyGenTool.java \
        $$PWD/android/src/omnicare/AndroidHelper.java \
        $$PWD/android/src/omnicare/App.java \
        $$PWD/android/src/omnicare/AppService.java \
        $$PWD/android/src/omnicare/BiometricHelper.java \
        $$PWD/android/src/omnicare/IAppService.java \
        $$PWD/android/src/omnicare/ImgUtils.java \
        $$PWD/android/src/omnicare/MainActivity.java \
        $$PWD/android/src/omnicare/MyWebView.java \
        $$PWD/android/src/omnicare/NotifierHelper.java \
        $$PWD/android/src/omnicare/WebViewHelper.java \
        $$PWD/android/src/omnicare/WebViewJavascriptInterface.java \
        $$PWD/android/res/drawable/bg_white_corners.xml \
        $$PWD/android/res/drawable/btn_style_one.xml \
        $$PWD/android/res/drawable/btn_style_two.xml \
        $$PWD/android/res/drawable/btn_white_corner_selector.xml \
        $$PWD/android/res/drawable/ic_fingerprint.xml \
        $$PWD/android/res/drawable/ic_launcher_background.xml \
        $$PWD/android/res/drawable-v24/ic_launcher_foreground.xml \
        $$PWD/android/res/layout/activity_main.xml \
        $$PWD/android/res/layout/activity_webview.xml \
        $$PWD/android/res/layout/finger_login_dialog.xml \
        $$PWD/android/res/layout/layout_wxentry_activity.xml \
        $$PWD/android/res/mipmap-anydpi-v26/ic_launcher.xml \
        $$PWD/android/res/mipmap-anydpi-v26/ic_launcher_round.xml \
        $$PWD/android/res/values/colors.xml \
        $$PWD/android/res/values/libs.xml \
        $$PWD/android/res/values/strings.xml \
        $$PWD/android/res/values/styles.xml \
        $$PWD/android/res/raw/alert.wav \
        $$PWD/android/res/raw/cancel.wav \
        $$PWD/android/res/raw/connect.wav \
        $$PWD/android/res/raw/disconnect.wav \
        $$PWD/android/res/raw/execute.wav \
        $$PWD/android/res/raw/news.wav \
        $$PWD/android/res/raw/placed.wav \
        $$PWD/android/res/raw/quote.wav \
        $$PWD/android/res/raw/submitted.wav \
        $$PWD/android/res/raw/timeout.wav \
        $$PWD/android/res/drawable/back.png \
        $$PWD/android/res/drawable/icon.png \
        $$PWD/android/res/drawable-hdpi/btn_style_one_disabled.9.png \
        $$PWD/android/res/drawable-hdpi/btn_style_one_focused.9.png \
        $$PWD/android/res/drawable-hdpi/btn_style_one_normal.9.png \
        $$PWD/android/res/drawable-hdpi/btn_style_one_pressed.9.png \
        $$PWD/android/res/drawable-hdpi/ic_wechat.png \
        $$PWD/android/res/drawable-hdpi/icon.png \
        $$PWD/android/res/drawable-ldpi/icon.png \
        $$PWD/android/res/drawable-mdpi/icon.png \
        $$PWD/android/res/mipmap-hdpi/ic_launcher.png \
        $$PWD/android/res/mipmap-hdpi/ic_launcher_round.png \
        $$PWD/android/res/mipmap-mdpi/ic_launcher.png \
        $$PWD/android/res/mipmap-mdpi/ic_launcher_round.png \
        $$PWD/android/res/mipmap-xhdpi/ic_launcher.png \
        $$PWD/android/res/mipmap-xhdpi/ic_launcher_round.png \
        $$PWD/android/res/mipmap-xxhdpi/ic_launcher.png \
        $$PWD/android/res/mipmap-xxhdpi/ic_launcher_round.png \
        $$PWD/android/res/mipmap-xxxhdpi/ic_launcher.png \
        $$PWD/android/res/mipmap-xxxhdpi/ic_launcher_round.png \
        $$PWD/android/gradle/wrapper/gradle-wrapper.jar \
        $$PWD/android/libs/android-support-v4.jar \
        $$PWD/android/libs/wechat-sdk-android-with-mta-5.1.6.jar \
        $$PWD/android/doc/debug.keystore \
        $$PWD/android/doc/omni2.keystore \
        $$PWD/android/doc/Omnicare.keystore


         contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
                ANDROID_EXTRA_LIBS = \
                    $$PWD/../lib/libcrypto.so \
                    $$PWD/../lib/libssl.so
            }
     message("unix:!mac：add dist files for android os")
}

unix:mac {
    QMAKE_ASSET_CATALOGS += $$PWD/ios/Images.xcassets
    QMAKE_ASSET_CATALOGS += $$PWD/ios/Sound.bundle
    QMAKE_INFO_PLIST += $$PWD/ios/iosProInfo.plist
    DISTFILES +=  \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/AppIcon_1024x1024.png \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/AppIcon_20x20@2x.png \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/AppIcon_20x20@2x~ipad.png \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/AppIcon_20x20@3x.png \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/AppIcon_20x20~ipad.png \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/AppIcon_29x29@2x.png \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/AppIcon_29x29@2x~ipad.png \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/AppIcon_29x29@3x.png \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/AppIcon_29x29~ipad.png \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/AppIcon_40x40@2x.png \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/AppIcon_40x40@2x~ipad.png \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/AppIcon_40x40@3x.png \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/AppIcon_40x40~ipad.png \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/AppIcon_60x60@2x.png \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/AppIcon_60x60@3x.png \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/AppIcon_76x76@2x~ipad.png \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/AppIcon_76x76~ipad.png \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/AppIcon_83x84@2x~ipad.png \
        $$PWD/ios/Images.xcassets/AppIcon.appiconset/Contents.json \
        $$PWD/ios/Images.xcassets/LaunchImage.launchimage/LaunchImage_768x1024@2x-1.png  \
        $$PWD/ios/Images.xcassets/LaunchImage.launchimage/LaunchImage_768x1024-1.png  \
        $$PWD/ios/Images.xcassets/LaunchImage.launchimage/LaunchImage_768x1004@2x.png  \
        $$PWD/ios/Images.xcassets/LaunchImage.launchimage/LaunchImage_768x1004.png  \
        $$PWD/ios/Images.xcassets/LaunchImage.launchimage/LaunchImage_640x1136-1.png  \
        $$PWD/ios/Images.xcassets/LaunchImage.launchimage/LaunchImage_320x480@2x-1.png  \
        $$PWD/ios/Images.xcassets/LaunchImage.launchimage/LaunchImage_320x480.png  \
        $$PWD/ios/Images.xcassets/LaunchImage.launchimage/LaunchImage_768x1024@2x.png  \
        $$PWD/ios/Images.xcassets/LaunchImage.launchimage/LaunchImage_768x1024.png  \
        $$PWD/ios/Images.xcassets/LaunchImage.launchimage/LaunchImage_640x1136.png  \
        $$PWD/ios/Images.xcassets/LaunchImage.launchimage/LaunchImage_320x480@2x \
        $$PWD/ios/Images.xcassets/LaunchImage.launchimage/LaunchImage_750x1334.png  \
        $$PWD/ios/Images.xcassets/LaunchImage.launchimage/LaunchImage_1242x2208.png  \
        $$PWD/ios/Images.xcassets/LaunchImage.launchimage/LaunchImage_1125x2436.png  \
        $$PWD/ios/Images.xcassets/LaunchImage.launchimage/LaunchImage_828x1792.png  \
        $$PWD/ios/Images.xcassets/LaunchImage.launchimage/LaunchImage_1242x2688.png  \
        $$PWD/ios/Images.xcassets/LaunchImage.launchimage/Contents.json \
        $$PWD/ios/Images.xcassets/Contents.json \
        $$PWD/ios/Sound.bundle/alert.wav \
        $$PWD/ios/Sound.bundle/cancel.wav \
        $$PWD/ios/Sound.bundle/connect.wav \
        $$PWD/ios/Sound.bundle/disconnect.wav \
        $$PWD/ios/Sound.bundle/execute.wav \
        $$PWD/ios/Sound.bundle/news.wav \
        $$PWD/ios/Sound.bundle/placed.wav \
        $$PWD/ios/Sound.bundle/quote.wav \
        $$PWD/ios/Sound.bundle/submitted.wav \
        $$PWD/ios/Sound.bundle/timeout.wav

    HEADERS += \
        $$PWD/IOSLib/MyAppDelegate.h \
        $$PWD/IOSLib/MyViewController.h \
        $$PWD/IOSLib/ScreenApiHelper.h \
        $$PWD/IOSLib/ScreenController.h \
        $$PWD/IOSLib/ScreenLockStateController.h \
        $$PWD/IOSLib/PhotoViewController.h \
        $$PWD/IOSLib/BiometricPromptApiHelper.h \
        $$PWD/IOSLib/BiometricPromptController.h

    OBJECTIVE_SOURCES += \
        $$PWD/IOSLib/MyAppDelegate.mm \
        $$PWD/IOSLib/MyViewController.mm \
        $$PWD/IOSLib/ScreenApiHelper.mm \
        $$PWD/IOSLib/ScreenController.mm \
        $$PWD/IOSLib/ScreenLockStateController.mm \
        $$PWD/IOSLib/PhotoViewController.mm \
        $$PWD/IOSLib/BiometricPromptApiHelper.mm\
        $$PWD/IOSLib/BiometricPromptController.mm
}

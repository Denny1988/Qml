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
    $$PWD/ Common/sortmanager.cpp \
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


## Additional import path used to resolve QML modules in Qt Creator's code model
#QML_IMPORT_PATH += $$PWD/qml \
#$$PWD/qml/components

## Additional import path used to resolve QML modules just for Qt Quick Designer
#QML_DESIGNER_IMPORT_PATH = qml \
#    qml/components
#    images


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
        $$PWD/android/res/drawable/page_loading.png \
        $$PWD/qml/QuotationForm.ui.qml \
        $$PWD/qml/Login.qml \
        $$PWD/qml/main.qml \
        $$PWD/qml/Quotation.qml \
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
        $$PWD/qml/qmlscene \
        $$PWD/qml/components/dialogs/BaseDialog.qml \
        $$PWD/qml/components/dialogs/ConfirmationDialog.qml \
        $$PWD/qml/components/dialogs/DialogLoader.qml \
        $$PWD/qml/components/dialogs/FontFamilyDialog.qml \
        $$PWD/qml/components/dialogs/FontSizeDialog.qml \
        $$PWD/qml/components/dialogs/IndentSizeDialog.qml \
        $$PWD/qml/components/dialogs/ListDialog.qml \
        $$PWD/qml/components/dialogs/MessageDialog.qml \
        $$PWD/qml/components/dialogs/NewFileDialog.qml \
        $$PWD/qml/components/dialogs/NewProjectDialog.qml \
        $$PWD/qml/components/palettes/BasePalette.qml \
        $$PWD/qml/components/palettes/CutePalette.qml \
        $$PWD/qml/components/palettes/DarkPalette.qml \
        $$PWD/qml/components/palettes/LightPalette.qml \
        $$PWD/qml/components/palettes/PaletteLoader.qml \
        $$PWD/qml/components/CApplicationWindow.qml \
        $$PWD/qml/components/CBackButton.qml \
        $$PWD/qml/components/CCheckBox.qml \
        $$PWD/qml/components/CContextMenuButton.qml \
        $$PWD/qml/components/CDialogButton.qml \
        $$PWD/qml/components/CEditorScrollBar.qml \
        $$PWD/qml/components/CFlickable.qml \
        $$PWD/qml/components/CHorizontalSeparator.qml \
        $$PWD/qml/components/CIcon.qml \
        $$PWD/qml/components/CInformationItem.qml \
        $$PWD/qml/components/CLabel.qml \
        $$PWD/qml/components/CListView.qml \
        $$PWD/qml/components/CNavigationButton.qml \
        $$PWD/qml/components/CNavigationScrollBar.qml \
        $$PWD/qml/components/CScrollBar.qml \
        $$PWD/qml/components/CSettingButton.qml \
        $$PWD/qml/components/CSlider.qml \
        $$PWD/qml/components/CSwitcher.qml \
        $$PWD/qml/components/CTextArea.qml \
        $$PWD/qml/components/CTextField.qml \
        $$PWD/qml/components/CToolBar.qml \
        $$PWD/qml/components/CToolButton.qml \
        $$PWD/qml/components/CTooltip.qml \
        $$PWD/qml/components/CVerticalSeparator.qml \
        $$PWD/qml/screens/BlankScreen.qml \
        $$PWD/qml/screens/MainMenuScreen.qml \
        $$PWD/qml/screens/MainSwipeScreen.qml \
        $$PWD/qml/layout/AccountDetail.qml \
        $$PWD/qml/layout/AccountDetailRow.qml \
        $$PWD/qml/layout/Login.qml \
        $$PWD/qml/layout/MyInputRow.qml \
        $$PWD/qml/layout/MyItem.qml \
        $$PWD/qml/layout/PageHeader.qml \
        $$PWD/qml/layout/Quotation.qml \
        $$PWD/qml/layout/AccountPage.qml \
        $$PWD/qml/layout/OrdersPage.qml \
        $$PWD/qml/layout/PageHeader.qml \
        $$PWD/qml/layout/QuotationsPage.qml \
        $$PWD/qml/layout/TradingPage.qml \
        $$PWD/qml/components/CDimens.qml \
        $$PWD/qml/layout/PlacePanel.qml \
        $$PWD/qml/components/CHintButton.qml \
        $$PWD/qml/components/CTabButton.qml \
        $$PWD/qml/components/qmldir \
        $$PWD/qml/components/CMessageBox


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

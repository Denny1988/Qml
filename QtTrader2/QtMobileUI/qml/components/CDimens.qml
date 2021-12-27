pragma Singleton

import QtQuick 2.0
import QtQuick.Window 2.2

QtObject {
    property double pixelRatio : 1.00;  //缩放倍数
    property double pixelDensity :  4.72//2.84; //pd pid

    property double logicalPixelDensity :  4.72;

    property double screenheight : 1280;

    property double screenWidth : 720;

    property double pageHeight : 800;

    property double pageWidth : 480;

    property double headerHeight : 8 * pixelDensity;

    property double footerHeight : 8 * pixelDensity;

    property double normalLineHeight : 6 * pixelDensity;

    property double normalButtonHeight : 5 * pixelDensity;

    property double placeHintButtonHeight : 12 * pixelDensity;

    property double headerTitleBarHeight : 6 * pixelDensity;

    property double listLineHeight : 8 * pixelDensity;

    //font point size
    property double fontPointRatio : 1.00;  //缩放倍数
    property int bigFont : 20;
    property int normalFont : 16;
    property int smallFont : 12;


    function initialize(settings)
    {
        if( !settings  || settings.debugMode)
        {
            pixelRatio = 1.00;
            pixelDensity =  2.84;
            logicalPixelDensity =  4.72;
        }
        else
        {
            pixelRatio = Screen.devicePixelRatio;
            pixelDensity = Screen.pixelDensity;
            logicalPixelDensity = Screen.logicalPixelDensity;
            screenheight = Screen.height;
            screenWidth =  Screen.width;

            if(settings.platformName == "windows")
            {
                pixelDensity = logicalPixelDensity;
            }

            headerHeight = 8 * pixelDensity;
            footerHeight = 8 * pixelDensity;
            normalLineHeight = 6 * pixelDensity;
            normalButtonHeight = 6 * pixelDensity;
            placeHintButtonHeight = 12 * pixelDensity;
            headerTitleBarHeight  =  6 * pixelDensity;
            listLineHeight =  10 * pixelDensity;

            //font
            if(pixelDensity > 6 && pixelRatio > 2 )
            {
                if(screenWidth > 600){
                    fontPointRatio = 1.20;
                }
                else if(screenWidth > 300){
                    fontPointRatio = 1.10;
                }
            }
            bigFont = parseInt(fontPointRatio * 20);
            normalFont = parseInt(fontPointRatio * 16);
            smallFont = parseInt(fontPointRatio * 12);
        }
    }

}

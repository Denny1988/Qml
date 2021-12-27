import QtQuick 2.2
import QtWebView 1.1

Rectangle {
    id: rectangle
    x: 0
    y: 0
    //color: "#c0c0c0"
    color: "transparent"
    width: parent.width;
    height: parent.height;

    Component.onCompleted:
    {
        console.log("Component.onCompleted:");
        console.log("rectangle.x = " + rectangle.x);
        console.log("rectangle.y = " + rectangle.y);
        console.log("rectangle.width = " + rectangle.width);
        console.log("rectangle.height = " + rectangle.height);

        console.log("webView.x = " + webView.x);
        console.log("webView.y = " + webView.y);
        console.log("webView.width = " + webView.width);
        console.log("webView.height = " + webView.height);
        console.log("qmlData.url = " + qmlData.url);
    }

    //webObject Model
    QtObject {
        id: webObject;
        property int posX: qmlData.getViewX();
        property int posY: qmlData.getViewY();
        property int qmlWidth: qmlData.getQmlWidth();
        property int qmlHeight: qmlData.getQmlHeight();
        property string initialUrl: qmlData.url;
        property string titleText: qmlData.getTitle();
        property int titleBarHeight: qmlData.getTitleBarHeight();
        property string titleBarBgColor: qmlData.getTitleBarBgColor();
        property string titleBarColor: qmlData.getTitleBarColor();
        property int titleBackHeight: qmlData.getTitleBackHeight();
        property int titleBackTop: qmlData.getTitleBackTop();
        property string titleBackPicPath: qmlData.getTitleBackPicPath();
        property string titleBackText: qmlData.getTitleBackText();
        property int titleBarPixelSize: qmlData.getTitleBarPixelSize();

    }

    Connections {
            target: qmlData;
            onUrlChanged:{
                console.log("qmlData onUrlChanged");
                webObject.initialUrl = qmlData.url;
                console.log("webObject.initialUrl = " + webObject.initialUrl);
                console.log("webView.url = " + webView.url);
                webView.reload();
            }
    }
    Connections {
            target: qmlData;
            onDataChanged:
            {
                console.log("qmlData onUrlChanged");
                //webView.reload();
            }
    }

    Column {
        spacing: 0
        Rectangle {
          id: titleBar
           x:0
           y:0
           color: webObject.titleBarBgColor
           width: webObject.qmlWidth
           height: webObject.posY

           Image {
                   id: backIcon
                   x: webObject.titleBackTop/2
                   y: webObject.titleBackTop
                   width: webObject.titleBackHeight
                   height: webObject.titleBackHeight
                   source: webObject.titleBackPicPath
                   fillMode: Image.PreserveAspectFit
                   clip: true

                   MouseArea{
                       anchors.fill:parent
                       onClicked:{
                           console.log("qmlData.getCloseQmlPage:");
                           qmlData.getCloseQmlPage();
                       }
                   }
               }

           Text {
               id: backText
               x: webObject.titleBackHeight + webObject.titleBackTop -  webObject.titleBackTop/3
               y: 0
               text: qsTr(webObject.titleBackText)
               color: webObject.titleBarColor
               width: webObject.titleBackHeight
               height: webObject.titleBarHeight
               font.pixelSize: webObject.titleBarPixelSize
               font.bold:true
               verticalAlignment: Text.AlignVCenter
               MouseArea{
                   anchors.fill:parent
                   onClicked:{
                       console.log("qmlData.getCloseQmlPage:");
                       qmlData.getCloseQmlPage();
                   }
               }
           }

           Text {
               id: titleText
               x: webObject.titleBarHeight + webObject.titleBackHeight + webObject.titleBackTop -  webObject.titleBackTop/3
               y: 0
               text: qsTr(webObject.titleText)
               color: webObject.titleBarColor
               width: webObject.qmlWidth - 2*(webObject.titleBarHeight + webObject.titleBackHeight + webObject.titleBackTop -  webObject.titleBackTop/3)
               height: webObject.titleBarHeight
               font.pointSize:webObject.titleBarPointSize
               font.bold:true
               horizontalAlignment: Text.AlignHCenter
               verticalAlignment: Text.AlignVCenter
           }
        }

        WebView {
            id: webView
            url: webObject.initialUrl
            x:0
            y:webObject.posY
            width:  webObject.qmlWidth
            height: webObject.qmlHeight

            onLoadingChanged: {
                if (loadRequest.errorString){
                    console.error(loadRequest.errorString);
                }
                if(webView) {
                    console.log("onLoadingChanged:");
                    console.log("webView.x = " + webView.x);
                    console.log("webView.y = " + webView.y);
                    console.log("webView.width = " + webView.width);
                    console.log("webView.height = " + webView.height);
                }
            }
        }
    }



}

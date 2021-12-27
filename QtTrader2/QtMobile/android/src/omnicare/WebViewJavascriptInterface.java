package omnicare;

import android.content.Context;
import android.text.TextUtils;
import android.util.Log;
import android.webkit.JavascriptInterface;
import android.app.Activity;

public class WebViewJavascriptInterface {

    private Context context;

    public WebViewJavascriptInterface(Context context) {
        this.context = context;
    }

    @JavascriptInterface
    public void callJavaMethod() {
        Log.e("callJavaMethod", "----No parameter");
    }


    @JavascriptInterface
    public void callJavaMethod(String data) {
        Log.e("callJavaMethod", "----parameter: " + data);
        WebViewHelper.setJsReturnValue(data);
        ((MyWebView)this.context).finish();
    }

    @JavascriptInterface
    public void showSource(String html) {
        Log.e("showSourceCode", html);
    }

    @JavascriptInterface
    public void closeAndroid(){        
        WebViewHelper.setJsReturnValue("GoBack");
        ((MyWebView)this.context).finish();
    }

}

package omnicare;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Toast;
import android.util.Log;

public class WebViewHelper {
    private static String _jsReturnValue = "";

    public static int openWebView(String url, String title, String goBackText, String goBackImagePath){
       Intent intent = new Intent(App.getMainActivity(), MyWebView.class);
       intent.putExtra("url", url);
       intent.putExtra("title", title);
       intent.putExtra("goBackText", goBackText);
       intent.putExtra("goBackImagePath", goBackImagePath);
       App.getMainActivity().startActivity(intent);
       return 0;
    }

    public static String getJsReturnValue()
    {
        String returnStr = WebViewHelper._jsReturnValue;
        if(WebViewHelper._jsReturnValue.length() > 0)
        {
            WebViewHelper._jsReturnValue = "";
        }
        return returnStr;
    }

    public static void setJsReturnValue(String value)
    {
        WebViewHelper._jsReturnValue = value;
    }

}

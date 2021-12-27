package omnicare;

import android.app.Activity;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.ViewGroup;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.TextView;
import android.widget.ImageView;
import android.view.View;
import android.view.View.OnClickListener;
import android.content.Context;
import android.content.Intent;

import com.omnicare.qt.trader.*;

public class MyWebView  extends Activity{
    WebView mWebview;
    WebSettings mWebSettings;
    TextView mButton;
    TextView mTextView;
    ImageView mImageView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_webview);

        Intent intent=getIntent();
        String url=(String)intent.getSerializableExtra("url");
        String title=(String)intent.getSerializableExtra("title");
        String goBackText=(String)intent.getSerializableExtra("goBackText");

        mImageView = (ImageView)findViewById(R.id.goBackImage);
        mButton = (TextView)findViewById(R.id.goBackButton);
        mTextView = (TextView)findViewById(R.id.titleText);
        mWebview = (WebView)findViewById(R.id.webView1);

        mTextView.setText(title);
        mButton.setText(goBackText);

        mWebview.addJavascriptInterface(new WebViewJavascriptInterface(this), "android");
        mWebSettings = mWebview.getSettings();
        mWebSettings.setJavaScriptEnabled(true);
        mWebSettings.setJavaScriptCanOpenWindowsAutomatically(true);
        mWebSettings.setAllowFileAccess(true);
        mWebSettings.setSupportMultipleWindows(true);
        mWebSettings.setDomStorageEnabled(true);
        mWebSettings.setSupportZoom(true);
        mWebSettings.setBuiltInZoomControls(true);
        mWebSettings.setUseWideViewPort(true);
        mWebSettings.setLoadWithOverviewMode(true);
        mWebview.loadUrl(url);


        //设置不用系统浏览器打开,直接显示在当前Webview
        mWebview.setWebViewClient(new WebViewClient() {
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url) {
                view.loadUrl(url);
                return true;
            }
        });

        //设置WebChromeClient类
        mWebview.setWebChromeClient(new WebChromeClient() {
            //获取网站标题
            @Override
            public void onReceivedTitle(WebView view, String title) {
                System.out.println("标题在这里");
            }

            //获取加载进度
            @Override
            public void onProgressChanged(WebView view, int newProgress) {
                if (newProgress < 100) {
                    String progress = newProgress + "%";
                } else if (newProgress == 100) {
                    String progress = newProgress + "%";
                }
            }
        });


        //设置WebViewClient类
        mWebview.setWebViewClient(new WebViewClient() {
            //设置加载前的函数
            @Override
            public void onPageStarted(WebView view, String url, Bitmap favicon) {
                System.out.println("开始加载了");

            }

            //设置结束加载函数
            @Override
            public void onPageFinished(WebView view, String url) {

            }
        });

        mButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                 WebViewHelper.setJsReturnValue("GoBack");
                finish();
            }
        });

        mImageView.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                 WebViewHelper.setJsReturnValue("GoBack");
                finish();
            }
        });

    }

    //点击返回上一页面而不是退出浏览器
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK && mWebview.canGoBack()) {
            mWebview.goBack();
            return true;
        }

        return super.onKeyDown(keyCode, event);
    }

    //销毁Webview
    @Override
    protected void onDestroy() {
        if (mWebview != null) {
            mWebview.loadDataWithBaseURL(null, "", "text/html", "utf-8", null);
            mWebview.clearHistory();

            ((ViewGroup) mWebview.getParent()).removeView(mWebview);
            mWebview.destroy();
            mWebview = null;
        }
        super.onDestroy();
    }
}

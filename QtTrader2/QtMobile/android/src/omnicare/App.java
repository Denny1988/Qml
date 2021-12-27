package omnicare;

import android.support.v4.app.NotificationCompat;
import android.support.v4.app.ActivityCompat;
import android.app.Application;
import android.app.NotificationManager;
import android.app.Notification;
import android.app.NotificationChannel;
import android.content.pm.PackageManager;
import android.content.pm.PackageInfo;
import android.content.pm.ResolveInfo;
import android.content.ComponentName;
import android.content.Context;
import android.content.ContentValues;
import android.content.Intent;
import android.app.PendingIntent;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.util.Log;
import android.net.Uri;
import android.os.*;
import android.os.Vibrator;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.Locale;

import org.qtproject.qt5.android.bindings.QtApplication;
import com.omnicare.qt.trader.R;

public class App extends QtApplication {

    private static Context _AppContext = null;

    private static IAppService _AppService;

    private static Context _MainActivity = null;
    private static final String TAG = MainActivity.class.getSimpleName();
    private static final int CODE_READ_PHONE_STATE = 0x11;
    private String serialNumber = "UNKNOWN";

    private ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            _AppService = (IAppService) service;
            Log.d("App", "omnicare.App.AppService onServiceConnected");
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            _AppService =  null;
            Log.d("App", "omnicare.App.AppService onServiceDisconnected");
        }
    };


    @Override
    public void onCreate() {
        super.onCreate();
        Log.d("App", "omnicare.App.onCreate() begin");
        App._AppContext = this.getApplicationContext();
        App._AppContext.getApplicationContext().bindService(new Intent(App._AppContext, AppService.class),
                mServiceConnection, Context.BIND_AUTO_CREATE);
        Log.d("App", "omnicare.App.onCreate() end, success");
        closeAndroidPDialog();
    }

    @SuppressWarnings({ "unused", "rawtypes", "unchecked" })
    private void closeAndroidPDialog(){
        try {
            Class aClass = Class.forName("android.content.pm.PackageParser$Package");
            Constructor declaredConstructor = aClass.getDeclaredConstructor(String.class);
            declaredConstructor.setAccessible(true);
        } catch (Exception e) {
            e.printStackTrace();
        }

        //if (Build.VERSION.SDK_INT < 29)return;
        try {
            Class clazz = Class.forName("android.app.ActivityThread");
            Method currentActivityThread = clazz.getDeclaredMethod("currentActivityThread");
            currentActivityThread.setAccessible(true);
            Object activityThread = currentActivityThread.invoke(null);
            Field mHiddenApiWarningShown = clazz.getDeclaredField("mHiddenApiWarningShown");
            mHiddenApiWarningShown.setAccessible(true);
            mHiddenApiWarningShown.setBoolean(activityThread, true);
        } catch (Exception e) {
          e.printStackTrace();
        }
    }

    public static Context getAppContext(){
        return _AppContext;
    }

    public static Context getAppService(){
        return _AppService.getContext();
    }

    public static AppService.AppData getAppData(){
        return ((AppService.AppData) _AppService.getAppData());
    }

    public static Context getMainActivity(){
        return App._MainActivity;
    }

    public static void setMainActivity(Context activity){
        App._MainActivity = activity;
    }

    public static void openUrl(String url){
        Log.d("AndroidHelper", "omnicare.App.openUrl url = " + url);
        final Uri uri = Uri.parse(url);
        final Intent it = new Intent(Intent.ACTION_VIEW, uri);
        //it.setClassName("com.android.browser", "com.android.browser.BrowserActivity");
        App._MainActivity.startActivity(it);
    }

    public static synchronized String getVersionName() {
        try {
            PackageManager packageManager = App._AppContext.getPackageManager();
            PackageInfo packageInfo = packageManager.getPackageInfo(
                    App._AppContext.getPackageName(), 0);
            return packageInfo.versionName;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    public static synchronized int getVersionCode() {
        try {
            PackageManager packageManager = App._AppContext.getPackageManager();
            PackageInfo packageInfo = packageManager.getPackageInfo(
                    App._AppContext.getPackageName(), 0);
            return packageInfo.versionCode;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return 0;
    }

    public static String getDeviceModel()
    {
        String mtype = "";
        try{
            mtype = android.os.Build.MODEL; // 手机型号
        }catch (Exception e) {
            e.printStackTrace();
        }
        return mtype;
    }

    public static String getSerialNumber(){
        MainActivity mainActivity = (MainActivity)App._MainActivity;
        return mainActivity.getSerialNumber();
    }

    public static String getOSLanguage(){
      String language = null;
      try {
            Log.d("App", "getOSLanguage 1111");
             Locale locale;
             Log.d("App", "getOSLanguage 222");
             if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
                 locale = _AppService.getContext().getResources().getSystem().getConfiguration().getLocales().get(0);
                 Log.d("App", "getOSLanguage 3");
             } else {
                 //noinspection deprecation
                 locale = _AppService.getContext().getResources().getSystem().getConfiguration().locale;
                 Log.d("App", "getOSLanguage 4");
             }
             Log.i("App", Locale.getDefault().toLanguageTag());
             Log.d("App", "getOSLanguage 5");
             String ZH = "zh";
             //中文简体
             String CN_HANS = "zh-Hans";
             //中文繁体
             String CN_HANT = "zh-Hant";
             //如果是中文
             if (locale.getLanguage().equals(ZH)) {
                 //locale.toLanguageTag()
                 //中国 zh-Hans-CN
                 //台湾 zh-Hans-TW
                 //澳门 zh-Hans-MO
                 //香港 zh-Hans-HK
                 if (locale.toLanguageTag().contains(CN_HANS)) {
                     //简体
                     language = "zh-Hans";
                 } else if (locale.toLanguageTag().contains(CN_HANT)){
                     //繁体
                     language = "zh-Hant";
                 } else {
                     switch (locale.getCountry()) {
                         case "CN":
                             //简体
                             language = "zh-Hans";
                             break;
                         default:
                             language = "zh-Hant";
                             break;
                     }
                 }
             } else {
                 language = "en_US";
             }

         }catch (Exception ex) {
             Log.i("App", "Exception:getOSLanguage()", ex);
         }
      return language;
    }

    public static String getOSVersionCode()
    {
        String mtype = "";
        try{
            mtype = android.os.Build.VERSION.RELEASE; // 操作系统版本号
        }catch (Exception e) {
            e.printStackTrace();
        }
        return mtype;
    }

    public static void setBadgeCount(int count, String titleStr, String contentStr) {
        Context context = _AppService.getContext();
        if (count <= 0) {
            count = 0;
        } else {
            count = Math.max(0, Math.min(count, 99));
        }

        if (Build.MANUFACTURER.equalsIgnoreCase("xiaomi")) {
            setBadgeOfMIUI(context, count, R.mipmap.ic_launcher, titleStr, contentStr);
        } else if (Build.MANUFACTURER.equalsIgnoreCase("sony")) {
            setBadgeOfSony(context, count);
        } else if (Build.MANUFACTURER.toLowerCase().contains("samsung") ||
                Build.MANUFACTURER.toLowerCase().contains("lg")) {
            setBadgeOfSumsung(context, count);
        } else if (Build.MANUFACTURER.toLowerCase().contains("htc")) {
            setBadgeOfHTC(context, count);
        } else if (Build.MANUFACTURER.toLowerCase().contains("nova")) {
            setBadgeOfNova(context, count);
        } else {
            //Toast.makeText(context, "Not Found Support Launcher", Toast.LENGTH_LONG).show();
             Log.d("App", "setBadgeCount Not Found Support Launcher");
        }
    }

    /**
     * 设置MIUI的Badge
     */
    private static void setBadgeOfMIUI(Context context, int count, int iconResId, String titleStr, String contentStr) {
        NotificationManager mNotificationManager = (NotificationManager)context.getSystemService(Context.NOTIFICATION_SERVICE);
        if (mNotificationManager == null) {
            return;
        }
        NotificationCompat.Builder builder = new NotificationCompat.Builder(context);
        builder.setContentTitle(titleStr).setContentText(contentStr).setSmallIcon(iconResId).setPriority(NotificationCompat.PRIORITY_HIGH);
        Notification notification = builder.build();
        try {
            Field field = notification.getClass().getDeclaredField("extraNotification");
            Object extraNotification = field.get(notification);
            Method method = extraNotification.getClass().getDeclaredMethod("setMessageCount", int.class);
            method.invoke(extraNotification, count);
        } catch (Exception e) {
            e.printStackTrace();
        }
        mNotificationManager.notify(count, notification);
    }

    /**
     * 设置索尼的Badge
     * 需添加权限：<uses-permission android:name="com.sonyericsson.home.permission.BROADCAST_BADGE" />
     */
    private static void setBadgeOfSony(Context context, int count) {
        String launcherClassName = getLauncherClassName(context);
        if (launcherClassName == null) {
            return;
        }
        boolean isShow = true;
        if (count == 0) {
            isShow = false;
        }
        Intent localIntent = new Intent();
        localIntent.setAction("com.sonyericsson.home.action.UPDATE_BADGE");
        localIntent.putExtra("com.sonyericsson.home.intent.extra.badge.SHOW_MESSAGE", isShow);//是否显示
        localIntent.putExtra("com.sonyericsson.home.intent.extra.badge.ACTIVITY_NAME", launcherClassName);//启动页
        localIntent.putExtra("com.sonyericsson.home.intent.extra.badge.MESSAGE", String.valueOf(count));//数字
        localIntent.putExtra("com.sonyericsson.home.intent.extra.badge.PACKAGE_NAME", context.getPackageName());//包名
        context.sendBroadcast(localIntent);
    }

    /**
     * 设置三星的Badge\设置LG的Badge
     */
    private static void setBadgeOfSumsung(Context context, int count) {
        // 获取你当前的应用
        String launcherClassName = getLauncherClassName(context);
        if (launcherClassName == null) {
            return;
        }
        Intent intent = new Intent("android.intent.action.BADGE_COUNT_UPDATE");
        intent.putExtra("badge_count", count);
        intent.putExtra("badge_count_package_name", context.getPackageName());
        intent.putExtra("badge_count_class_name", launcherClassName);
        context.sendBroadcast(intent);
    }

    /**
     * 设置HTC的Badge
     */
    private static void setBadgeOfHTC(Context context, int count) {
        Intent intentNotification = new Intent("com.htc.launcher.action.SET_NOTIFICATION");
        ComponentName localComponentName = new ComponentName(context.getPackageName(), getLauncherClassName(context));
        intentNotification.putExtra("com.htc.launcher.extra.COMPONENT", localComponentName.flattenToShortString());
        intentNotification.putExtra("com.htc.launcher.extra.COUNT", count);
        context.sendBroadcast(intentNotification);

        Intent intentShortcut = new Intent("com.htc.launcher.action.UPDATE_SHORTCUT");
        intentShortcut.putExtra("packagename", context.getPackageName());
        intentShortcut.putExtra("count", count);
        context.sendBroadcast(intentShortcut);
    }

    /**
     * 设置Nova的Badge
     */
    private static void setBadgeOfNova(Context context, int count) {
        ContentValues contentValues = new ContentValues();
        contentValues.put("tag", context.getPackageName() + "/" + getLauncherClassName(context));
        contentValues.put("count", count);
        context.getContentResolver().insert(Uri.parse("content://com.teslacoilsw.notifier/unread_count"),
                contentValues);
    }

    public static void setBadgeOfMadMode(Context context, int count, String packageName, String className) {
        Intent intent = new Intent("android.intent.action.BADGE_COUNT_UPDATE");
        intent.putExtra("badge_count", count);
        intent.putExtra("badge_count_package_name", packageName);
        intent.putExtra("badge_count_class_name", className);
        context.sendBroadcast(intent);
    }

    /**
     * 重置Badge
     */
    public static void resetBadgeCount() {
        setBadgeCount(0, "", "");
    }

    public static String getLauncherClassName(Context context) {
        PackageManager packageManager = context.getPackageManager();
        Intent intent = new Intent(Intent.ACTION_MAIN);
        intent.setPackage(context.getPackageName());
        intent.addCategory(Intent.CATEGORY_LAUNCHER);
        ResolveInfo info = packageManager.resolveActivity(intent, PackageManager.MATCH_DEFAULT_ONLY);
        if (info == null) {
            info = packageManager.resolveActivity(intent, 0);
        }
        return info.activityInfo.name;
    }

    public static void startVibrate(int duration)  {
        Vibrator v = (Vibrator)_AppService.getContext().getSystemService(Context.VIBRATOR_SERVICE);
        v.vibrate(duration);
    }

    public static void playSound(String type)
    {
        MainActivity main = (MainActivity)App.getMainActivity();
        main.playSound(type);
    }
}

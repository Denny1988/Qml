package omnicare;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;

import android.text.ClipboardManager;
import omnicare.*;

public class AndroidHelper {
    private static boolean _isSleepStatus = false;
    private static String _orientation = "-1";//-1= unspecified 0= landscape 1= portrait
    public static String getAppExCardDirectoryPath() throws IOException {
            try
            {
                File dirsFile = new File(Environment.getExternalStorageDirectory()
                                + File.separator
                                + App.getAppContext().getPackageName());
                if (!dirsFile.exists()) {
                        if (dirsFile.mkdirs()) {
                                Log.d("MY_LOG_FILE", "Mkdirs Path = " + dirsFile.getPath());
                        } else {
                                Log.d("MY_LOG_FILE",
                                                "Mkdirs Path Fail ! #" + dirsFile.getPath());
                        }
                } else {
                        Log.d("MY_LOG_FILE", "Path = " + dirsFile.getPath());
                }
                return dirsFile.getPath();
            }
            catch(Throwable ex){
               ex.printStackTrace();
            }
            return "";
    }


    public static String getAppDataDirectoryPath()
    {
        //File file = App.getAppContext().getDataDir();
        //return file.getPath();
        return App.getAppContext().getFilesDir().getPath();
    }

    public static String getAppCacheDirectoryPath()
    {
        File file = App.getAppContext().getCacheDir();
        return file.getPath();
    }

    public static void setIsSleepStatus(boolean isSleepStatus)
    {
        AndroidHelper._isSleepStatus = isSleepStatus;
    }

    public static boolean getIsSleepStatus()
    {
        return AndroidHelper._isSleepStatus;
    }

    public static void setAppOrientation(String orientation)
    {
        AndroidHelper._orientation = orientation;
    }

    public static String getAppOrientation()
    {
        return AndroidHelper._orientation;
    }

    public static int openUrl(String url)  {
        App.openUrl(url);
        return 0;
    }

    public static int getAppVersionCode()
    {
        return App.getVersionCode();
    }

    public static String getAppversionName()
    {
        return App.getVersionName();
    }

    public static String getDeviceModel()
    {
        return App.getDeviceModel();
    }

    public static String getSerialNumber(){
      return App.getSerialNumber();
    }

    public static String getOSLanguage()
    {
        return App.getOSLanguage();
    }

    public static String getOSVersionCode()
    {
        return App.getOSVersionCode();
    }

    public static int setBadgeCount(int count, String titleStr, String contentStr)
    {
        App.setBadgeCount(count, titleStr, contentStr);
        return 0;
    }

    public static void copyTxtToSysytemClipboard(String txt)  {
        MainActivity mainActivity = (MainActivity)App.getMainActivity();
        ClipboardManager cm = (ClipboardManager)mainActivity.getSystemService(Context.CLIPBOARD_SERVICE);
        cm.setText(txt);
         Log.d("App", "copyTxtToSysytemClipboard: "+ txt);
    }

    public static String getAppGalleryPath()
    {
        String galleryPath = Environment.getExternalStorageDirectory()
                        + File.separator + Environment.DIRECTORY_DCIM
                        +File.separator+"Camera"+ File.separator;
        return galleryPath;
    }

    public static void startVibrate(int duration)  {
        App.startVibrate(duration);
    }

    public static int playSound(String type)
    {
        App.playSound(type);
        return 0;
    }
}

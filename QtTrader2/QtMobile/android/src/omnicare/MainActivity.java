package omnicare;

import java.util.HashMap;
import java.util.Set;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import javax.crypto.Cipher;

import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.content.Intent;
import android.content.Context;
import android.content.ContentUris;
import android.content.res.Configuration;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.os.Bundle;
import android.os.Environment;
import android.os.Build;
import android.os.Build.VERSION;
import android.util.Log;
import android.util.Base64;
import android.view.View;
import android.view.KeyEvent;
import android.widget.Button;
import android.widget.Toast;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.provider.MediaStore;
import android.provider.DocumentsContract;
import android.hardware.biometrics.BiometricPrompt;
import android.hardware.fingerprint.FingerprintManager;
import android.provider.Settings.Secure;
import java.lang.reflect.Method;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import org.qtproject.qt5.android.bindings.QtApplication;
import org.qtproject.qt5.android.bindings.QtActivity;

import hk.com.omnicare.iexchange4.android.biometricprompt.*;
import com.omnicare.qt.trader.*;

import android.media.AudioManager;
import android.media.SoundPool;
import android.media.AudioAttributes;

public class MainActivity extends QtActivity {
    private BiometricPromptManager _BiometricManager;

    public static native void fileSelected(String fileName);
    static final int REQUEST_OPEN_IMAGE = 1;
    private static MainActivity m_instance;
    private SoundPool m_soundPool;
    HashMap<Integer, Integer> m_soundData;

    public MainActivity()
    {
        m_instance = this;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d("MainActivity", "onCreate() Begin");
        App.setMainActivity(this);
        _BiometricManager = BiometricPromptManager.from(this);

        if(Build.VERSION.SDK_INT>=Build.VERSION_CODES.M &&
        checkSelfPermission(android.Manifest.permission.READ_EXTERNAL_STORAGE)!= PackageManager.PERMISSION_GRANTED)
        {
            //请求权限
            requestPermissions(new String[]{
            android.Manifest.permission.READ_EXTERNAL_STORAGE,
            android.Manifest.permission.WRITE_EXTERNAL_STORAGE,
            android.Manifest.permission.READ_PHONE_STATE},1);
        }
        initSound();
        Log.d("App", "NativeBreakpad init end");
    }

    @Override
    public void onResume() {
        super.onResume();
        AndroidHelper.setIsSleepStatus(false);
        Log.d("MainActivity", "onResume() Intent" + getIntent());
    }

    @Override
    protected void onStop() {
       super.onStop();
       AndroidHelper.setIsSleepStatus(true);
        Log.d("MainActivity", "onStop() Intent" + getIntent());
    }

    @Override
    public void super_onNewIntent(Intent intent){
        super.super_onNewIntent(intent);
        Log.d("MainActivity", "super_onNewIntent intent:" + intent);
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        Log.d("MainActivity", "onNewIntent intent:" + intent);
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        Log.d("MainActivity", "onConfigurationChanged newConfig:" + newConfig);
         if(this.getResources().getConfiguration().orientation ==Configuration.ORIENTATION_LANDSCAPE) {
             AndroidHelper.setAppOrientation("0");//0= landscape
             Log.d("MainActivity", "land donothing is ok");
         } else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)  {
            AndroidHelper.setAppOrientation("1");//1= portrait
            Log.d("MainActivity", "port donothing is ok");
         }
        else
        {
            AndroidHelper.setAppOrientation("-1");//-1= unspecified
            Log.d("MainActivity", "port donothing is ok");
        }
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK)
                return true;
        return super.onKeyUp(keyCode, event);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
       if (resultCode == RESULT_OK)
        {
            if(requestCode == REQUEST_OPEN_IMAGE)
            {
                String filePath = getPath(getApplicationContext(), data.getData());
                Log.d("MainActivity", "onActivityResult filePath:" + filePath);
                fileSelected(filePath);
            }
        }
        else
        {
            fileSelected("");
        }

        super.onActivityResult(requestCode, resultCode, data);
    }


    //----------------------public------------------------------
    public String openFingerLogin() {
        Log.d("MainActivity", "openFingerLogin start");
        String checkError = checkHardware();
        if(checkError!="") return checkError;
        try
        {
            Log.d("MainActivity", "openFingerLogin start authenticate");
            _BiometricManager.authenticate(false, new BiometricPromptManager.OnBiometricIdentifyCallback() {
                @Override
                public void onUsePassword() {
                    Log.d("MainActivity", "VERSION_CODES.M onUsePassword");
                }

                @RequiresApi(api = Build.VERSION_CODES.M)
                @Override
                public void onSucceeded(FingerprintManager.AuthenticationResult result) {
                    try {
                        Log.d("MainActivity", "VERSION_CODES.M openFingerLogin authenticate onSucceeded");
                        Cipher cipher = result.getCryptoObject().getCipher();
                        byte[] iv = cipher.getIV();
                        String ivStr = Base64.encodeToString(iv,Base64.URL_SAFE);
                        //Log.d("MainActivity", "设置指纹时保存的加密IV: " + ivStr);
                        BiometricHelper.setBiometricPromptIV(ivStr);
                        BiometricHelper.setOpenFingerLoginStatus(1);
                        Log.d("MainActivity", "VERSION_CODES.M onSucceeded end ");
                    }catch (Exception e){
                        Log.e("MainActivity", "openfingerLogin Failed:", e);
                        BiometricHelper.setOpenFingerLoginStatus(0);
                    }
                }

                @RequiresApi(api = Build.VERSION_CODES.P)
                @Override
                public void onSucceeded(BiometricPrompt.AuthenticationResult result) {
                    try {
                        Log.d("MainActivity", "VERSION_CODES.P openFingerLogin authenticate onSucceeded");
                        Cipher cipher = result.getCryptoObject().getCipher();
                        byte[] iv = cipher.getIV();
                        String ivStr = Base64.encodeToString(iv,Base64.URL_SAFE);
                        //Log.d("MainActivity", "设置指纹时保存的加密IV: " + ivStr);
                        BiometricHelper.setBiometricPromptIV(ivStr);
                        BiometricHelper.setOpenFingerLoginStatus(1);
                        Log.d("MainActivity", "VERSION_CODES.P onSucceeded end ");
                    }catch (Exception e){
                        Log.e("MainActivity", "openfingerLogin Failed:", e);
                        BiometricHelper.setOpenFingerLoginStatus(0);
                    }
                }

                @Override
                public void onFailed() {
                    Log.d("MainActivity", "openFingerLogin authenticate onFailed");
                    BiometricHelper.setOpenFingerLoginStatus(0);
                }

                @Override
                public void onError(int code, String reason) {
                    Log.d("MainActivity", "openFingerLogin authenticate onError");
                    BiometricHelper.setOpenFingerLoginStatus(0);
                }

                @Override
                public void onCancel() {
                    Log.d("MainActivity", "openFingerLogin authenticate onCancel");
                    BiometricHelper.setOpenFingerLoginStatus(0);
                }

            });
            Log.d("MainActivity", "openFingerLogin end");
            return "";
        } catch(Exception ex) {
            Log.e("MainActivity", "openfingerLogin Failed:", ex);
            return "OpenFingerLoginFailed";
        }
    }

    public String fingerLogin(){
       Log.d("MainActivity", "fingerLogin start");
       String checkError = checkHardware();
       if(checkError!="") return checkError;
        try
        {
            Log.d("MainActivity", "fingerLogin start authenticate");
            _BiometricManager.authenticate(true, new BiometricPromptManager.OnBiometricIdentifyCallback() {
                @Override
                public void onUsePassword() {
                }

                @RequiresApi(api = Build.VERSION_CODES.M)
                @Override
                public void onSucceeded(FingerprintManager.AuthenticationResult result) {
                    try {
                        Log.d("MainActivity", "fingerLogin authenticate onSucceeded");
                        BiometricHelper.setFingerLoginStatus(1);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }

                @RequiresApi(api = Build.VERSION_CODES.P)
                @Override
                public void onSucceeded(BiometricPrompt.AuthenticationResult result) {
                    try {
                        Log.d("MainActivity", "fingerLogin authenticate onSucceeded");
                       BiometricHelper.setFingerLoginStatus(1);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }

                @Override
                public void onFailed() {
                    Log.d("MainActivity", "fingerLogin authenticate onFailed");
                    BiometricHelper.setFingerLoginStatus(0);
                }

                @Override
                public void onError(int code, String reason) {
                    Log.d("MainActivity", "fingerLogin authenticate onError:" + reason);
                    BiometricHelper.setFingerLoginStatus(0);
                }

                @Override
                public void onCancel() {
                    Log.d("MainActivity", "fingerLogin authenticate onCancel");
                    BiometricHelper.setFingerLoginStatus(0);
                }
            });
            Log.d("MainActivity", "fingerLogin end");
            return "";
        } catch(Exception ex) {
            Log.e("MainActivity", "fingerLogin Failed:", ex);
            return "FingerLoginFailed";
        }
    }

    public String checkHardware(){
        try
        {
            if (Build.VERSION.SDK_INT >= 23 ) {
                if (_BiometricManager.isHardwareDetected()
                && _BiometricManager.hasEnrolledFingerprints()
                && _BiometricManager.isKeyguardSecure()){
                    Log.d("MainActivity", "手机硬件支持指纹登录");
                    return "";
                }else {
                    Log.d("MainActivity", "手机硬件不支持指纹登录");
                    return "MobileNotSupportFingerprint";
                }
            }else {
                Log.d("MainActivity", "API 低于23,不支持指纹登录");
                return "APILowerThan23NotSupportFingerprint";
            }
        } catch(Exception ex) {
            Log.e("MainActivity", "检查硬件失败:", ex);
            return "CheckHardwareFailed";
        }
    }

    public String getRealPathFromURI(Context context, Uri contentUri)
    {
        Cursor cursor = null;
        try
        {
            String[] proj = { MediaStore.Images.Media.DATA };
            cursor = context.getContentResolver().query(contentUri,  proj, null, null, null);
            int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
            cursor.moveToFirst();
            return cursor.getString(column_index);
        }
        finally
        {
            if (cursor != null)
            {
                cursor.close();
            }
        }
    }

    public void playSound(String type){
        Integer m_voiceId;
        if(type.equals("alert"))
            m_voiceId = m_soundData.get(R.raw.alert);
        else if(type.equals("cancel"))
            m_voiceId = m_soundData.get(R.raw.cancel);
        else if(type.equals("connect"))
            m_voiceId = m_soundData.get(R.raw.connect);
        else if(type.equals("disconnect"))
            m_voiceId = m_soundData.get(R.raw.disconnect);
        else if(type.equals("execute"))
            m_voiceId = m_soundData.get(R.raw.execute);
        else if(type.equals("news"))
            m_voiceId = m_soundData.get(R.raw.news);
        else if(type.equals("placed"))
            m_voiceId = m_soundData.get(R.raw.placed);
        else if(type.equals("quote"))
            m_voiceId = m_soundData.get(R.raw.quote);
        else if(type.equals("submitted"))
            m_voiceId = m_soundData.get(R.raw.submitted);
        else
            m_voiceId = m_soundData.get(R.raw.timeout);
       Log.d("MainActivity", "omnicare.MainActivity.playSound type:" + type + " m_voiceId:" + m_voiceId);
        m_soundPool.play(m_voiceId, 1, 1, 1, 0, 1);
    }

    public void initSound(){
        Log.d("MainActivity", "omnicare.MainActivity.initSound start");

        //实例化SoundPool
        //sdk版本21是SoundPool 的一个分水岭
        if (Build.VERSION.SDK_INT >= 21) {
            Log.d("MainActivity", "omnicare.MainActivity.initSound SDK >= 21");
            SoundPool.Builder builder = new SoundPool.Builder();
            //传入最多播放音频数量,
            builder.setMaxStreams(10);
            //AudioAttributes是一个封装音频各种属性的方法
            AudioAttributes.Builder attrBuilder = new AudioAttributes.Builder();
            //设置音频流的合适的属性
            attrBuilder.setLegacyStreamType(AudioManager.STREAM_MUSIC);
            //加载一个AudioAttributes
            builder.setAudioAttributes(attrBuilder.build());
            m_soundPool = builder.build();
        } else {
            /**
            * 第一个参数：int maxStreams：SoundPool对象的最大并发流数
            * 第二个参数：int streamType：AudioManager中描述的音频流类型
            *第三个参数：int srcQuality：采样率转换器的质量。 目前没有效果。 使用0作为默认值。
            */
            Log.d("MainActivity", "omnicare.MainActivity.initSound SDK < 21");
            m_soundPool = new SoundPool(10, AudioManager.STREAM_MUSIC, 0);
        }

        //可以通过四种途径来记载一个音频资源：
        //1.通过一个AssetFileDescriptor对象
        //int load(AssetFileDescriptor afd, int priority)
        //2.通过一个资源ID
        //int load(Context context, int resId, int priority)
        //3.通过指定的路径加载
        //int load(String path, int priority)
        //4.通过FileDescriptor加载
        //int load(FileDescriptor fd, long offset, long length, int priority)
        //声音ID 加载音频资源,这里用的是第二种，第三个参数为priority，声音的优先级*API中指出，priority参数目前没有效果，建议设置为1。
        m_soundData = new HashMap<Integer, Integer>();
        String path = App.getAppContext().getFilesDir().getPath() + File.separator + "Merchant" + File.separator + "Sounds";
        Log.d("MainActivity", "omnicare.MainActivity.initSound path:" + path);
        File dirsFile = new File(path);
        if (dirsFile.exists()) {
             Log.d("MainActivity", "omnicare.MainActivity.initSound path exist!");
            path = path + File.separator;
            m_soundData.put(R.raw.alert, m_soundPool.load(path + "alert.wav", 1));
            m_soundData.put(R.raw.cancel, m_soundPool.load(path + "cancel.wav", 1));
            m_soundData.put(R.raw.connect, m_soundPool.load(path + "connect.wav", 1));
            m_soundData.put(R.raw.disconnect, m_soundPool.load(path + "disconnect.wav", 1));
            m_soundData.put(R.raw.execute, m_soundPool.load(path + "execute.wav", 1));
            m_soundData.put(R.raw.news, m_soundPool.load(path + "news.wav", 1));
            m_soundData.put(R.raw.placed, m_soundPool.load(path + "placed.wav", 1));
            m_soundData.put(R.raw.quote, m_soundPool.load(path + "quote.wav", 1));
            m_soundData.put(R.raw.submitted, m_soundPool.load(path + "submitted.wav", 1));
            m_soundData.put(R.raw.timeout, m_soundPool.load(path + "timeout.wav", 1));
        }
        else{
            Log.d("MainActivity", "omnicare.MainActivity.initSound path not exist!");
            m_soundData.put(R.raw.alert, m_soundPool.load(this, R.raw.alert, 1));
            m_soundData.put(R.raw.cancel, m_soundPool.load(this, R.raw.cancel, 1));
            m_soundData.put(R.raw.connect, m_soundPool.load(this, R.raw.connect, 1));
            m_soundData.put(R.raw.disconnect, m_soundPool.load(this, R.raw.disconnect, 1));
            m_soundData.put(R.raw.execute, m_soundPool.load(this, R.raw.execute, 1));
            m_soundData.put(R.raw.news, m_soundPool.load(this, R.raw.news, 1));
            m_soundData.put(R.raw.placed, m_soundPool.load(this, R.raw.placed, 1));
            m_soundData.put(R.raw.quote, m_soundPool.load(this, R.raw.quote, 1));
            m_soundData.put(R.raw.submitted, m_soundPool.load(this, R.raw.submitted, 1));
            m_soundData.put(R.raw.timeout, m_soundPool.load(this, R.raw.timeout, 1));
        }


        //异步需要等待加载完成，音频才能播放成功
        m_soundPool.setOnLoadCompleteListener(new SoundPool.OnLoadCompleteListener() {
            @Override
            public void onLoadComplete(SoundPool soundPool, int sampleId, int status) {
                if (status == 0) {
                    //第一个参数soundID
                    //第二个参数leftVolume为左侧音量值（范围= 0.0到1.0）
                    //第三个参数rightVolume为右的音量值（范围= 0.0到1.0）
                    //第四个参数priority 为流的优先级，值越大优先级高，影响当同时播放数量超出了最大支持数时SoundPool对该流的处理
                    //第五个参数loop 为音频重复播放次数，0为值播放一次，-1为无限循环，其他值为播放loop+1次
                    //第六个参数 rate为播放的速率，范围0.5-2.0(0.5为一半速率，1.0为正常速率，2.0为两倍速率)
                    Log.d("MainActivity", "omnicare.MainActivity.initSound Load Complete sampleId:" + sampleId);
                }
                else
                    Log.d("MainActivity", "omnicare.MainActivity.initSound Load Complete status!=0 sampleId:" +sampleId);
            }
        });
    }

    public String getSerialNumber(){
      String serial = null;
      try {
          if (Build.VERSION.SDK_INT > Build.VERSION_CODES.O) {
               Log.d("MainActivity", "omnicare.MainActivity.getSerialNumber android 8.0+");
              serial = Build.getSerial();
              if(serial.length() == 0)
                  serial = Secure.getString(App.getAppContext().getContentResolver(), Secure.ANDROID_ID);
                Log.d("App", "omnicare.MainActivity.getSerialNumber serial: "+ serial);

          } else {
              Log.d("MainActivity", "omnicare.MainActivity.getSerialNumber android 8.0-");
              Class<?> c =Class.forName("android.os.SystemProperties");
              Method get =c.getMethod("get", String.class);
              serial = (String)get.invoke(c, "ro.serialnocustom");
              if(serial.length() == 0)
                  serial = (String)get.invoke(c, "ro.serialno");
              Log.d("App", "omnicare.MainActivity.getSerialNumber serial: "+ serial);
          }
      } catch (Exception e) {
        e.printStackTrace();
      }
      return serial;
    }

    //----------------------private------------------------------
    private void dispatchOpenGallery()
    {
    //       Intent intent = new Intent();
    //       intent.addCategory(Intent.CATEGORY_OPENABLE);
    //       intent.setType("image/*");
    //        if (Build.VERSION.SDK_INT <19) {
    //            intent.setAction(Intent.ACTION_GET_CONTENT);
    //        }else {
    //            intent.setAction(Intent.ACTION_OPEN_DOCUMENT);
    //        }
    //        startActivityForResult(intent, REQUEST_OPEN_IMAGE);
        Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
        intent.setType("image/*");
        startActivityForResult(intent, REQUEST_OPEN_IMAGE);
    }

    //-----------------------------------static-----------------------------
    public static void openAnImage()
    {
        if(Build.VERSION.SDK_INT>=Build.VERSION_CODES.M && m_instance.checkSelfPermission(android.Manifest.permission.READ_EXTERNAL_STORAGE)!= PackageManager.PERMISSION_GRANTED)
        {
            //请求权限
            m_instance.requestPermissions(new String[]{
            android.Manifest.permission.READ_EXTERNAL_STORAGE,
            android.Manifest.permission.WRITE_EXTERNAL_STORAGE},1);
            return;
        }
        m_instance.dispatchOpenGallery();
    }

    /**
     * Get a file path from a Uri. This will get the the path for Storage Access
     * Framework Documents, as well as the _data field for the MediaStore and
     * other file-based ContentProviders.
     *
     * @param context The context.
     * @param uri The Uri to query.
     * @author paulburke
     */
    public static String getPath(final Context context, final Uri uri) {

        final boolean isKitKat = Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT;

        // DocumentProvider
        if (isKitKat && DocumentsContract.isDocumentUri(context, uri)) {
            // ExternalStorageProvider
            if (isExternalStorageDocument(uri)) {
                final String docId = DocumentsContract.getDocumentId(uri);
                final String[] split = docId.split(":");
                final String type = split[0];

                if ("primary".equalsIgnoreCase(type)) {
                    return Environment.getExternalStorageDirectory() + "/" + split[1];
                }

                // TODO handle non-primary volumes
            }
            // DownloadsProvider
            else if (isDownloadsDocument(uri)) {

                final String id = DocumentsContract.getDocumentId(uri);
                final Uri contentUri = ContentUris.withAppendedId(
                        Uri.parse("content://downloads/public_downloads"), Long.valueOf(id));

                return getDataColumn(context, contentUri, null, null);
            }
            // MediaProvider
            else if (isMediaDocument(uri)) {
                final String docId = DocumentsContract.getDocumentId(uri);
                final String[] split = docId.split(":");
                final String type = split[0];

                Uri contentUri = null;
                if ("image".equals(type)) {
                    contentUri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
                } else if ("video".equals(type)) {
                    contentUri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;
                } else if ("audio".equals(type)) {
                    contentUri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
                }

                final String selection = "_id=?";
                final String[] selectionArgs = new String[] {
                        split[1]
                };

                return getDataColumn(context, contentUri, selection, selectionArgs);
            }
        }
        // MediaStore (and general)
        else if ("content".equalsIgnoreCase(uri.getScheme())) {
            return getDataColumn(context, uri, null, null);
        }
        // File
        else if ("file".equalsIgnoreCase(uri.getScheme())) {
            return uri.getPath();
        }

        return null;
    }

    /**
     * Get the value of the data column for this Uri. This is useful for
     * MediaStore Uris, and other file-based ContentProviders.
     *
     * @param context The context.
     * @param uri The Uri to query.
     * @param selection (Optional) Filter used in the query.
     * @param selectionArgs (Optional) Selection arguments used in the query.
     * @return The value of the _data column, which is typically a file path.
     */
    public static String getDataColumn(Context context, Uri uri, String selection, String[] selectionArgs) {
        Cursor cursor = null;
        final String column = "_data";
        final String[] projection = {
                column
        };

        try {
            cursor = context.getContentResolver().query(uri, projection, selection, selectionArgs,
                    null);
            if (cursor != null && cursor.moveToFirst()) {
                final int column_index = cursor.getColumnIndexOrThrow(column);
                return cursor.getString(column_index);
            }
        } finally {
            if (cursor != null)
                cursor.close();
        }
        return null;
    }


    /**
     * @param uri The Uri to check.
     * @return Whether the Uri authority is ExternalStorageProvider.
     */
    public static boolean isExternalStorageDocument(Uri uri) {
        return "com.android.externalstorage.documents".equals(uri.getAuthority());
    }

    /**
     * @param uri The Uri to check.
     * @return Whether the Uri authority is DownloadsProvider.
     */
    public static boolean isDownloadsDocument(Uri uri) {
        return "com.android.providers.downloads.documents".equals(uri.getAuthority());
    }

    /**
     * @param uri The Uri to check.
     * @return Whether the Uri authority is MediaProvider.
     */
    public static boolean isMediaDocument(Uri uri) {
        return "com.android.providers.media.documents".equals(uri.getAuthority());
    }


}

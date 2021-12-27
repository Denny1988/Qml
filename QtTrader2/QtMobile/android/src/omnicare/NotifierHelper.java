package hk.com.omnicare.iexchange4.android;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Toast;
import android.util.Log;

import android.os.Build;
import javax.crypto.Cipher;
import android.util.Base64;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;

import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.R.drawable;

import omnicare.*;
import com.omnicare.qt.trader.R;


public class NotifierHelper {    

    private static NotificationManager m_notificationManager;
    private static Notification.Builder m_builder;

    public static void notify(String notificationContent){
        MainActivity mainActivity = (MainActivity)App.getMainActivity();

        Log.d("MainActivity", "notify start");
       if (m_notificationManager == null) {
           Log.d("MainActivity", "_notificationManager is null");
           m_notificationManager = (NotificationManager)mainActivity.getSystemService(Context.NOTIFICATION_SERVICE);
           m_builder = new Notification.Builder(mainActivity);
           m_builder.setSmallIcon(R.drawable.icon);
           m_builder.setContentTitle("A message from Qt!");
       }

       m_builder.setContentText(notificationContent);
       Log.d("MainActivity", "notify setContentText");
       m_notificationManager.notify(1, m_builder.build());
       Log.d("MainActivity", "notify end");
    }

}

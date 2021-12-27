package omnicare;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Toast;
import android.util.Log;

import android.hardware.biometrics.BiometricPrompt;
import android.hardware.fingerprint.FingerprintManager;
import android.os.Build;
import javax.crypto.Cipher;
import android.util.Base64;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;

import hk.com.omnicare.iexchange4.android.biometricprompt.*;


public class BiometricHelper {
    private static String _BiometricPromptIV = "";
    private static int _FingerLoginStatus = -1;//-1 = None 0=Faile 1=success
    private static int _OpenFingerLoginStatus = -1;//-1 = None 0=Faile 1=success

    public static String openFingerLogin(){
        MainActivity mainActivity = (MainActivity)App.getMainActivity();
        return mainActivity.openFingerLogin();
    }

    public static String fingerLogin(){
        MainActivity mainActivity = (MainActivity)App.getMainActivity();
        return mainActivity.fingerLogin();
    }

    //check Hardware is support Biometric
    public static String checkHardware(){
        MainActivity mainActivity = (MainActivity)App.getMainActivity();
        return mainActivity.checkHardware();
    }

    public static boolean setBiometricPromptIV(String iv){
        BiometricHelper._BiometricPromptIV = iv;
        return true;
    }

    public static String getBiometricPromptIV()
    {
        return BiometricHelper._BiometricPromptIV;
    }

    public static void setFingerLoginStatus(int status)
    {
        BiometricHelper._FingerLoginStatus = status;
    }

    public static int getFingerLoginStatus()
    {
        return BiometricHelper._FingerLoginStatus;
    }

    public static void setOpenFingerLoginStatus(int status)
    {
        Log.d("BiometricHelper", "setOpenFingerLoginStatus:" + status);
        BiometricHelper._OpenFingerLoginStatus = status;
        Log.d("BiometricHelper", "setOpenFingerLoginStatus11:" + BiometricHelper._OpenFingerLoginStatus);
    }

    public static int getOpenFingerLoginStatus()
    {
        Log.d("BiometricHelper", "getOpenFingerLoginStatus:" + BiometricHelper._OpenFingerLoginStatus);
        return BiometricHelper._OpenFingerLoginStatus;
    }

    public static boolean resetOpenFingerLoginStatus(){
        BiometricHelper._OpenFingerLoginStatus = -1;
        return true;
    }

    public static boolean resetFingerLoginStatus(){
        BiometricHelper._FingerLoginStatus = -1;
        return true;
    }

}

package hk.com.omnicare.iexchange4.android.biometricprompt;

import android.app.Activity;
import android.content.DialogInterface;
import android.hardware.biometrics.BiometricPrompt;
import android.os.Build;
import android.os.CancellationSignal;
import android.util.Base64;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;

import omnicare.*;
import com.omnicare.qt.trader.R;
import android.util.Log;

@RequiresApi(Build.VERSION_CODES.P)
public class BiometricPromptApi28 implements IBiometricPromptImpl {

    private Activity mActivity;
    private BiometricPrompt mBiometricPrompt;
    private BiometricPromptManager.OnBiometricIdentifyCallback mManagerIdentifyCallback;
    private CancellationSignal mCancellationSignal;

    @RequiresApi(Build.VERSION_CODES.P)
    public BiometricPromptApi28(Activity activity) {
        mActivity = activity;
        mBiometricPrompt = new BiometricPrompt
                .Builder(activity)
                .setTitle(activity.getResources().getString(R.string.title))
                .setDescription(activity.getResources().getString(R.string.touch_2_auth))
                .setSubtitle("")
                .setNegativeButton(activity.getResources().getString(R.string.use_password),
                        activity.getMainExecutor(), new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        if (mManagerIdentifyCallback != null) {
                            mManagerIdentifyCallback.onUsePassword();
                        }
                        mCancellationSignal.cancel();
                    }
                })
                .build();

    }

    @RequiresApi(Build.VERSION_CODES.P)
    @Override
    public void authenticate(boolean loginFlg, @Nullable CancellationSignal cancel,
                             @NonNull BiometricPromptManager.OnBiometricIdentifyCallback callback) {
        mManagerIdentifyCallback = callback;

        mCancellationSignal = cancel;
        if (mCancellationSignal == null) {
            mCancellationSignal = new CancellationSignal();
        }
        mCancellationSignal.setOnCancelListener(new CancellationSignal.OnCancelListener() {
            @Override
            public void onCancel() {
            }
        });

        KeyGenTool mKeyGenTool = new KeyGenTool(mActivity);
        BiometricPrompt.CryptoObject object;
        if (loginFlg){
            //??????
            try {
                /**
                 * ????????????????????????iv,???????????????????????????????????????
                 */
                //???????????????AES-CBC
                String ivStr = BiometricHelper.getBiometricPromptIV();
                byte[] iv = Base64.decode(ivStr, Base64.URL_SAFE);

                object = new BiometricPrompt.CryptoObject(mKeyGenTool.getDecryptCipher(iv));
                mBiometricPrompt.authenticate(object,
                        mCancellationSignal, mActivity.getMainExecutor(), new BiometricPromptCallbackImpl());
            } catch (Exception e) {
                e.printStackTrace();
            }
        }else {
            //??????
            try {
                object = new BiometricPrompt.CryptoObject(mKeyGenTool.getEncryptCipher());
                mBiometricPrompt.authenticate(object,
                        mCancellationSignal, mActivity.getMainExecutor(), new BiometricPromptCallbackImpl());
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

    }

    @RequiresApi(Build.VERSION_CODES.P)
    private class BiometricPromptCallbackImpl extends BiometricPrompt.AuthenticationCallback {
        @Override
        public void onAuthenticationError(int errorCode, CharSequence errString) {
            super.onAuthenticationError(errorCode, errString);
            mCancellationSignal.cancel();

        }

        @Override
        public void onAuthenticationHelp(int helpCode, CharSequence helpString) {
            super.onAuthenticationHelp(helpCode, helpString);
        }

        @Override
        public void onAuthenticationSucceeded(BiometricPrompt.AuthenticationResult result) {
            super.onAuthenticationSucceeded(result);
            mManagerIdentifyCallback.onSucceeded(result);
            mCancellationSignal.cancel();
        }

        @Override
        public void onAuthenticationFailed() {
            super.onAuthenticationFailed();
        }
    }

}

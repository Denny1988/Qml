package hk.com.omnicare.iexchange4.android.biometricprompt;

import android.os.CancellationSignal;
import androidx.annotation.NonNull;



interface IBiometricPromptImpl {

    void authenticate(boolean loginFlg, @NonNull CancellationSignal cancel,
                      @NonNull BiometricPromptManager.OnBiometricIdentifyCallback callback);

}

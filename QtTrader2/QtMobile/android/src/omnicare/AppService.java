package omnicare;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;
import android.content.Context;

import com.tencent.mm.opensdk.constants.ConstantsAPI;
import com.tencent.mm.opensdk.modelbase.BaseReq;
import com.tencent.mm.opensdk.modelbase.BaseResp;
import com.tencent.mm.opensdk.modelmsg.GetMessageFromWX;
import com.tencent.mm.opensdk.modelmsg.SendAuth;
import com.tencent.mm.opensdk.modelmsg.ShowMessageFromWX;
import com.tencent.mm.opensdk.openapi.IWXAPIEventHandler;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.Set;

public class AppService extends Service implements IAppService, IWXAPIEventHandler {

    private ServiceBinder mServiceBinder = new ServiceBinder();

    private AppService.AppData mAppData= new AppData();

    @Override
    public IBinder onBind(Intent intent) {
        return mServiceBinder;
    }

    @Override
    public Object getAppData() {
        return mAppData;
    }

    @Override
    public Context getContext(){
        return this;
    }

    @Override
    public void onReq(BaseReq baseReq) {
        Log.d("AppService", "onReq begin:" + baseReq);
        switch (baseReq.getType()) {
            case ConstantsAPI.COMMAND_GETMESSAGE_FROM_WX:
                break;
            case ConstantsAPI.COMMAND_SHOWMESSAGE_FROM_WX:
                break;
            default:
                break;
        }
        Log.d("AppService","onReq end.");
    }

    @Override
    public void onResp(BaseResp baseresp) {
        Log.d("AppService", "onResp begin:" + baseresp);
        String result = "";
        switch (baseresp.errCode) {
            case BaseResp.ErrCode.ERR_OK:
                result = "errcode_success";
                break;
            case BaseResp.ErrCode.ERR_USER_CANCEL:
                result = "errcode_cancel";
                break;
            case BaseResp.ErrCode.ERR_AUTH_DENIED:
                result = "errcode_deny";
                break;
            case BaseResp.ErrCode.ERR_UNSUPPORT:
                result = "errcode_unsupported";
                break;
            default:
                result = "errcode_unknown";
                break;
        }
        String message = "baseresp.getType = " + baseresp.getType() + " result = " + result;
        Log.d("AppService",message);
    }



    public class ServiceBinder extends Binder implements IAppService {

        @Override
        public Object getAppData() {
            return AppService.this.getAppData();
        }
        @Override
        public Context getContext(){
            return AppService.this;
        }
    }

    public static class AppData {

        private final HashMap<String,Object> mWXRespData = new  HashMap<String,Object>();

        private boolean mIsAuthorized = false;

        public AppData(){

        }

        public void setAuthorized(boolean value)
        {
            mIsAuthorized = value;
        }

        public boolean isAuthorized()
        {
           return mIsAuthorized;
        }

        public HashMap<String, Object> getWXRespData() {
            return mWXRespData;
        }

        public String getWXRespData(String key) {
            if(mWXRespData.containsKey(key)){
                return mWXRespData.get(key).toString();
            }
            return "null";
        }

        public String getWXAuthRespData() {
            Object authResult = mWXRespData.get("SendAuth.Resp");
            return authResult.toString();
        }

        public String getAuthRespCode() {
            if(mIsAuthorized)
            {
                try{
                    Object authResult = mWXRespData.get("SendAuth.Resp");
                    JSONObject json = (JSONObject)authResult;
                    String ret = json.getString("_wxapi_sendauth_resp_token");
                    return ret;
                } catch(Exception ex){
                    Log.e("AppService", "getAuthRespCode", ex);
                    return null;
                }
            }
            return "";
        }
    }
}

//************ TODO-COCOS-FIX START ************//

package org.cocos2dx.lib;

import org.cocos2dx.lib.Cocos2dxHelper;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;

import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;

import android.util.Log;

public class Cocos2dxNativeAlert {

	private static native void alertDidDismiss( String alertID, int index );

	public static void _alertDidDismiss( final String alertID, final int index ) {
		Cocos2dxGLSurfaceView.getInstance().queueEvent(
			new Runnable() {
			public void run() {
				alertDidDismiss( alertID, index);
			}
		});
	}
	
	public static String showNativeAlert(final String aTitle, final String aMsg, final String aCnclBtn, final String aRetBtn, final String aOthBtn) {

		FutureTask<String> task = new FutureTask<String>(new Callable<String>() {
			@Override
			public String call() throws Exception {
				AlertDialog.Builder builder1 = new AlertDialog.Builder(Cocos2dxHelper.getActivity());
				if (aTitle.length() > 0) builder1.setTitle( aTitle );
				if (aMsg.length() > 0) builder1.setMessage( aMsg );
				
				DialogInterface.OnClickListener diaInterf = new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int id) {
						dialog.cancel();
						
						Cocos2dxNativeAlert._alertDidDismiss( "" + dialog, id );
					}
				};
				
				if (aCnclBtn.length() > 0) {
					builder1.setNegativeButton( aCnclBtn, diaInterf );
					builder1.setCancelable(true);
                } else {
                    builder1.setCancelable(false);
                }
				
				if (aRetBtn.length() > 0) {
					builder1.setPositiveButton( aRetBtn, diaInterf );
				}
				
				if (aOthBtn.length() > 0) {
					builder1.setNeutralButton( aOthBtn, diaInterf );
				}
				
                
				AlertDialog alert1 = builder1.create();
                
				alert1.show();
				return "" + alert1;
			}
		});
		
		// Log.d("NativeAlert (Java)","showNativeAlert: About to try callInMainThread" );
		try {
			Cocos2dxHelper.getActivity().runOnUiThread(task);
			String alertID = task.get();
			// Log.d("NativeAlert (Java)","showNativeAlert: returning " + alertID );
			return alertID;
		} catch (Exception e) {
			Log.d("NativeAlert (Java)","showNativeAlert: excpetion = " + e );
			return "";
		}
	}
}
//************ TODO-COCOS-FIX END ************//

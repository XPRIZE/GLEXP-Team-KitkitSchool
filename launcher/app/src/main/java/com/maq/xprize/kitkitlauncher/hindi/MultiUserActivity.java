package com.maq.xprize.kitkitlauncher.hindi;

import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioGroup;
import android.widget.TextView;

import com.maq.kitkitProvider.KitkitDBHandler;
import com.maq.kitkitProvider.User;

import java.util.ArrayList;

public class MultiUserActivity extends AppCompatActivity {


    private static final String TAG = "UserNameActivity" ;
    Dialog addUserDialog;
    TextView closeButton;
    TextView gender;
    EditText usrnameInput;
    EditText userAge;
    String userGender = "";
    Button submit;
    private RadioGroup genderRadioGroup;
    private Context schoolContext;
    private SharedPreferences schoolPref;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_multi_user);
        addUserDialog = new Dialog(this);
    }

    public void AddAllUser(View v) {

        addUserDialog.setContentView(R.layout.addalluser);
        closeButton = addUserDialog.findViewById(R.id.textView3);
        usrnameInput = addUserDialog.findViewById(R.id.editText);
        gender =  addUserDialog.findViewById(R.id.textView4);
        userAge =  addUserDialog.findViewById(R.id.editText2);
        submit =  addUserDialog.findViewById(R.id.button2);
        genderRadioGroup =  addUserDialog.findViewById(R.id.radio);

        closeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                addUserDialog.dismiss();
            }
        });

        genderRadioGroup.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //get selected radio button from Radio Group
                int selectedId = genderRadioGroup.getCheckedRadioButtonId();
                switch(selectedId){
                    case R.id.radioFemale:
                        userGender = "Female";
                        break;
                    case R.id.radioMale:
                        userGender = "Male";
                        break;
                }
            }
        });

        // to show the Add user dialog
        addUserDialog.show();

        submit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    KitkitDBHandler dbHandler = new KitkitDBHandler(getApplicationContext());
                    User user = new User(usrnameInput.getText().toString(), userGender,userAge.getText().toString());
                    dbHandler.addUser(user);
                    dbHandler.setCurrentUser(user);

                    Intent intent = new Intent(MultiUserActivity.this, MainActivity.class);
                    startActivity(intent);
                    finish();

                } catch (Exception e) {
                    System.out.println("Error in adding user " + e.getMessage());
                }
            }
        });
    }

    // code for selecting all users.

    public void SelectAllUser(View view) {
        KitkitDBHandler dbHandler = ((LauncherApplication) getApplication()).getDbHandler();
        ArrayList<User> users = dbHandler.getUserList();

        try {
            schoolContext = getApplicationContext().createPackageContext("com.maq.xprize.kitkitschool.hindi", 0);
            schoolPref = schoolContext.getSharedPreferences("Cocos2dxPrefsFile", Context.MODE_PRIVATE);
            for (User u:users) {
                u.setGamesClearedInTotal_L(schoolPref.getInt((u.getUserName() + "_gamesClearedInTotal_en-US_L"), 0));
                u.setGamesClearedInTotal_M(schoolPref.getInt((u.getUserName() + "_gamesClearedInTotal_en-US_M"), 0));
            }
        }
        catch (PackageManager.NameNotFoundException ne) {
            Log.e(TAG, ne.toString());
        }

        UserNameListDialog userNameListDialog = new UserNameListDialog(MultiUserActivity.this, users);
        userNameListDialog.show();
    }

    public void SetUser(View view) {
        new SelectNumberDialog(MultiUserActivity.this, SelectNumberDialog.MODE.USER_NO, new SelectNumberDialog.Callback() {
            @Override
            public void onSelectedNumber(int number) {
                KitkitDBHandler dbHandler = ((LauncherApplication) getApplication()).getDbHandler();
                User user = dbHandler.findUser("user" + number);
                if (user != null) {
                    dbHandler.setCurrentUser(user);
                }
            }
        });
        Intent intent = new Intent(MultiUserActivity.this, MainActivity.class);
        startActivity(intent);
        finish();
    }
}
package com.maq.xprize.kitkitlauncher.hindi;

import android.app.Dialog;
import android.content.Intent;
import android.database.sqlite.SQLiteDatabase;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;
import android.graphics.Color;
import android.graphics.Rect;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;

import com.maq.kitkitProvider.KitkitDBHandler;
import com.maq.kitkitProvider.User;

import java.util.ArrayList;

//import com.maq.kitkitProvider.KitkitDBHandler;
//import com.maq.kitkitProvider.User;
//import com.maq.kitkitlogger.KitKitLogger;
//import com.maq.kitkitlogger.KitKitLoggerActivity;

public class FirstScreen extends AppCompatActivity {


    Dialog mydialog;
    Dialog mydialogS;
    TextView txtclose;
    TextView usrname;
    TextView gender;
    EditText usrnameInput;
    TextView age;
    EditText ageno;
    SQLiteDatabase mydb;
    String userGender = "Male";
    Button submit;
    TextView setuser;
    private User currentUser;
    private String currentUsername;
    int i = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_first_screen);
        // Util.hideSystemUI(this);
        mydialog = new Dialog(this);
        mydialogS = new Dialog(this);


    }

    private RadioGroup RG;
    private RadioButton mf;

    public void AddAllUser(View v) {

        mydialog.setContentView(R.layout.addalluser);
        txtclose = (TextView) mydialog.findViewById(R.id.textView3);
        usrnameInput = (EditText) mydialog.findViewById(R.id.editText);
        //usrname.setOnClickListener(EOnClickListner);
        gender = (TextView) mydialog.findViewById(R.id.textView4);
        ageno = (EditText) mydialog.findViewById(R.id.editText2);
        submit = (Button) mydialog.findViewById(R.id.button2);
        RG = (RadioGroup) mydialog.findViewById(R.id.radio);

        RG.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int selectedId = RG.getCheckedRadioButtonId();//get selected radio button from Radio Group

                mf = (RadioButton) findViewById(selectedId);

                Toast.makeText(FirstScreen.this, mf.getText(), Toast.LENGTH_SHORT).show();
            }
        });

        //Add here txtclose and submit methods

        //  boolean checked = ((RadioButton) v).isChecked();
       /* switch(v.getId()) {
          case R.id.radioFemale:
              if (checked)
              userGender = "female";
         break;
        case R.id.radioMale:
          if (checked)
            userGender = "male";
        break;
        }
*/
        mydialog.show();
        submit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    KitkitDBHandler dbHandler = new KitkitDBHandler(getApplicationContext());

                    User user = new User(toString(), toString(), toString());
                    user.setDisplayName(usrnameInput.getText().toString());
                    user.setAge(ageno.getText().toString());
                    user.setgender(userGender);


                    dbHandler.addUser(user);
                    dbHandler.setCurrentUser(user);


                    Intent intent = new Intent(FirstScreen.this, MainActivity.class);
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
        UserNameListDialog userNameListDialog = new UserNameListDialog(FirstScreen.this, users);
        userNameListDialog.show();
    }

    public void SetUser(View view) {
        SelectNumberDialog selectNumberDialog = new SelectNumberDialog(FirstScreen.this, SelectNumberDialog.MODE.USER_NO, new SelectNumberDialog.Callback() {
            @Override
            public void onSelectedNumber(int number) {
                KitkitDBHandler dbHandler = ((LauncherApplication) getApplication()).getDbHandler();
                User user = dbHandler.findUser("user" + number);
                if (user != null) {
                    dbHandler.setCurrentUser(user);
                }
            }
        });
        Intent intent = new Intent(FirstScreen.this, MainActivity.class);
        startActivity(intent);
        finish();
    }
}





// private View.OnClickListener EOnClickListner = new View.OnClickListener() {
//   @Override
// public void onClick(View v) {
//   switch(v.getId()){
//     case R.id.textView2:
                /*{
                    User user = ((LauncherApplication) getApplication()).getDbHandler().getCurrentUser();
                    RenameUserDialog registerUserDialog = new RenameUserDialog(FirstScreen.this, user, new RenameUserDialog.Callback() {
                        @Override
                        public void onSubmit(String name) {
                            KitkitDBHandler kitkitDBHandler = ((LauncherApplication) getApplication()).getDbHandler();
                            User user = kitkitDBHandler.getCurrentUser();
                            if (user != null) {
                                user.setDisplayName(name);
                                kitkitDBHandler.updateUser(user);
                            }
                            //refreshUI();
                        }
                    });

                    registerUserDialog.show();
                }
                break;*/
//            }
//         }
// };



    //public void onClick(View final){
   // }


//    mydialogS.setContentView(R.layout.selectalluser);
//  KitkitDBHandler dbHandler = ((LauncherApplication) getApplication()).getDbHandler();
//ArrayList<User> users = dbHandler.getUserList();
//User user = dbHandler.findUser("user" + number);
//if(user!=null)
//mydialogS.show();

//}


package library.todoschool.enuma.com.todoschoollibrary;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

/**
 * Created by ingtellect on 12/23/16.
 */

public class BookDetailActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //getWindow().requestFeature(Window.FEATURE_ACTION_BAR);
        setContentView(R.layout.activity_bookdetail);

        View roundedRectView = findViewById(R.id.bg);
        roundedRectView.setClipToOutline(true);


    }
}

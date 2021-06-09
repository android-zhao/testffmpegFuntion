package com.example.helloffmpeg.newffmpeg;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;

import com.example.helloffmpeg.R;

import java.io.File;

public class MainActivitynewFffmpeg extends Activity {

    private LinearLayout ll_content;
    String TAG = "newFffmpeg";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_activitynew_fffmpeg);
        initView();

    }

    File mFile;

    void initView() {
        ll_content = findViewById(R.id.ll_content_new);
        mFile = new File(getCacheDir().getAbsoluteFile() + "/sintel.mp4");
        Log.i(TAG, "mFile :" + mFile.getAbsolutePath());

        addButton("show and test ffmpeg", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                FFmpegUtils.helloFFmpeg_new();
            }
        });
        addButton("print file info", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                FFmpegUtils.printFileInfo_new("file:" + mFile.getAbsolutePath());
            }
        });
        addButton("decode video 2 yuv", new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                File src = new File(getCacheDir().getAbsolutePath(), "sintel.h264");
                File dst = new File(getCacheDir().getAbsolutePath(), "sintel_frame");
                FFmpegUtils.decodeVideo2yuv(src.getAbsolutePath(),dst.getAbsolutePath());
            }
        });
    }

    private void addButton(String name, View.OnClickListener lisener) {
        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        lp.setMargins(20, 20, 20, 20);

        Button button = new Button(this);
        button.setPadding(0, 20, 0, 20);
        button.setGravity(Gravity.CENTER);
        button.setTextSize(TypedValue.COMPLEX_UNIT_DIP, 16);
        button.setText(name);
        button.setAllCaps(false);
        button.setOnClickListener(lisener);

        ll_content.addView(button, lp);
    }
}
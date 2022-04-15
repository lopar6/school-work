package com.example.logans_inventory;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.DownloadManager;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TableRow;
import android.widget.TextView;
import android.widget.Toast;

public class LoginActivity extends AppCompatActivity {
    private static final int SMS_PERMISSION_CODE = 1;
    InventoryDatabase db;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        db = new InventoryDatabase(getApplicationContext());
        setContentView(R.layout.login_activity);

        View storage = findViewById(R.id.permissions);

        storage.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                checkPermission(Manifest.permission.SEND_SMS, SMS_PERMISSION_CODE);
            }
        });

        Button loginButton = findViewById(R.id.buttonLogin);
        loginButton.setOnClickListener(loginButtonListener);
        Button signUpButton = findViewById(R.id.buttonSignUp);
        signUpButton.setOnClickListener(signUpButtonListener);
    }

    public void checkPermission(String permission, int requestCode)
    {
        this.requestPermissions(new String[] { permission }, requestCode);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           @NonNull String[] permissions,
                                           @NonNull int[] grantResults)
    {
        ActivityCompat.requestPermissions(this,
                new String[] { Manifest.permission.SEND_SMS },
                requestCode);
        super.onRequestPermissionsResult(SMS_PERMISSION_CODE, permissions, grantResults);

        if (requestCode == SMS_PERMISSION_CODE) {
            if (grantResults.length > 0
                    && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                Toast.makeText(this, "SMS Permission Granted", Toast.LENGTH_SHORT).show();
            }
            else {
                Toast.makeText(this, "SMS Permission Denied", Toast.LENGTH_SHORT).show();
            }
        }
    }

    private View.OnClickListener loginButtonListener = new View.OnClickListener() {
        @SuppressLint("SetTextI18n")
        public void onClick(View v) {

            EditText userText = findViewById(R.id.userNameText);
            String userName = userText.getText().toString();
            EditText passwordText = findViewById(R.id.userPasswordText);
            String password = passwordText.getText().toString();

            if( !(userName.equals("")) && !(password.equals(""))) {
                if (db.login(userName, password)) {
                    setContentView(R.layout.activity_data);
                    Intent intent = new Intent(getApplicationContext(), DataActivity.class);
                    startActivity(intent);
                } else {
                    TextView errorView = findViewById(R.id.loginErrorText);
                    errorView.setText("Username/Password combination incorrect");
                }
            }
        }
    };

    private View.OnClickListener signUpButtonListener = new View.OnClickListener() {
        public void onClick(View v) {
            EditText userText = findViewById(R.id.userNameText);
            String userName = userText.getText().toString();
            EditText passwordText = findViewById(R.id.userPasswordText);
            String password = passwordText.getText().toString();

            Toast.makeText(getApplicationContext(), "User Created", Toast.LENGTH_SHORT).show();
            db.signUp(userName, password);
        }
    };
}
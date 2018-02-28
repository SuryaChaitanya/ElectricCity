package com.example.anjali.retreivedata;


import android.app.ProgressDialog;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

public class relay extends AppCompatActivity {
    Switch simpleSwitch1, simpleSwitch2;
    String result;
    private ProgressDialog pDialog;
    String TAG = "good";
    Button submit;
    String url = "http://165.227.105.231/sangam/relay.php";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_relay);
        simpleSwitch1 = (Switch) findViewById(R.id.simpleSwitch1);
        simpleSwitch2 = (Switch) findViewById(R.id.simpleSwitch2);
        submit = (Button) findViewById(R.id.submitButton);
        submit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String statusSwitch1, statusSwitch2;
                if (simpleSwitch1.isChecked()&& simpleSwitch2.isChecked())
                    url="http://165.227.105.231/sangam/relay.php?one=1&two=3";
                else if (simpleSwitch1.isChecked()&& (!simpleSwitch2.isChecked()))
                    url="http://165.227.105.231/sangam/relay.php?one=1&two=2";
                else if ((!simpleSwitch1.isChecked())&& (simpleSwitch2.isChecked()))
                    url="http://165.227.105.231/sangam/relay.php?one=0&two=3";
                else if ((!simpleSwitch1.isChecked())&& (!simpleSwitch2.isChecked()))
                    url="http://165.227.105.231/sangam/relay.php?one=0&two=2";
                new relay.GetContacts().execute();
            }

        });


    }
    class GetContacts extends AsyncTask<String, Void, String> {
        final String REQUEST_METHOD = "GET";
        final int READ_TIMEOUT = 15000;
        final int CONNECTION_TIMEOUT = 15000;

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            // Showing progress dialog
            pDialog = new ProgressDialog(relay.this);
            pDialog.setMessage("Please wait...");
            pDialog.setCancelable(false);
            pDialog.show();

        }

        @Override
        protected String doInBackground(String... params) {
            String inputLine;
            URL myUrl = null;
            try {
                myUrl = new URL(url);
                Log.e(TAG, "the myURL is :: "+myUrl);
            } catch (MalformedURLException e) {
                e.printStackTrace();
            }
            try {
                assert myUrl != null;
                HttpURLConnection connection = (HttpURLConnection)
                        myUrl.openConnection();
                connection.setRequestMethod(REQUEST_METHOD);
                connection.setReadTimeout(READ_TIMEOUT);
                connection.setConnectTimeout(CONNECTION_TIMEOUT);
                connection.connect();
                InputStreamReader streamReader = new
                        InputStreamReader(connection.getInputStream());
                //Create a new buffered reader and String Builder
                BufferedReader reader = new BufferedReader(streamReader);
                StringBuilder stringBuilder = new StringBuilder();
                //Check if the line we are reading is not null
                while ((inputLine = reader.readLine()) != null) {
                    stringBuilder.append(inputLine);
                }
                //Close our InputStream and Buffered reader
                reader.close();
                streamReader.close();
                Log.d(TAG, stringBuilder.toString());
                result = stringBuilder.toString();
                Log.d(TAG, result);
            } catch (IOException e) {
                e.printStackTrace();
            }
            return result;
        }

        protected void onPostExecute(String result) {
            super.onPostExecute(result);
            super.onPostExecute(result);
            if (pDialog.isShowing())
                pDialog.dismiss();
            Toast.makeText(relay.this, result, Toast.LENGTH_LONG).show();
        }

    }
}

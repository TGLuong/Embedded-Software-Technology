package com.example.xetuhanh;

import static java.lang.Thread.*;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Bundle;
import android.os.Handler;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.google.android.material.slider.Slider;

import java.io.IOException;
import java.io.OutputStream;
import java.util.UUID;

public class MainActivity<count> extends AppCompatActivity {
  private int count=0;
  static final UUID mUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
  BluetoothAdapter ba;
  BluetoothDevice bd;
  BluetoothSocket socket;
  OutputStream out;
  private boolean tienlenState = false;
  Thread tienlen;
  @Override
  protected void onCreate(Bundle saveInstanceState){
    super.onCreate(saveInstanceState);
    setContentView(R.layout.activity_main);

    ba = BluetoothAdapter.getDefaultAdapter();
    System.out.println(ba.getBondedDevices());
    bd = ba.getRemoteDevice("00:20:10:08:3A:3F");
    System.out.println(bd.getName());
    try {
      socket = bd.createRfcommSocketToServiceRecord(mUUID);
      System.out.println(socket);
      socket.connect();
      if(socket.isConnected()){
        Toast.makeText(this, "Connected successfully!", Toast.LENGTH_SHORT).show();
      }else{
        Toast.makeText(this, "Connected fail!", Toast.LENGTH_SHORT).show();
      }
      out = socket.getOutputStream();
    } catch (IOException e) {
      e.printStackTrace();
    }

    Button upButton = findViewById(R.id.up_button);
    Button downButton = findViewById(R.id.down_button);
    Button leftButton = findViewById(R.id.left_button);
    Button rightButton = findViewById(R.id.right_button);
    Slider slider = findViewById(R.id.speed_slider);
    upButton.setOnTouchListener(new View.OnTouchListener() {
      @Override
      public boolean onTouch(View view, MotionEvent motionEvent) {
        if(!socket.isConnected()){
          Toast.makeText(MainActivity.this, "Please Connected!", Toast.LENGTH_SHORT).show();
          return false;
        }
        int mask = motionEvent.getActionMasked();
        switch (mask){
          case MotionEvent.ACTION_DOWN:
            try {
              out.write('1');
            } catch (IOException e) {
              e.printStackTrace();
            }
            break;
          case MotionEvent.ACTION_UP:
            try {
              out.write('5');
            } catch (IOException e) {
              e.printStackTrace();
            }
            break;
        }
        return false;
      }
    });
    downButton.setOnTouchListener(new View.OnTouchListener() {
      @Override
      public boolean onTouch(View view, MotionEvent motionEvent) {
        if(!socket.isConnected()){
          Toast.makeText(MainActivity.this, "Please Connected!", Toast.LENGTH_SHORT).show();
          return false;
        }
        int mask = motionEvent.getActionMasked();
        switch (mask){
          case MotionEvent.ACTION_DOWN:
            try {
              out.write('2');
            } catch (IOException e) {
              e.printStackTrace();
            }
            break;
          case MotionEvent.ACTION_UP:
            try {
              out.write('5');
            } catch (IOException e) {
              e.printStackTrace();
            }
            break;
        }
        return false;
      }
    });
    leftButton.setOnTouchListener(new View.OnTouchListener() {
      @Override
      public boolean onTouch(View view, MotionEvent motionEvent) {
        if(!socket.isConnected()){
          Toast.makeText(MainActivity.this, "Please Connected!", Toast.LENGTH_SHORT).show();
          return false;
        }
        try {
          int mask = motionEvent.getActionMasked();
          switch (mask){
            case MotionEvent.ACTION_DOWN:
              out.write('3');
              break;
            case MotionEvent.ACTION_UP:
              out.write('5');
              break;
          }
        }catch(Exception e){
          e.printStackTrace();
        }
        return false;
      }
    });
    rightButton.setOnTouchListener(new View.OnTouchListener() {
      @Override
      public boolean onTouch(View view, MotionEvent motionEvent) {
        if(!socket.isConnected()){
          Toast.makeText(MainActivity.this, "Please Connected!", Toast.LENGTH_SHORT).show();
          return false;
        }
        try{
          int mask = motionEvent.getActionMasked();
          switch (mask){
            case MotionEvent.ACTION_DOWN:
              out.write('4');
              break;
            case MotionEvent.ACTION_UP:
              out.write('5');
              break;
          }
        }catch (Exception e){
          e.printStackTrace();
        }
        return false;
      }
    });
    slider.addOnChangeListener(new Slider.OnChangeListener() {
      @Override
      public void onValueChange(@NonNull Slider slider, float value, boolean fromUser) {
        if(!socket.isConnected()){
          Toast.makeText(MainActivity.this, "Please Connected!", Toast.LENGTH_SHORT).show();
          return;
        }
        try {
          out.write((int)value);
        } catch (IOException e) {
          e.printStackTrace();
        }
      }
    });
  }

  @Override
  protected void onDestroy() {
    if(socket.isConnected()) {
      try {
        socket.close();
      } catch (IOException e) {
        e.printStackTrace();
      }
    }
    super.onDestroy();
  }
}

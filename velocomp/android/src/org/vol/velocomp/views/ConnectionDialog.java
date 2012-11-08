package org.vol.velocomp.views;

import android.app.Dialog;
import android.bluetooth.BluetoothAdapter;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import org.vol.velocomp.MainActivity;
import org.vol.velocomp.R;
import org.vol.velocomp.exceptions.*;
import org.vol.velocomp.service.BikeService;

import java.lang.ref.WeakReference;
import java.util.LinkedList;
import java.util.concurrent.TimeoutException;

public class ConnectionDialog extends Dialog {
    private static final String TAG = ConnectionDialog.class.getSimpleName();

    private class Message {
        private Exception exception;
        private String message;
        private int count;

        private Message(Exception exception, String message, int count) {
            this.exception = exception;
            this.message = message;
            this.count = count;
        }
    }

    private WeakReference<MainActivity> mainActivity;
    private LinkedList<Message> history = new LinkedList<Message>();

    private BikeService.BikeServiceListener serviceListener = new BikeService.BikeServiceListener() {
        @Override
        public void exception(BikeCommunicationException ex) {
            showDialog(ex, ex.getMessage());
        }

        @Override
        public void exception(BikeNotFoundException ex) {
            showDialog(ex, ex.getMessage());
        }

        @Override
        public void exception(BikeNotPairedException ex) {
            showDialog(ex, ex.getMessage());
        }

        @Override
        public void exception(BluetoothNotReadyException ex) {
            showDialog(ex, ex.getMessage());
        }

        @Override
        public void exception(SocketAcquiringException ex) {
            showDialog(ex, ex.getMessage());
        }

        @Override
        public void exception(SocketConnectionException ex) {
            showDialog(ex, ex.getMessage());
        }

        @Override
        public void exception(StreamConnectionException ex) {
            showDialog(ex, ex.getMessage());
        }

        @Override
        public void exception(TimeoutException ex) {
            showDialog(ex, ex.getMessage() != null ? ex.getMessage() : "Timeout");
        }

        @Override
        public void exception(InterruptedException ex) {
            //showDialog(ex, "");
        }

        @Override
        public void connect() {
            hideDialog();
        }

        @Override
        public void disconnect() {
            hideDialog();
        }
    };

    public ConnectionDialog(MainActivity mainActivity) {
        super(mainActivity);
        BikeService.getInstance().addListener(serviceListener);
        this.mainActivity = new WeakReference<MainActivity>(mainActivity);
        setContentView(R.layout.connection_dialog);
        getWindow().setLayout(ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        setTitle("Connection");
        init();
    }

    void init() {

        final BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        final Button bluetoothButton = (Button) findViewById(R.id.bluetoothButton);

        bluetoothButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (bluetoothAdapter.isEnabled()) {
                    bluetoothAdapter.disable();
                } else {
                    bluetoothAdapter.enable();
                }
            }
        });

    }

    private void showDialog(final Exception ex, String message) {

        if (mainActivity.get() == null) {
            return;
        }

        if (!history.isEmpty()) {
            if (history.getLast().message.equals(message)) {
                history.getLast().count++;
            } else {
                history.add(new Message(ex, message, 1));
            }
        } else {
            history.add(new Message(ex, message, 1));
        }
        Log.d(TAG, "Showing dialog (showDialog): " + mainActivity.get());
        mainActivity.get().runOnUiThread(new Runnable() {
            @Override
            public void run() {

                if (mainActivity.get() == null && mainActivity.get().isFinishing()) {
                    return;
                }

                LinearLayout messages = (LinearLayout) findViewById(R.id.messages);
                messages.removeAllViews();

                for (Message message : history) {
                    TextView messageField = new TextView(getContext());
                    messageField.setText(message.message);
                    messages.addView(messageField);
                }

                if (!history.isEmpty()) {
                    setTitle("Connection tryings: " + history.getLast().count);
                }

                Log.d(TAG, "Showing dialog (runOnUiThread): " + mainActivity.get());
                show();

            }
        });
    }


    private void hideDialog() {
        if (mainActivity.get() == null) {
            return;
        }


        mainActivity.get().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                hide();
                LinearLayout messages = (LinearLayout) findViewById(R.id.messages);
                messages.removeAllViews();
                history.clear();
            }
        });
    }
}

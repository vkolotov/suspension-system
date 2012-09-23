package org.vol.velocomp;

import android.content.res.Configuration;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import greendroid.app.GDActivity;
import greendroid.widget.ActionBar;
import greendroid.widget.ActionBarItem;
import greendroid.widget.SegmentedAdapter;
import greendroid.widget.SegmentedHost;
import org.vol.velocomp.messages.Telemetry;
import org.vol.velocomp.threads.BikeConnectionThread;
import org.vol.velocomp.threads.TelemetryThread;

import java.util.concurrent.TimeoutException;


public class VelocompActivity extends GDActivity {

    private ModeSegmentedAdapter modeSegmentAdapter;
    private BikeConnectionThread connectionThread;
    private TelemetryThread telemetryThread;
    private boolean isConnected;


    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setActionBarContentView(R.layout.modes_segmented_controls);
        getActionBar().setType(ActionBar.Type.Empty);
        getActionBar().setTitle(getString(R.string.app_name));
        BikeConnection.getInstance().init();
        addActionBarItem(ActionBarItem.Type.Settings);
        SegmentedHost segmentedHost = (SegmentedHost) findViewById(R.id.segmented_host);
        modeSegmentAdapter = new ModeSegmentedAdapter();
        segmentedHost.setAdapter(modeSegmentAdapter);
    }

    @Override
    public void onStart() {
        super.onStart();
        connectBike();

    }

    @Override
    protected void onResume() {
        super.onResume();
        connectBike();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onDestroy() {
        this.connectionThread.cancel();
        this.telemetryThread.cancel();
        super.onDestroy();
    }

    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);

        if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {

        } else if (newConfig.orientation == Configuration.ORIENTATION_PORTRAIT) {

        }
    }

    public void descendSuspensionToggle(View view) {

    }

    public void trailSuspensionToggle(View view) {

    }

    public void climbSuspensionToggle(View view) {

    }


    public void sendMessage(View view) {

        Telemetry telemetry = new Telemetry();

        telemetry.speed = 7;
        telemetry.cadence = 8;
        telemetry.distance = 9;
        telemetry.heartRate = 10;

        telemetry.floatField = 1234.5678f;
        telemetry.doubleField = 9876.54321;


        try {
            BikeConnection.getInstance().send((byte) 2, telemetry, 2000);
        } catch (TimeoutException e) {
            //setStatus("Timeout");
        } catch (ConnectionException e) {
            //setStatus("Connection");
        }

    }

    public boolean isConnected() {
        return isConnected;
    }


    public void setConnectionStatus(String status) {
        getActionBar().setTitle(getString(R.string.app_name) + " " + status);
    }

    public void updateTelemetry(Telemetry telemetry) {
        ((TextView) findViewById(R.id.speed)).setText(telemetry.speed);
        ((TextView) findViewById(R.id.cadence)).setText(telemetry.cadence);
    }

    public void setConnected(boolean connected) {
        isConnected = connected;
    }


    private void connectBike() {
        startConnectionThread();
        startTelemetryThread();
    }

    private void startConnectionThread() {
        if (this.connectionThread == null || this.connectionThread.isKilled()) {
            this.connectionThread = new BikeConnectionThread(this);
            this.connectionThread.start();
        }
    }

    private void startTelemetryThread() {
        if (this.telemetryThread == null || this.telemetryThread.isKilled()) {
            this.telemetryThread = new TelemetryThread(this);
            this.telemetryThread.start();
        }
    }

    private class ModeSegmentedAdapter extends SegmentedAdapter {

        @Override
        public View getView(int position, ViewGroup parent) {

            switch (position) {
                case 0:
                    return getLayoutInflater().inflate(R.layout.manual_mode, parent, false);
                case 1:
                    return getLayoutInflater().inflate(R.layout.cdt_mode, parent, false);
                case 2:
                    return getLayoutInflater().inflate(R.layout.automatic_mode, parent, false);

            }
            return null;
        }

        @Override
        public int getCount() {
            return 3;
        }

        @Override
        public String getSegmentTitle(int position) {

            switch (position) {
                case 0:
                    return getString(R.string.manualMode);
                case 1:
                    return getString(R.string.semiautomaticMode);
                case 2:
                    return getString(R.string.automaticMode);

            }

            return null;
        }
    }


}

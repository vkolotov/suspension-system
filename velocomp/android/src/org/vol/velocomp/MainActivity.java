package org.vol.velocomp;

import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.animation.AnimationUtils;
import android.widget.ViewFlipper;
import greendroid.app.GDActivity;
import greendroid.widget.ActionBar;
import greendroid.widget.ActionBarItem;
import org.vol.velocomp.service.BikeConnection;
import org.vol.velocomp.service.BikeService;
import org.vol.velocomp.threads.BikeConnectionThread;
import org.vol.velocomp.views.ConfigurationView;
import org.vol.velocomp.views.ConnectionDialog;
import org.vol.velocomp.views.Dashboard;

public class MainActivity extends GDActivity {

    private BikeConnectionThread connectionThread;
    private ActionBarItem settingsActionBarItem;
    private ActionBarItem sleepActionBarItem;
    private ActionBarItem resetActionBarItem;

    private ViewFlipper viewFlipper;
    private Dashboard dashboard;
    private ConfigurationView configurationView;

    private ConnectionDialog connectionDialog;

    private BikeService.BasicBikeServiceListener serviceListener = new BikeService.BasicBikeServiceListener() {
        @Override
        public void connect() {
            setEnabled(true);
            stopConnectionThread();
        }

        @Override
        public void disconnect() {
            setEnabled(false);
            startConnectionThread();
        }
    };

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setActionBarContentView(R.layout.main);
        getActionBar().setType(ActionBar.Type.Empty);
        getActionBar().setTitle(getString(R.string.app_name));

        sleepActionBarItem = addActionBarItem(ActionBarItem.Type.Eye, R.id.action_bar_sleep);
        resetActionBarItem = addActionBarItem(ActionBarItem.Type.Star, R.id.action_bar_reset);
        settingsActionBarItem = addActionBarItem(ActionBarItem.Type.Settings, R.id.action_bar_settings);
        viewFlipper = (ViewFlipper) findViewById(R.id.flipper);
        dashboard = (Dashboard) findViewById(R.id.dashboard);
        configurationView = (ConfigurationView) findViewById(R.id.configuration_view);
        connectionDialog = new ConnectionDialog(this);

        connectionDialog.show();

        getActionBar().setOnActionBarListener( new ActionBar.OnActionBarListener() {
            @Override
            public void onActionBarItemClicked(int position) {
                if (position == ActionBar.OnActionBarListener.HOME_ITEM) {
                    showDashboard();
                } else if (getActionBar().getItem(position) == settingsActionBarItem) {
                    showConfiguration();
                } else if (getActionBar().getItem(position) == sleepActionBarItem) {
                    BikeService.getInstance().sleep();
                } else if (getActionBar().getItem(position) == resetActionBarItem) {
                    BikeService.getInstance().reset();
                }
            }
        });

    }

    @Override
    public void onStart() {
        super.onStart();
        connectBike();
        BikeService.getInstance().addListener(serviceListener);
    }

    @Override
    protected void onResume() {
        super.onResume();
        connectBike();
        BikeService.getInstance().addListener(serviceListener);
    }

    @Override
    protected void onDestroy() {
        if (this.connectionThread != null) {
            BikeService.getInstance().removeListener(serviceListener);
            stopConnectionThread();
            try {
                this.connectionThread.join();
            } catch (InterruptedException e) {}
        }
        BikeService.getInstance().tearDown();
        BikeConnection.getInstance().tearDown();
        super.onDestroy();
    }

    private void connectBike() {
        startConnectionThread();
    }

    private void startConnectionThread() {
        if (this.connectionThread == null || this.connectionThread.isKilled()) {
            this.connectionThread = new BikeConnectionThread();
            this.connectionThread.start();
        }
    }

    private void stopConnectionThread() {
        if (this.connectionThread != null) {
            this.connectionThread.cancel();
            this.connectionThread.interrupt();
        }
    }

    private void showDashboard() {

        configurationView.hide();

        viewFlipper.setAnimation(AnimationUtils.loadAnimation(this, R.anim.gd_grow_from_topleft_to_bottomright));
        viewFlipper.showNext();

        getActionBar().setType(ActionBar.Type.Empty);
        getActionBar().addItem(settingsActionBarItem);
    }

    private void showConfiguration() {

        viewFlipper.setAnimation(AnimationUtils.loadAnimation(this, R.anim.gd_grow_from_topright_to_bottomleft));
        viewFlipper.showNext();
        configurationView.init();

        getActionBar().removeItem(settingsActionBarItem);
        getActionBar().setType(ActionBar.Type.Normal);

    }

    public boolean onKeyDown(int keyCode, KeyEvent event) {

        if (viewFlipper.getCurrentView() != dashboard && KeyEvent.KEYCODE_BACK == keyCode) {
            showDashboard();
            return false;
        } else if (viewFlipper.getCurrentView() == dashboard && KeyEvent.KEYCODE_BACK == keyCode) {

        }
        return super.onKeyDown(keyCode, event);
    }

    private void setEnabled(final boolean isEnabled) {
        runOnUiThread( new Runnable() {
            @Override
            public void run() {
                if (settingsActionBarItem != null) {
                    settingsActionBarItem.getItemView().setVisibility(isEnabled ? View.VISIBLE : View.GONE);
                }
                if (sleepActionBarItem != null) {
                    sleepActionBarItem.getItemView().setVisibility(isEnabled ? View.VISIBLE : View.GONE);
                }

                if (resetActionBarItem != null) {
                    resetActionBarItem.getItemView().setVisibility(isEnabled ? View.VISIBLE : View.GONE);
                }
                getActionBar().setTitle(getString(R.string.app_name) + (isEnabled ? " connected " : " disconnected"));
                for (int i = 0; i < dashboard.getSegmentedBar().getChildCount(); i++) {
                    dashboard.getSegmentedBar().getChildAt(i).setEnabled(isEnabled);
                }
            }
        });

    }

}

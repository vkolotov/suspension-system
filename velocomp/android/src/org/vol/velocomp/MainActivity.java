package org.vol.velocomp;

import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.animation.AnimationUtils;
import android.widget.ViewFlipper;
import greendroid.app.GDActivity;
import greendroid.widget.ActionBar;
import greendroid.widget.ActionBarItem;
import org.vol.velocomp.service.BikeService;
import org.vol.velocomp.threads.BikeConnectionThread;
import org.vol.velocomp.views.ConfigurationView;
import org.vol.velocomp.views.Dashboard;

public class MainActivity extends GDActivity {

    private BikeConnectionThread connectionThread;
    private ActionBarItem settingsActionBarItem;
    private ActionBarItem sleepActionBarItem;
    private ActionBarItem resetActionBarItem;

    private ViewFlipper viewFlipper;
    private Dashboard dashboard;
    private ConfigurationView configurationView;

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
        BikeService.getInstance().addListener(new BikeService.BikeServiceListener() {
            @Override
            public void onDisconnected(Exception ex) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        if (settingsActionBarItem != null) {
                            settingsActionBarItem.getItemView().setVisibility(View.GONE);
                        }
                        if (sleepActionBarItem != null) {
                            sleepActionBarItem.getItemView().setVisibility(View.GONE);
                        }

                        if (resetActionBarItem != null) {
                            resetActionBarItem.getItemView().setVisibility(View.GONE);
                        }
                        getActionBar().setTitle(getString(R.string.app_name) + " disconnected");
                        for (int i = 0; i < dashboard.getSegmentedBar().getChildCount(); i++) {
                            dashboard.getSegmentedBar().getChildAt(i).setEnabled(false);
                        }
                    }
                });
            }

            @Override
            public void onConnected() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        if (settingsActionBarItem != null) {
                            settingsActionBarItem.getItemView().setVisibility(View.VISIBLE);
                        }
                        if (sleepActionBarItem != null) {
                            sleepActionBarItem.getItemView().setVisibility(View.VISIBLE);
                        }
                        if (resetActionBarItem != null) {
                            resetActionBarItem.getItemView().setVisibility(View.VISIBLE);
                        }
                        getActionBar().setTitle(getString(R.string.app_name) + " connected");
                        for (int i = 0; i < dashboard.getSegmentedBar().getChildCount(); i++) {
                            dashboard.getSegmentedBar().getChildAt(i).setEnabled(true);
                        }
                    }
                });
            }
        });
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
    }

    @Override
    protected void onResume() {
        super.onResume();
        connectBike();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (this.connectionThread != null) {
            this.connectionThread.cancel();
        }
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
        }
        return super.onKeyDown(keyCode, event);
    }

}

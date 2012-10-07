package org.vol.velocomp;

import android.os.Bundle;
import android.view.KeyEvent;
import android.view.animation.AnimationUtils;
import android.widget.ViewFlipper;
import greendroid.app.GDActivity;
import greendroid.widget.ActionBar;
import greendroid.widget.ActionBarItem;
import org.vol.velocomp.service.BikeService;
import org.vol.velocomp.threads.BikeConnectionThread;
import org.vol.velocomp.views.ConfigurationView;

public class MainActivity extends GDActivity {

    private BikeConnectionThread connectionThread;
    private ActionBarItem settingsActionBarItem;


    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setActionBarContentView(R.layout.main);
        getActionBar().setType(ActionBar.Type.Empty);
        getActionBar().setTitle(getString(R.string.app_name));
        settingsActionBarItem = addActionBarItem(ActionBarItem.Type.Settings, R.id.action_bar_settings);
        BikeService.getInstance().setListener(new BikeService.BikeServiceListener() {
            @Override
            public void onDisconnected(Exception ex) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        getActionBar().setTitle(getString(R.string.app_name) + " disconnected");
                    }
                });
            }

            @Override
            public void onConnected() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        getActionBar().setTitle(getString(R.string.app_name) + " connected");
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
        ViewFlipper viewFlipper = (ViewFlipper) findViewById(R.id.flipper);
        viewFlipper.setAnimation(AnimationUtils.loadAnimation(this, R.anim.gd_grow_from_topleft_to_bottomright));
        viewFlipper.showNext();

        getActionBar().setType(ActionBar.Type.Empty);
        getActionBar().addItem(settingsActionBarItem);

    }

    private void showConfiguration() {
        ViewFlipper viewFlipper = (ViewFlipper) findViewById(R.id.flipper);
        viewFlipper.setAnimation(AnimationUtils.loadAnimation(this, R.anim.gd_grow_from_topleft_to_bottomright));
        viewFlipper.showNext();
        ((ConfigurationView) viewFlipper.getCurrentView().findViewById(R.id.configuration_view)).init();

        getActionBar().removeItem(settingsActionBarItem);
        getActionBar().setType(ActionBar.Type.Normal);

    }

    public boolean onKeyDown(int keyCode, KeyEvent event) {
        return super.onKeyDown(keyCode, event);
    }

}

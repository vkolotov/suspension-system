package org.vol.velocomp.views;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.ScrollView;
import android.widget.SeekBar;
import org.vol.velocomp.R;
import org.vol.velocomp.messages.Configuration;

public class PowerSaveSystemConfiguration extends ScrollView {

    private SeekBarSectionView sleepTimeout;
    private CheckBox sleepCheckbox;
    private SeekBarSectionView bluetoothSleepTimeout;
    private CheckBox bluetoothSleepCheckbox;

    public PowerSaveSystemConfiguration(Context context) {
        super(context);
    }

    public PowerSaveSystemConfiguration(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public PowerSaveSystemConfiguration(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();
        this.sleepTimeout = (SeekBarSectionView) findViewById(R.id.sleep_timeout);
        this.sleepCheckbox = (CheckBox) findViewById(R.id.sleep_checkbox);
        this.bluetoothSleepTimeout = (SeekBarSectionView) findViewById(R.id.bluetooth_sleep_timeout);
        this.bluetoothSleepCheckbox = (CheckBox) findViewById(R.id.bluetooth_sleep_checkbox);
    }

    public void setConfiguration(final Configuration.PowerSaveSystemConfig configuration) {
        this.sleepCheckbox.setChecked(configuration.isSleepEnabled);
        this.sleepCheckbox.setOnCheckedChangeListener( new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                configuration.isSleepEnabled = b;
            }
        });

        this.sleepTimeout.setTitle("Minutes");
        this.sleepTimeout.setValue(configuration.sleepTimeout / 60000);
        this.sleepTimeout.setProgress(configuration.sleepTimeout / 60000);
        this.sleepTimeout.setMaxValue(10);
        this.sleepTimeout.setOnSeekBarChangeListener( new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                sleepTimeout.setValue(i);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                configuration.sleepTimeout = seekBar.getProgress() * 60000;
            }
        });


        this.bluetoothSleepCheckbox.setChecked(configuration.isBluetoothSleepEnabled);
        this.bluetoothSleepCheckbox.setOnCheckedChangeListener( new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                configuration.isBluetoothSleepEnabled = b;
            }
        });

        this.bluetoothSleepTimeout.setTitle("Minutes");
        this.bluetoothSleepTimeout.setValue(configuration.bluetoothSleepTimeout / 60000);
        this.bluetoothSleepTimeout.setProgress(configuration.bluetoothSleepTimeout / 60000);
        this.bluetoothSleepTimeout.setMaxValue(10);
        this.bluetoothSleepTimeout.setOnSeekBarChangeListener( new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                bluetoothSleepTimeout.setValue(i);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                configuration.bluetoothSleepTimeout = seekBar.getProgress() * 60000;
            }
        });

    }


}

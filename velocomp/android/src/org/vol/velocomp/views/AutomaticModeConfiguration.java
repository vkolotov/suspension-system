package org.vol.velocomp.views;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.ScrollView;
import android.widget.SeekBar;
import org.vol.velocomp.R;
import org.vol.velocomp.messages.Configuration;

public class AutomaticModeConfiguration extends ScrollView {

    private SeekBarSectionView bumpsPer10Meters;
    private SeekBarSectionView measuringPeriod;
    private SeekBarSectionView maxUnlockTimeout;


    public AutomaticModeConfiguration(Context context) {
        super(context);
    }

    public AutomaticModeConfiguration(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public AutomaticModeConfiguration(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();
        this.bumpsPer10Meters = (SeekBarSectionView) findViewById(R.id.bumps);
        this.measuringPeriod = (SeekBarSectionView) findViewById(R.id.measuringPeriod);
        this.maxUnlockTimeout = (SeekBarSectionView) findViewById(R.id.maxUnlockTimeout);
    }

    public void setConfig(final Configuration config) {
        Utils.initSeekBarSectionView(bumpsPer10Meters, "Max bumps per 10 meters", 40,
                (int) (config.system.maxBumpsPerMeter * 10), new Utils.AbstractSeekBarListener() {
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                config.system.maxBumpsPerMeter = seekBar.getProgress() / 10;
            }
        });
        Utils.initSeekBarSectionView(measuringPeriod, "Measuring period (secs)", 15,
                config.unsprungAccelerometerSystem.measuringPeriod / 1000, new Utils.AbstractSeekBarListener() {
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                config.unsprungAccelerometerSystem.measuringPeriod = (short) (seekBar.getProgress() * 1000);
            }
        });
        Utils.initSeekBarSectionView(maxUnlockTimeout, "Max unlock timeout (secs)", 15,
                config.system.maxUnlockTimeout / 1000, new Utils.AbstractSeekBarListener() {
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                config.system.maxUnlockTimeout = (short) (seekBar.getProgress() * 1000);
            }
        });
    }


}

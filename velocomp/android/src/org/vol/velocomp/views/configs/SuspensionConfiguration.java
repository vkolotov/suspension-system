package org.vol.velocomp.views.configs;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.SeekBar;
import org.vol.velocomp.R;
import org.vol.velocomp.messages.Configuration;
import org.vol.velocomp.service.BikeService;
import org.vol.velocomp.views.controls.SeekBarSectionView;

public class SuspensionConfiguration extends ScrollView {

    public SuspensionConfiguration(Context context) {
        super(context);
    }

    public SuspensionConfiguration(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public SuspensionConfiguration(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();
    }

    public void setSuspensionSystemConfiguration(final Configuration.SuspensionSystemConfig suspensionSystemConfig) {
        LayoutInflater inflater = (LayoutInflater) getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        LinearLayout linearLayout = (LinearLayout) findViewById(R.id.suspension_configuration_content);

        for (int i = 0; i < suspensionSystemConfig.modes; i++) {

            final SeekBarSectionView seekBarSectionView = (SeekBarSectionView) inflater.inflate(R.layout.seekbar_section, this, false);

            seekBarSectionView.setTitle(getModeTitle(suspensionSystemConfig, i));
            seekBarSectionView.setValue(suspensionSystemConfig.angles[i] - suspensionSystemConfig.minAngle);
            seekBarSectionView.setMaxValue(suspensionSystemConfig.maxAngle - suspensionSystemConfig.minAngle);
            seekBarSectionView.setProgress(suspensionSystemConfig.angles[i] - suspensionSystemConfig.minAngle);

            final int mode = i;
            seekBarSectionView.setOnSeekBarChangeListener( new SeekBar.OnSeekBarChangeListener() {
                @Override
                public void onProgressChanged(SeekBar seekBar, int value, boolean fromUser) {
                    seekBarSectionView.setValue(value - suspensionSystemConfig.minAngle);
                }

                @Override
                public void onStartTrackingTouch(SeekBar seekBar) {
                    suspensionSystemConfig.mode = (byte) mode;
                }

                @Override
                public void onStopTrackingTouch(SeekBar seekBar) {
                    suspensionSystemConfig.angles[mode] = (short) (suspensionSystemConfig.minAngle + seekBar.getProgress());

                    BikeService.getInstance().sendFrontSuspensionConfiguration(suspensionSystemConfig);

                }
            });

            linearLayout.addView(seekBarSectionView, new LinearLayout.LayoutParams(LinearLayout.LayoutParams.FILL_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT));
        }

    }

    private String getModeTitle(Configuration.SuspensionSystemConfig suspensionSystemConfig, int mode) {
        if (suspensionSystemConfig.modes == 3) {
            return mode == 0 ? "Climb angle" : (mode == 1 ? "Trail angle" : "Descent angle");
        } else if (suspensionSystemConfig.modes == 2) {
            return mode == 0 ? "Climb & Trail angle" : "Descent angle";
        } else {
            return "Mode #" + mode + " angle";
        }
    }
}

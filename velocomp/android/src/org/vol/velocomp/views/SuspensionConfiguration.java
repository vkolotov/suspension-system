package org.vol.velocomp.views;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.TextView;
import org.vol.velocomp.R;
import org.vol.velocomp.messages.Configuration;
import org.vol.velocomp.service.BikeService;

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

            View seekBarSection = inflater.inflate(R.layout.seekbar_section, this, false);

            ((TextView) seekBarSection.findViewById(R.id.seekBarLabel)).setText("Mode #" + (i + 1));
            ((TextView) seekBarSection.findViewById(R.id.seekBarMinValue)).setText(
                    String.valueOf(suspensionSystemConfig.minAngle));
            ((TextView) seekBarSection.findViewById(R.id.seekBarMaxValue)).setText(
                    String.valueOf(suspensionSystemConfig.maxAngle));

            SeekBar seekBar = (SeekBar) seekBarSection.findViewById(R.id.seekBar);

            seekBar.setMax(suspensionSystemConfig.maxAngle - suspensionSystemConfig.minAngle);
            seekBar.setProgress(suspensionSystemConfig.angles[i] - suspensionSystemConfig.minAngle);

            final int mode = i;

            seekBar.setOnSeekBarChangeListener( new SeekBar.OnSeekBarChangeListener() {
                @Override
                public void onProgressChanged(SeekBar seekBar, int value, boolean fromUser) {
                }

                @Override
                public void onStartTrackingTouch(SeekBar seekBar) {
                }

                @Override
                public void onStopTrackingTouch(SeekBar seekBar) {
                    suspensionSystemConfig.angles[mode] = (short) (suspensionSystemConfig.minAngle + seekBar.getProgress());

                    BikeService.getInstance().sendFrontSuspensionConfiguration(suspensionSystemConfig);

                }
            });

            linearLayout.addView(seekBarSection, new LinearLayout.LayoutParams(LinearLayout.LayoutParams.FILL_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT));
        }

    }
}

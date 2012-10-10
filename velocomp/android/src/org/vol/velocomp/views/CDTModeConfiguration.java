package org.vol.velocomp.views;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.ScrollView;
import android.widget.SeekBar;
import org.vol.velocomp.R;
import org.vol.velocomp.messages.Configuration;

public class CDTModeConfiguration extends ScrollView {

    private SeekBarSectionView transitionGap;

    public CDTModeConfiguration(Context context) {
        super(context);
    }

    public CDTModeConfiguration(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public CDTModeConfiguration(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();
        this.transitionGap = (SeekBarSectionView) findViewById(R.id.transition_gap);
    }

    public void setSemiautomaticStateConfig(final Configuration.SemiautomaticStateConfig semiautomaticStateConfig) {
        this.transitionGap.setMaxValue(10);
        this.transitionGap.setValue((int) (semiautomaticStateConfig.transitionGap * 180.0f / Math.PI));
        this.transitionGap.setProgress((int) (semiautomaticStateConfig.transitionGap * 180.0f / Math.PI));
        this.transitionGap.setTitle("Transition gap (degrees)");
        this.transitionGap.setOnSeekBarChangeListener( new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                transitionGap.setValue(i);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                semiautomaticStateConfig.transitionGap = (float) (seekBar.getProgress() * Math.PI / 180.0f);
            }
        });
    }
}

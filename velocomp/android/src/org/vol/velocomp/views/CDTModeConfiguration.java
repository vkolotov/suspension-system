package org.vol.velocomp.views;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.SeekBar;
import org.vol.velocomp.R;
import org.vol.velocomp.messages.Configuration;

public class CDTModeConfiguration extends ScrollView {

    private SeekBarSectionView transitionGap;
    private EditText alphaFactor;
    private EditText betaFactor;

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
        this.alphaFactor = (EditText) findViewById(R.id.alphaFactor);
        this.betaFactor = (EditText) findViewById(R.id.betaFactor);
    }

    public void setConfig(final Configuration configuration) {
        this.transitionGap.setMaxValue(10);
        this.transitionGap.setValue((int) (configuration.semiautomaticStateConfig.transitionGap * 180.0f / Math.PI));
        this.transitionGap.setProgress((int) (configuration.semiautomaticStateConfig.transitionGap * 180.0f / Math.PI));
        this.transitionGap.setTitle("Transition gap (degrees)");
        this.transitionGap.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                transitionGap.setValue(i);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                configuration.semiautomaticStateConfig.transitionGap = (float) (seekBar.getProgress() * Math.PI / 180.0f);
            }
        });
        this.alphaFactor.setText(String.valueOf(configuration.sprungAccelerometerSystem.angleFilterAlpha));
        this.alphaFactor.setOnFocusChangeListener( new OnFocusChangeListener() {
            @Override
            public void onFocusChange(View view, boolean b) {
                configuration.sprungAccelerometerSystem.angleFilterAlpha = Float.parseFloat(alphaFactor.getText().toString());
            }
        });
        this.betaFactor.setText(String.valueOf(configuration.sprungAccelerometerSystem.angleFilterBeta));
        this.betaFactor.setOnFocusChangeListener( new OnFocusChangeListener() {
            @Override
            public void onFocusChange(View view, boolean b) {
                configuration.sprungAccelerometerSystem.angleFilterBeta = Float.parseFloat(betaFactor.getText().toString());
            }
        });
    }
}

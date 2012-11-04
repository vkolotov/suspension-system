package org.vol.velocomp.views.controls;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;
import org.vol.velocomp.R;

public class SeekBarSectionView extends RelativeLayout {

    private TextView seekBarLabel;
    private TextView seekBarValue;
    private SeekBar seekBar;

    public SeekBarSectionView(Context context) {
        super(context);
    }

    public SeekBarSectionView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public SeekBarSectionView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();

        this.seekBar = (SeekBar) findViewById(R.id.seekBar);
        this.seekBarLabel = (TextView) findViewById(R.id.seekBarLabel);
        this.seekBarValue = (TextView) findViewById(R.id.seekBarValue);
    }

    public void setMaxValue(int max) {
        this.seekBar.setMax(max);
    }

    public void setValue(int value) {
        this.seekBarValue.setText(String.valueOf(value));
    }

    public void setTitle(String title) {
        this.seekBarLabel.setText(title);
    }

    public void setProgress(int value) {
        this.seekBar.setProgress(value);
    }

    public void setSecondaryProgress(int value) {
        this.seekBar.setSecondaryProgress(value);
    }

    public void setOnSeekBarChangeListener(SeekBar.OnSeekBarChangeListener onSeekBarChangeListener) {
        this.seekBar.setOnSeekBarChangeListener(onSeekBarChangeListener);
    }
}

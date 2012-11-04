package org.vol.velocomp.views.controls;

import android.content.Context;
import android.content.res.TypedArray;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;
import org.vol.velocomp.R;

public class SeekBarConfig extends RelativeLayout implements SeekBar.OnSeekBarChangeListener {

    public static class AbstractOnSeekBarChangeListener implements SeekBar.OnSeekBarChangeListener {
        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {

        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {

        }

        @Override
        public void onProgressChanged(SeekBar seekBar, int i, boolean b) {

        }
    }

    private TextView title;
    private TextView value;
    private SeekBar seekBar;

    private SeekBar.OnSeekBarChangeListener externalListener;

    public SeekBarConfig(Context context) {
        super(context);
        inflateContent();
    }

    public SeekBarConfig(Context context, AttributeSet attrs) {
        super(context, attrs);
        inflateContent();
        init(attrs);
    }

    public SeekBarConfig(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        inflateContent();
        init(attrs);
    }

    private void inflateContent() {
        LayoutInflater inflater = (LayoutInflater) getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        inflater.inflate(R.layout.seekbar_config, this);
    }

    private void init(AttributeSet attrs) {

        this.title = (TextView) findViewById(R.id.seekBarLabel);
        this.value = (TextView) findViewById(R.id.seekBarValue);
        this.seekBar = (SeekBar) findViewById(R.id.seekBar);

        TypedArray typedArray =
                this.getContext().obtainStyledAttributes(attrs, R.styleable.SeekBarConfig);

        String label = typedArray.getString(R.styleable.SeekBarConfig_title);
        Integer value = typedArray.getInteger(R.styleable.SeekBarConfig_value, 0);
        Integer secondaryValue = typedArray.getInteger(R.styleable.SeekBarConfig_secondaryValue, 0);
        Integer maxValue = typedArray.getInteger(R.styleable.SeekBarConfig_maxValue, 100);

        this.title.setText(label);
        this.value.setText(String.valueOf(value));
        this.seekBar.setMax(maxValue);
        this.seekBar.setProgress(value);
        this.seekBar.setSecondaryProgress(secondaryValue);
        this.seekBar.setOnSeekBarChangeListener(this);
        typedArray.recycle();
    }

    public void setMaxValue(int max) {
        this.seekBar.setMax(max);
    }

    public void setValue(int value) {
        this.value.setText(String.valueOf(value));
        this.seekBar.setProgress(Math.abs(value));
    }

    public void setTitle(String title) {
        this.title.setText(title);
    }

    public void setSecondaryProgress(int value) {
        this.seekBar.setSecondaryProgress(value);
    }

    public void setOnSeekBarChangeListener(SeekBar.OnSeekBarChangeListener onSeekBarChangeListener) {
        this.externalListener = onSeekBarChangeListener;
    }

    public int getValue() {
        return Integer.parseInt(this.value.getText().toString());
    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
        int v = getValue() >= 0 ?  i : -i;
        value.setText(String.valueOf(v));
        if (externalListener != null) {
            externalListener.onProgressChanged(seekBar, v, b);
        }
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {
        if (externalListener != null) {
            externalListener.onStartTrackingTouch(seekBar);
        }
    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {
        if (externalListener != null) {
            externalListener.onStopTrackingTouch(seekBar);
        }
    }

    @Override
    public void setEnabled(boolean enabled) {
        super.setEnabled(enabled);
        this.seekBar.setEnabled(enabled);
    }
}

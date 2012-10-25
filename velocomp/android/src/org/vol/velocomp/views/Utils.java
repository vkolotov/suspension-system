package org.vol.velocomp.views;

import android.view.View;
import android.widget.SeekBar;
import android.widget.TextView;

public class Utils {

    public static void init(TextView textView, Object value) {
        if (value != null && textView != null) {
            textView.setText(value.toString());
        }
    }

    public static Indicator initIndicator(View parent, int id, String label) {
        Indicator result = (Indicator) parent.findViewById(id);
        if (result != null) {
            result.setLabel(label);
        }
        return result;
    }

    public static void initSeekBarSectionView(final SeekBarSectionView seekBarSectionView, String title,
                                              int maxValue, int value, final AbstractSeekBarListener seekBarListener) {
        seekBarSectionView.setMaxValue(maxValue);
        seekBarSectionView.setValue(value);
        seekBarSectionView.setProgress(value);
        seekBarSectionView.setTitle(title);
        seekBarSectionView.setOnSeekBarChangeListener( new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                seekBarSectionView.setValue(i);
                seekBarListener.onProgressChanged(seekBar, i, b);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                seekBarListener.onStartTrackingTouch(seekBar);
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                seekBarListener.onStopTrackingTouch(seekBar);
            }
        });
    }

    public static class AbstractSeekBarListener implements SeekBar.OnSeekBarChangeListener {

        @Override
        public void onProgressChanged(SeekBar seekBar, int i, boolean b) {

        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {

        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {

        }
    }
}

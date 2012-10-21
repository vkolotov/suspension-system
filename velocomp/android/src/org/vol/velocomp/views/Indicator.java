package org.vol.velocomp.views;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.RelativeLayout;
import android.widget.TextView;
import org.vol.velocomp.R;

public class Indicator extends RelativeLayout {

    private TextView label;
    private TextView value;

    public Indicator(Context context) {
        super(context);
    }

    public Indicator(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public Indicator(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();

        this.label = (TextView) findViewById(R.id.indicatorLabel);
        this.value = (TextView) findViewById(R.id.indicatorValue);
    }

    public void setLabel(String label) {
        this.label.setText(label);
    }

    public void setValue(int value) {
        this.value.setText(String.valueOf(value));
    }

    public void setValue(String value) {
        this.value.setText(value);
    }

}

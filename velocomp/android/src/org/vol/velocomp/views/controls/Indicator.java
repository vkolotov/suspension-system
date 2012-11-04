package org.vol.velocomp.views.controls;

import android.content.Context;
import android.content.res.TypedArray;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.widget.RelativeLayout;
import android.widget.TextView;
import org.vol.velocomp.R;
import org.vol.velocomp.views.Utils;

public class Indicator extends RelativeLayout {

    private TextView labelField;
    private TextView valueField;

    public Indicator(Context context) {
        super(context);
    }

    public Indicator(Context context, AttributeSet attrs) {
        super(context, attrs);
        inflateContent();
        init(attrs);
    }

    public Indicator(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        inflateContent();
        init(attrs);
    }

    public void setLabel(String label) {
        this.labelField.setText(label);
    }

    public void setValue(int valueField) {
        this.valueField.setText(String.valueOf(valueField));
    }

    public void setValue(String value) {
        this.valueField.setText(value);
    }

    private void init(AttributeSet attrs) {

        this.labelField = (TextView) findViewById(R.id.indicatorLabel);
        this.valueField = (TextView) findViewById(R.id.indicatorValue);

        TypedArray typedArray =
                this.getContext().obtainStyledAttributes(attrs, R.styleable.Indicator);

        String label = typedArray.getString(R.styleable.Indicator_label);
        String value = typedArray.getString(R.styleable.Indicator_valueStr);

        Utils.init(labelField, label);
        Utils.init(valueField, value);

        typedArray.recycle();
    }

    private void inflateContent() {
        LayoutInflater inflater = (LayoutInflater) getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        inflater.inflate(R.layout.indicator, this);
    }

}

package org.vol.velocomp.views;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.CheckBox;
import com.cyrilmottier.android.greendroid.R;
import greendroid.widget.SegmentedHost;
import org.vol.velocomp.adapters.ModesAdapter;

public class Dashboard extends SegmentedHost {

    private ModesAdapter modesAdapter = new ModesAdapter(this);

    public Dashboard(Context context) {
        this(context, null);
    }

    public Dashboard(Context context, AttributeSet attrs) {
        this(context, attrs, R.attr.gdSegmentedHostStyle);
    }

    public Dashboard(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    protected void onFinishInflate() {
        super.onFinishInflate();
        setAdapter(modesAdapter);
    }

    public void setBoard(Mode mode) {
        CheckBox checkBox = (CheckBox) getSegmentedBar().getChildSegmentAt(mode.getBoard());
        checkBox.performClick();
    }

}

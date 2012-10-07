package org.vol.velocomp.views;

import android.content.Context;
import android.util.AttributeSet;
import com.cyrilmottier.android.greendroid.R;
import greendroid.widget.SegmentedHost;
import org.vol.velocomp.adapters.ModesAdapter;

public class Dashboard extends SegmentedHost {

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
        setAdapter(new ModesAdapter(this));
    }

}

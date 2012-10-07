package org.vol.velocomp.adapters;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import greendroid.widget.SegmentedAdapter;
import org.vol.velocomp.R;
import org.vol.velocomp.views.Dashboard;

public class ModesAdapter extends SegmentedAdapter {

    private Dashboard dashboard;

    public ModesAdapter(Dashboard dashboard) {
        this.dashboard = dashboard;
    }

    @Override
    public View getView(int position, ViewGroup parent) {

        LayoutInflater inflater = (LayoutInflater) parent.getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        switch (position) {
            case 0:
                return inflater.inflate(R.layout.manual_mode, parent, false);
            case 1:
                return inflater.inflate(R.layout.cdt_mode, parent, false);
            case 2:
                return inflater.inflate(R.layout.automatic_mode, parent, false);

        }
        return null;
    }

    @Override
    public int getCount() {
        return 3;
    }

    @Override
    public String getSegmentTitle(int position) {

        switch (position) {
            case 0:
                return dashboard.getContext().getString(R.string.manualMode);
            case 1:
                return dashboard.getContext().getString(R.string.semiautomaticMode);
            case 2:
                return dashboard.getContext().getString(R.string.automaticMode);

        }

        return null;
    }

}

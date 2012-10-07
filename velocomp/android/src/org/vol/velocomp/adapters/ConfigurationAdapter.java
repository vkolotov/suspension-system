package org.vol.velocomp.adapters;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import greendroid.widget.PagedAdapter;
import org.vol.velocomp.R;
import org.vol.velocomp.messages.Configuration;
import org.vol.velocomp.views.SuspensionConfiguration;

public class ConfigurationAdapter extends PagedAdapter {

    public static final int PAGE_COUNT = 1;
    public static final int PAGE_MAX_INDEX = PAGE_COUNT - 1;

    private final Configuration configuration;

    public ConfigurationAdapter(Configuration configuration) {
        this.configuration = configuration;
    }

    @Override
    public int getCount() {
        return PAGE_COUNT;
    }

    @Override
    public Object getItem(int position) {
        return null;
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }

    public View getView(int position, View convertView, ViewGroup parent) {

        switch (position) {
            case 0:
                return getSuspensionView(parent, "Front suspension", configuration.frontSuspension);

        }
        return null;
    }

    private View getSuspensionView(ViewGroup parent, String name, final Configuration.SuspensionSystemConfig suspensionSystemConfig) {
        LayoutInflater inflater = (LayoutInflater) parent.getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        SuspensionConfiguration suspensionConfiguration = (SuspensionConfiguration)
                inflater.inflate(R.layout.suspension_configuration, parent, false);

        suspensionConfiguration.setSuspensionSystemConfiguration(suspensionSystemConfig);
        return suspensionConfiguration;
    }

}

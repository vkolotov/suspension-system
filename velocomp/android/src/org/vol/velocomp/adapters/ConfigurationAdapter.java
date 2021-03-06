package org.vol.velocomp.adapters;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import greendroid.widget.PagedAdapter;
import org.vol.velocomp.R;
import org.vol.velocomp.messages.Configuration;
import org.vol.velocomp.views.configs.AutomaticModeConfiguration;
import org.vol.velocomp.views.configs.CDTModeConfiguration;
import org.vol.velocomp.views.configs.PowerSaveSystemConfiguration;
import org.vol.velocomp.views.configs.SuspensionConfiguration;

public class ConfigurationAdapter extends PagedAdapter {

    public static final int PAGE_COUNT = 5;
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
                return getSuspensionView(parent, "Front Suspension Configuration", configuration.frontSuspension);
            case 1:
                return getSuspensionView(parent, "Rear Suspension Configuration", configuration.rearSuspension);
            case 2:
                return getCDTMode(parent);
            case 3:
                return getAutomaticModeView(parent);
            case 4:
                return getPowerSaveView(parent);



        }
        return null;
    }

    private View getSuspensionView(ViewGroup parent, String name, final Configuration.SuspensionSystemConfig suspensionSystemConfig) {
        LayoutInflater inflater = (LayoutInflater) parent.getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        SuspensionConfiguration suspensionConfiguration = (SuspensionConfiguration)
                inflater.inflate(R.layout.suspension_configuration, parent, false);

        ((TextView) suspensionConfiguration.findViewById(R.id.config_title)).setText(name);

        suspensionConfiguration.setSuspensionSystemConfiguration(suspensionSystemConfig);
        return suspensionConfiguration;
    }

    private View getCDTMode(ViewGroup parent) {
        LayoutInflater inflater = (LayoutInflater) parent.getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        CDTModeConfiguration cdtModeConfiguration = (CDTModeConfiguration)
                inflater.inflate(R.layout.cdt_mode_configuration, parent, false);

        cdtModeConfiguration.setConfig(configuration);

        return cdtModeConfiguration;
    }

    private View getAutomaticModeView(ViewGroup parent) {
        LayoutInflater inflater = (LayoutInflater) parent.getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        AutomaticModeConfiguration cdtModeConfiguration = (AutomaticModeConfiguration)
                inflater.inflate(R.layout.automatic_mode_configuration, parent, false);

        cdtModeConfiguration.setConfig(configuration);

        return cdtModeConfiguration;
    }

    private View getPowerSaveView(ViewGroup parent) {
        LayoutInflater inflater = (LayoutInflater) parent.getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        PowerSaveSystemConfiguration powerSaveSystemConfiguration = (PowerSaveSystemConfiguration)
                inflater.inflate(R.layout.powe_save_configuration, parent, false);

        powerSaveSystemConfiguration.setConfiguration(configuration.powerSave);

        return powerSaveSystemConfiguration;
    }

}

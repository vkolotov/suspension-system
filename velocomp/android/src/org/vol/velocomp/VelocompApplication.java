package org.vol.velocomp;

import android.content.Intent;
import android.net.Uri;
import greendroid.app.GDApplication;

public class VelocompApplication extends GDApplication {

    @Override
    public Class<?> getHomeActivityClass() {
        return VelocompActivity.class;
    }

    @Override
    public Intent getMainApplicationIntent() {
        return new Intent(Intent.ACTION_VIEW, Uri.parse(getString(R.string.app_url)));
    }

}

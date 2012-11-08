package org.vol.velocomp;

import android.content.Intent;
import android.net.Uri;
import greendroid.app.GDApplication;

public class VelocompApplication extends GDApplication {

    private static final String TAG = VelocompApplication.class.getSimpleName();

    @Override
    public Class<?> getHomeActivityClass() {
        return MainActivity.class;
    }

    @Override
    public Intent getMainApplicationIntent() {
        return new Intent(Intent.ACTION_VIEW, Uri.parse(getString(R.string.app_url)));
    }

}

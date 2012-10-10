package org.vol.velocomp.views;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.LinearLayout;
import greendroid.widget.PageIndicator;
import greendroid.widget.PagedView;
import org.vol.velocomp.R;
import org.vol.velocomp.adapters.ConfigurationAdapter;
import org.vol.velocomp.messages.Configuration;
import org.vol.velocomp.service.BikeService;

public class ConfigurationView extends LinearLayout {

    private PageIndicator pageIndicatorNext;
    private PageIndicator pageIndicatorPrev;
    private PagedView pagedView;

    private Configuration configuration;

    private PagedView.OnPagedViewChangeListener onPagedViewChangeListener = new PagedView.OnPagedViewChangeListener() {

        @Override
        public void onStopTracking(PagedView pagedView) {
        }

        @Override
        public void onStartTracking(PagedView pagedView) {
        }

        @Override
        public void onPageChanged(PagedView pagedView, int previousPage, int newPage) {
            setActivePage(newPage);
        }
    };

    public ConfigurationView(Context context) {
        super(context);
    }

    public ConfigurationView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    private void setActivePage(int page) {
        pageIndicatorNext.setActiveDot(ConfigurationAdapter.PAGE_MAX_INDEX - page);
        pageIndicatorPrev.setActiveDot(page);
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();

        this.pagedView = (PagedView) findViewById(R.id.configuration_paged_view);

        if (this.pagedView != null) {
            this.pagedView.setOnPageChangeListener(onPagedViewChangeListener);
        }

        pageIndicatorNext = (PageIndicator) findViewById(R.id.page_indicator_next);
        if (pageIndicatorNext != null) {
            pageIndicatorNext.setDotCount(ConfigurationAdapter.PAGE_MAX_INDEX);
            pageIndicatorNext.setOnClickListener(new OnClickListener() {
                public void onClick(View v) {
                    ConfigurationView.this.pagedView.smoothScrollToNext();
                }
            });
        }

        pageIndicatorPrev = (PageIndicator) findViewById(R.id.page_indicator_prev);
        if (pageIndicatorPrev != null) {
            pageIndicatorPrev.setDotCount(ConfigurationAdapter.PAGE_MAX_INDEX);
            pageIndicatorPrev.setOnClickListener(new OnClickListener() {
                public void onClick(View v) {
                    ConfigurationView.this.pagedView.smoothScrollToPrevious();
                }
            });
        }
    }

    public void init() {
        configuration = BikeService.getInstance().getConfiguration();
        if (configuration != null) {
            BikeService.getInstance().setManualMode();
            this.pagedView.setAdapter(new ConfigurationAdapter(configuration));
        }
    }

    public void hide() {
        if (configuration != null) {
            BikeService.getInstance().sendConfiguration(configuration);
            BikeService.getInstance().saveConfiguration();
        }
    }

}

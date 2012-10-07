package org.vol.velocomp.views;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import greendroid.widget.PageIndicator;
import greendroid.widget.PagedView;
import org.vol.velocomp.R;
import org.vol.velocomp.adapters.ConfigurationAdapter;
import org.vol.velocomp.messages.Configuration;
import org.vol.velocomp.service.BikeService;

public class ConfigurationView extends PagedView {

    private PageIndicator pageIndicatorNext;
    private PageIndicator pageIndicatorPrev;

    private Configuration configuration;

    private OnPagedViewChangeListener onPagedViewChangeListener = new OnPagedViewChangeListener() {

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

    public ConfigurationView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    private void setActivePage(int page) {
        pageIndicatorNext.setActiveDot(ConfigurationAdapter.PAGE_MAX_INDEX - page);
        pageIndicatorPrev.setActiveDot(page);
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();

        this.setOnPageChangeListener(onPagedViewChangeListener);

        pageIndicatorNext = (PageIndicator) findViewById(R.id.page_indicator_next);

        if (pageIndicatorNext == null) {
            return;
        }

        pageIndicatorNext.setDotCount(ConfigurationAdapter.PAGE_MAX_INDEX);
        pageIndicatorNext.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                smoothScrollToNext();
            }
        });

        pageIndicatorPrev = (PageIndicator) findViewById(R.id.page_indicator_prev);
        pageIndicatorPrev.setDotCount(ConfigurationAdapter.PAGE_MAX_INDEX);
        pageIndicatorPrev.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                smoothScrollToPrevious();
            }
        });
    }

    public void init() {
        configuration = BikeService.getInstance().getConfiguration();
        if (configuration != null) {
            setAdapter(new ConfigurationAdapter(configuration));
            //setActivePage(getCurrentPage());
        }
    }
}

package org.vol.velocomp.graph;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.GraphViewSeries;
import org.vol.velocomp.utils.FixedSizeQueue;

public class FixedSizeGraphViewSeries extends GraphViewSeries {

    private final FixedSizeQueue<GraphView.GraphViewData> series;

    public FixedSizeGraphViewSeries(String description, GraphViewStyle style, int size) {
        super(description, style, new GraphView.GraphViewData[] {});
        this.series = new FixedSizeQueue<GraphView.GraphViewData>(size);
    }

    @Override
    public void appendData(GraphView.GraphViewData value, boolean scrollToEnd) {
        series.add(value);
    }

    public void prepare() {
        this.resetData(series.toArray(new GraphView.GraphViewData[0]));
    }

    public void appendData(GraphView.GraphViewData[] newValues, int length) {
        for (GraphView.GraphViewData data : newValues) {
            series.add(data);
            length--;
            if (length <= 0) {
                break;
            }
        }

        this.resetData(series.toArray(new GraphView.GraphViewData[0]));
    }
}

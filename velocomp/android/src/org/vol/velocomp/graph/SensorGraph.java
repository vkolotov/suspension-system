package org.vol.velocomp.graph;

import android.content.Context;
import android.graphics.*;
import com.jjoe64.graphview.LineGraphView;

import java.util.LinkedList;
import java.util.List;

public class SensorGraph extends LineGraphView {

    protected List<Threshold> thresholds = new LinkedList<Threshold>();

    protected String[] horlabels;
    protected String[] verlabels;

    protected double viewPortSize;

    public SensorGraph(Context context, String title) {
        super(context, title);
        this.setScalable(false);
        this.setScrollable(false);
        this.setHorizontalLabels(new String[]{});
    }

    @Override
    public void setHorizontalLabels(String[] horlabels) {
        super.setHorizontalLabels(horlabels);
        this.horlabels = horlabels;
    }

    protected String[] generateHorlabels(float graphwidth) {
        return horlabels;
    }

    @Override
    public void setVerticalLabels(String[] verlabels) {
        super.setVerticalLabels(verlabels);
        this.verlabels = verlabels;
    }

    @Override
    protected synchronized String[] generateVerlabels(float graphheight) {
        return verlabels;
    }

    @Override
    public void setViewPort(double start, double size) {
        super.setViewPort(start, size);
        this.viewPortSize = size;
    }

    @Override
    public void scrollToEnd() {
        super.setViewPort(getMaxX(true) - this.viewPortSize + this.viewPortSize * 0.1, this.viewPortSize);
    }

    @Override
    public void drawSeries(Canvas canvas, GraphViewData[] values, float graphwidth, float graphheight,
                           float border, double minX, double minY, double diffX, double diffY, float horstart) {
        paint.setTextSize(20);
        super.drawSeries(canvas, values, graphwidth, graphheight, border, minX, minY, diffX, diffY, horstart);

        paint.setStyle(Paint.Style.STROKE);
        PathEffect old = paint.getPathEffect();
        paint.setTextAlign(Paint.Align.RIGHT);
        for (Threshold threshold : thresholds) {
            paint.setColor(threshold.color);
            paint.setStrokeWidth(threshold.thickness);
            double valY = threshold.value - minY;
            double ratY = valY / diffY;
            float y = (float) (border - graphheight * ratY + graphheight);
            paint.setPathEffect(new DashPathEffect(new float[] {10, 20}, 0));
            canvas.drawLine(horstart, y, getWidth(), y, paint);
            paint.setPathEffect(old);
            paint.setStrokeWidth(0);
            canvas.drawText(String.valueOf((int)threshold.value), getWidth() - 45, y - 5, paint);
        }

    }

    public void addThreshold(Threshold threshold) {
        thresholds.add(threshold);
    }

    public void removeThreshold(Threshold threshold) {
        thresholds.remove(threshold);
    }

}

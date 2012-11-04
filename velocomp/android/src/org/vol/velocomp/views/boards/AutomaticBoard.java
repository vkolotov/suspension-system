package org.vol.velocomp.views.boards;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.GraphViewSeries;
import org.vol.velocomp.R;
import org.vol.velocomp.graph.FixedSizeGraphViewSeries;
import org.vol.velocomp.graph.SensorGraph;
import org.vol.velocomp.graph.Threshold;
import org.vol.velocomp.messages.AutomaticBoardMessage;
import org.vol.velocomp.messages.AutomaticTelemetry;
import org.vol.velocomp.service.BikeService;
import org.vol.velocomp.views.Board;
import org.vol.velocomp.views.Mode;
import org.vol.velocomp.views.controls.Indicator;
import org.vol.velocomp.views.controls.SeekBarConfig;

public class AutomaticBoard extends RelativeLayout {

    private Indicator cpuClockSpeed;
    private Indicator freeMemory;
    private Indicator speed;
    private Indicator cadence;
    private Indicator mode;
    private Indicator timeout;

    private SeekBarConfig unsprungSeverityThreshold;
    private SeekBarConfig sprungSeverityThreshold;

    private SensorGraph accelerometerReadingsGraph;
    private FixedSizeGraphViewSeries sprungSeries;
    private FixedSizeGraphViewSeries unsprungSeries;

    private Threshold sprungThreshold = new Threshold(0xff0077cc, 1, 50);
    private Threshold unsprungThreshold = new Threshold(0xffff0000, 1, 50);


    private long time;


    private Board board = new Board<AutomaticTelemetry, AutomaticBoard>(this, 1000) {
        @Override
        public AutomaticTelemetry getTelemetry() {
            return BikeService.getInstance().getAutomaticTelemetry();
        }

        @Override
        public void updateTelemetry(AutomaticTelemetry telemetry) {
            super.updateTelemetry(telemetry);
            cpuClockSpeed.setValue(telemetry.clockSpeed);
            freeMemory.setValue(telemetry.freeMemory);
            speed.setValue(telemetry.speed);
            cadence.setValue(telemetry.cadence);
            mode.setValue(geMode(telemetry.state));
            timeout.setValue(telemetry.timeout);

            unsprungSeverityThreshold.setValue(telemetry.unsprungSeverityThreshold);
            unsprungThreshold.setValue(telemetry.unsprungSeverityThreshold);

            sprungSeverityThreshold.setValue(telemetry.sprungSeverityThreshold);
            sprungThreshold.setValue(telemetry.sprungSeverityThreshold);


            for (int i = 0; i < telemetry.dataLength; i++) {
                sprungSeries.appendData(new GraphView.GraphViewData(time, telemetry.sprungReadingsX[i]), false);
                unsprungSeries.appendData(new GraphView.GraphViewData(time, telemetry.unsprungReadingsX[i]), false);
                time += 50;
            }
            sprungSeries.prepare();
            unsprungSeries.prepare();
            if (telemetry.dataLength > 0) {
                accelerometerReadingsGraph.scrollToEnd();
            }

        }

        @Override
        public void runOnUi(Runnable runnable) {
            post(runnable);
        }

        @Override
        public void onConnected() {
            unsprungSeverityThreshold.setEnabled(true);
            sprungSeverityThreshold.setEnabled(true);
        }

        @Override
        public void onDisconnected() {
            unsprungSeverityThreshold.setEnabled(false);
            sprungSeverityThreshold.setEnabled(false);
        }
    };

    private String geMode(byte state) {
        Mode result = Mode.getMode(state);
        return result != null ? result.getName() : "Unknown";
    }

    private class SeverityThresholdSeekBarListener extends SeekBarConfig.AbstractOnSeekBarChangeListener {

        private Threshold threshold;

        private SeverityThresholdSeekBarListener(Threshold threshold) {
            this.threshold = threshold;
        }

        @Override
        public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
            threshold.setValue(i);
        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
            BikeService.getInstance().sendAutomaticMessage(getAutomaticBoardMessage());
        }
    }


    public AutomaticBoard(Context context) {
        super(context);
    }

    public AutomaticBoard(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public AutomaticBoard(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();

        cpuClockSpeed = (Indicator) findViewById(R.id.clockSpeed);
        freeMemory = (Indicator) findViewById(R.id.freeMemory);
        speed = (Indicator) findViewById(R.id.speed);
        cadence = (Indicator) findViewById(R.id.cadence);
        mode = (Indicator) findViewById(R.id.mode);
        timeout = (Indicator) findViewById(R.id.timeout);

        unsprungSeverityThreshold = (SeekBarConfig) findViewById(R.id.unsprungSeverityThreshold);
        if (unsprungSeverityThreshold != null) {
            unsprungSeverityThreshold.setOnSeekBarChangeListener(new SeverityThresholdSeekBarListener(unsprungThreshold));
        }

        sprungSeverityThreshold = (SeekBarConfig) findViewById(R.id.sprungSeverityThreshold);
        if (sprungSeverityThreshold != null) {
            sprungSeverityThreshold.setOnSeekBarChangeListener(new SeverityThresholdSeekBarListener(sprungThreshold));
        }

        sprungSeries = new FixedSizeGraphViewSeries("Sprung", new GraphViewSeries.GraphViewStyle(0xff0077cc, 3), 20 * 8 * 10);
        unsprungSeries = new FixedSizeGraphViewSeries("Unsprung", new GraphViewSeries.GraphViewStyle(0xffff0000, 3), 20 * 8 * 10);

        accelerometerReadingsGraph = new SensorGraph(this.getContext(), "Accelerometers");

        accelerometerReadingsGraph.addSeries(unsprungSeries);
        accelerometerReadingsGraph.addSeries(sprungSeries);

        accelerometerReadingsGraph.setViewPort(0, 4000);
        accelerometerReadingsGraph.setManualYAxis(true);
        accelerometerReadingsGraph.setManualYAxisBounds(400, -400);
        accelerometerReadingsGraph.setVerticalLabels(new String[]{"400", "0", "-400"});

        accelerometerReadingsGraph.addThreshold(sprungThreshold);
        accelerometerReadingsGraph.addThreshold(unsprungThreshold);


        LinearLayout layout = (LinearLayout) findViewById(R.id.accelerometersReadings);
        layout.addView(accelerometerReadingsGraph);

        layout.setOnClickListener( new OnClickListener() {
            @Override
            public void onClick(View view) {
                int visibility = findViewById(R.id.configs).getVisibility();
                findViewById(R.id.configs).setVisibility(View.VISIBLE == visibility ? View.GONE : View.VISIBLE);
                findViewById(R.id.indicators).setVisibility(View.VISIBLE != visibility ? View.GONE : View.VISIBLE);
            }
        });

    }

    private AutomaticBoardMessage getAutomaticBoardMessage() {
        AutomaticBoardMessage automaticBoardMessage = new AutomaticBoardMessage();
        automaticBoardMessage.unsprungSeverityThreshold = (short) unsprungSeverityThreshold.getValue();
        automaticBoardMessage.sprungSeverityThreshold = (short) sprungSeverityThreshold.getValue();
        return automaticBoardMessage;
    }


    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
        BikeService.getInstance().setAutomaticMode();
        board.onShow(Mode.AUTOMATIC);
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        board.onHide();
    }

}

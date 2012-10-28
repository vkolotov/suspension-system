package org.vol.velocomp.views;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;
import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.GraphViewSeries;
import org.vol.velocomp.R;
import org.vol.velocomp.graph.FixedSizeGraphViewSeries;
import org.vol.velocomp.graph.SensorGraph;
import org.vol.velocomp.graph.Threshold;
import org.vol.velocomp.messages.AutomaticBoardMessage;
import org.vol.velocomp.messages.AutomaticTelemetry;
import org.vol.velocomp.service.BikeService;

public class AutomaticBoard extends RelativeLayout {

    private Indicator cpuClockSpeed;
    private Indicator speed;
    private Indicator cadence;
    private Indicator mode;
    private Indicator timeout;

    private TextView unsprungSeverityThreshold;
    private SeekBar unsprungSeverityThresholdSeekBar;
    private TextView sprungSeverityThreshold;
    private SeekBar sprungSeverityThresholdSeekBar;

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
            speed.setValue(telemetry.speed);
            cadence.setValue(telemetry.cadence);
            mode.setValue(geMode(telemetry.state));
            timeout.setValue(telemetry.timeout);

            unsprungSeverityThreshold.setText(String.valueOf(telemetry.unsprungSeverityThreshold));
            unsprungSeverityThresholdSeekBar.setProgress(telemetry.unsprungSeverityThreshold);
            unsprungThreshold.setValue(telemetry.unsprungSeverityThreshold);

            sprungSeverityThreshold.setText(String.valueOf(telemetry.sprungSeverityThreshold));
            sprungSeverityThresholdSeekBar.setProgress(-telemetry.sprungSeverityThreshold);
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
            unsprungSeverityThresholdSeekBar.setEnabled(true);
        }

        @Override
        public void onDisconnected() {
            unsprungSeverityThresholdSeekBar.setEnabled(false);
        }
    };

    private String geMode(byte state) {
        Mode result = Mode.getMode(state);
        return result != null ? result.getName() : "Unknown";
    }

    private class SeverityThresholdSeekBarListener implements SeekBar.OnSeekBarChangeListener {

        private TextView label;
        private Threshold threshold;

        private SeverityThresholdSeekBarListener(TextView label, Threshold threshold) {
            this.label = label;
            this.threshold = threshold;
        }

        @Override
        public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
            label.setText(String.valueOf(threshold == sprungThreshold ? -i : i));
            threshold.setValue(threshold == sprungThreshold ? -i : i);
        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {

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
        speed = (Indicator) findViewById(R.id.speed);
        cadence = (Indicator) findViewById(R.id.cadence);
        mode = (Indicator) findViewById(R.id.mode);
        timeout = (Indicator) findViewById(R.id.timeout);

        unsprungSeverityThreshold = (TextView) findViewById(R.id.unsprungSeverityThreshold);
        unsprungSeverityThresholdSeekBar = (SeekBar) findViewById(R.id.unsprungSeverityThresholdSeekBar);
        if (unsprungSeverityThresholdSeekBar != null) {
            unsprungSeverityThresholdSeekBar.setOnSeekBarChangeListener(new SeverityThresholdSeekBarListener(unsprungSeverityThreshold, unsprungThreshold));
            unsprungSeverityThresholdSeekBar.setMax(400);
        }

        sprungSeverityThreshold = (TextView) findViewById(R.id.sprungSeverityThreshold);
        sprungSeverityThresholdSeekBar = (SeekBar) findViewById(R.id.sprungSeverityThresholdSeekBar);
        if (sprungSeverityThresholdSeekBar != null) {
            sprungSeverityThresholdSeekBar.setOnSeekBarChangeListener(new SeverityThresholdSeekBarListener(sprungSeverityThreshold, sprungThreshold));
            sprungSeverityThresholdSeekBar.setMax(400);
        }

        sprungSeries = new FixedSizeGraphViewSeries("Sprung", new GraphViewSeries.GraphViewStyle(0xff0077cc, 3), 20 * 8);
        unsprungSeries = new FixedSizeGraphViewSeries("Unsprung", new GraphViewSeries.GraphViewStyle(0xffff0000, 3), 20 * 8);

        accelerometerReadingsGraph = new SensorGraph(this.getContext(), "Accelerometers");

        accelerometerReadingsGraph.addSeries(unsprungSeries);
        accelerometerReadingsGraph.addSeries(sprungSeries);

        accelerometerReadingsGraph.setViewPort(0, 7000);
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
                int visibility = findViewById(R.id.unsprungSeverityThresholdGroup).getVisibility();
                findViewById(R.id.unsprungSeverityThresholdGroup).setVisibility(View.VISIBLE == visibility ? View.GONE : View.VISIBLE);
                findViewById(R.id.sprungSeverityThresholdGroup).setVisibility(View.VISIBLE == visibility ? View.GONE : View.VISIBLE);
            }
        });

    }

    private AutomaticBoardMessage getAutomaticBoardMessage() {
        AutomaticBoardMessage automaticBoardMessage = new AutomaticBoardMessage();
        automaticBoardMessage.unsprungSeverityThreshold = (short) unsprungSeverityThresholdSeekBar.getProgress();
        automaticBoardMessage.sprungSeverityThreshold = (short) -sprungSeverityThresholdSeekBar.getProgress();
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

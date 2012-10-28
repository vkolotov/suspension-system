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
import org.vol.velocomp.messages.CDTBoardMessage;
import org.vol.velocomp.messages.CDTTelemetry;
import org.vol.velocomp.service.BikeService;

public class CDTBoard extends RelativeLayout {

    private Indicator cpuClockSpeed;
    private Indicator speed;
    private Indicator cadence;
    private Indicator gradient;
    private Indicator cdtMode;

    private TextView climbGradient;
    private SeekBar climbGradientSeekBar;
    private TextView descendGradient;
    private SeekBar descendGradientSeekBar;

    private SensorGraph gradientGraph;
    private FixedSizeGraphViewSeries filteredGradientSeries;
    private FixedSizeGraphViewSeries rawGradientSeries;

    private Threshold climbThreshold = new Threshold(0xffff0000, 1, 0);
    private Threshold descentThreshold = new Threshold(0xff0077cc, 1, 0);


    private long time;

    private Board board = new Board<CDTTelemetry, CDTBoard>(this, 1000) {
        @Override
        public CDTTelemetry getTelemetry() {
            return BikeService.getInstance().getCDTTelemetry();
        }

        @Override
        public void updateTelemetry(CDTTelemetry telemetry) {
            super.updateTelemetry(telemetry);
            cpuClockSpeed.setValue(telemetry.clockSpeed);
            speed.setValue(telemetry.speed);
            cadence.setValue(telemetry.cadence);
            gradient.setValue(telemetry.gradient);
            cdtMode.setValue(telemetry.suspensionMode == 0 ? "Climb" : (telemetry.suspensionMode == 1 ? "Trail" : "Descend"));

            descendGradient.setText(String.valueOf(telemetry.descendGradient));
            climbGradient.setText(String.valueOf(telemetry.climbGradient));

            descendGradientSeekBar.setProgress(Math.abs(telemetry.descendGradient));
            climbGradientSeekBar.setProgress(telemetry.climbGradient);

            descendGradientSeekBar.setSecondaryProgress(-telemetry.gradient);
            climbGradientSeekBar.setSecondaryProgress(telemetry.gradient);

            climbThreshold.setValue(telemetry.climbGradient);
            descentThreshold.setValue(telemetry.descendGradient);

            for (int i = 0; i < telemetry.dataLength; i++) {
                filteredGradientSeries.appendData(new GraphView.GraphViewData(time, telemetry.filteredGradients[i]), false);
                rawGradientSeries.appendData(new GraphView.GraphViewData(time, telemetry.rawGradients[i]), false);
                time += 50;
            }
            filteredGradientSeries.prepare();
            rawGradientSeries.prepare();
            if (telemetry.dataLength > 0) {
                gradientGraph.scrollToEnd();
            }

        }

        @Override
        public void runOnUi(Runnable runnable) {
            post(runnable);
        }

        @Override
        public void onConnected() {
            descendGradientSeekBar.setEnabled(true);
            climbGradientSeekBar.setEnabled(true);
        }

        @Override
        public void onDisconnected() {
            descendGradientSeekBar.setEnabled(false);
            climbGradientSeekBar.setEnabled(false);
        }
    };

    private class GradientSeekBarListener implements SeekBar.OnSeekBarChangeListener {

        private TextView label;
        private Threshold threshold;

        private GradientSeekBarListener(TextView label, Threshold threshold) {
            this.label = label;
            this.threshold = threshold;
        }

        @Override
        public void onProgressChanged(SeekBar seekBar, int i, boolean b) {

            if (seekBar == descendGradientSeekBar) {
                i = -i;
            }

            label.setText(String.valueOf(i));
            threshold.setValue(i);
        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {

        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
            BikeService.getInstance().sendCDTBoardMessage(getCdtBoardMessage());
        }
    }


    public CDTBoard(Context context) {
        super(context);
    }

    public CDTBoard(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public CDTBoard(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();

        cpuClockSpeed = (Indicator) findViewById(R.id.clockSpeed);
        speed = (Indicator) findViewById(R.id.speed);
        cadence = (Indicator) findViewById(R.id.cadence);
        gradient = (Indicator) findViewById(R.id.gradient);
        cdtMode = (Indicator) findViewById(R.id.cdtMode);

        climbGradient = (TextView) findViewById(R.id.climbGradient);
        climbGradientSeekBar = (SeekBar) findViewById(R.id.climbGradientSeekBar);
        if (climbGradientSeekBar != null) {
            climbGradientSeekBar.setOnSeekBarChangeListener(new GradientSeekBarListener(climbGradient, climbThreshold));
        }
        descendGradient = (TextView) findViewById(R.id.descendGradient);
        descendGradientSeekBar = (SeekBar) findViewById(R.id.descendGradientSeekBar);
        if (descendGradientSeekBar != null) {
            descendGradientSeekBar.setOnSeekBarChangeListener(new GradientSeekBarListener(descendGradient, descentThreshold));
        }

        if (gradient != null) {
            gradient.setOnClickListener( new OnClickListener() {
                @Override
                public void onClick(View view) {
                    BikeService.getInstance().calibrateGradient();
                }
            });
        }

        filteredGradientSeries = new FixedSizeGraphViewSeries("Filtered", new GraphViewSeries.GraphViewStyle(0xff0077cc, 3), 20 * 8);
        rawGradientSeries = new FixedSizeGraphViewSeries("Raw", new GraphViewSeries.GraphViewStyle(0xffff0000, 3), 20 * 8);

        gradientGraph = new SensorGraph(this.getContext(), "Gradient");
        gradientGraph.addSeries(rawGradientSeries);
        gradientGraph.addSeries(filteredGradientSeries);


        gradientGraph.setViewPort(0, 7000);
        gradientGraph.setManualYAxis(true);
        gradientGraph.setManualYAxisBounds(40, -40);
        gradientGraph.setVerticalLabels( new String[] {"40", "0", "-40"});

        LinearLayout layout = (LinearLayout) findViewById(R.id.gradientGraph);
        layout.addView(gradientGraph);

        layout.setOnClickListener( new OnClickListener() {
            @Override
            public void onClick(View view) {
                int visibility = findViewById(R.id.climbGradientGroup).getVisibility();
                findViewById(R.id.climbGradientGroup).setVisibility(View.VISIBLE == visibility ? View.GONE : View.VISIBLE);
                findViewById(R.id.descendGradientGroup).setVisibility(View.VISIBLE == visibility ? View.GONE : View.VISIBLE);
            }
        });

        gradientGraph.addThreshold(climbThreshold);
        gradientGraph.addThreshold(descentThreshold);
    }

    private CDTBoardMessage getCdtBoardMessage() {
        CDTBoardMessage cdtBoardMessage = new CDTBoardMessage();
        cdtBoardMessage.climbGradient = (byte) climbGradientSeekBar.getProgress();
        cdtBoardMessage.descendGradient = (byte) -descendGradientSeekBar.getProgress();
        return cdtBoardMessage;
    }


    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
        BikeService.getInstance().setCDTMode();
        board.onShow(Mode.CDT);
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        board.onHide();
    }

}

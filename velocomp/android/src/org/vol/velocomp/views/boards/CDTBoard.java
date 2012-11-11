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
import org.vol.velocomp.VelocompApplication;
import org.vol.velocomp.graph.FixedSizeGraphViewSeries;
import org.vol.velocomp.graph.SensorGraph;
import org.vol.velocomp.graph.Threshold;
import org.vol.velocomp.messages.CDTBoardMessage;
import org.vol.velocomp.messages.CDTTelemetry;
import org.vol.velocomp.service.BikeService;
import org.vol.velocomp.views.Board;
import org.vol.velocomp.views.Mode;
import org.vol.velocomp.views.controls.Indicator;
import org.vol.velocomp.views.controls.SeekBarConfig;

public class CDTBoard extends RelativeLayout {

    private Indicator cpuClockSpeed;
    private Indicator freeMemory;
    private Indicator speed;
    private Indicator cadence;
    private Indicator gradient;
    private Indicator cdtMode;

    private SeekBarConfig climbGradient;
    private SeekBarConfig descendGradient;

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
            freeMemory.setValue(telemetry.freeMemory);
            speed.setValue(telemetry.speed);
            cadence.setValue(telemetry.cadence);
            gradient.setValue(telemetry.gradient);
            cdtMode.setValue(telemetry.suspensionMode == 0 ? "Climb" : (telemetry.suspensionMode == 1 ? "Trail" : "Descend"));

            descendGradient.setValue(telemetry.descendGradient);
            climbGradient.setValue(telemetry.climbGradient);

            descendGradient.setSecondaryProgress(-telemetry.gradient);
            climbGradient.setSecondaryProgress(telemetry.gradient);

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
            descendGradient.setEnabled(true);
            climbGradient.setEnabled(true);
        }

        @Override
        public void onDisconnected() {
            descendGradient.setEnabled(false);
            climbGradient.setEnabled(false);
        }
    };

    private class GradientSeekBarListener extends SeekBarConfig.AbstractOnSeekBarChangeListener {

        private Threshold threshold;

        private GradientSeekBarListener(Threshold threshold) {
            this.threshold = threshold;
        }

        @Override
        public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
            threshold.setValue(i);
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
        freeMemory = (Indicator) findViewById(R.id.freeMemory);
        speed = (Indicator) findViewById(R.id.speed);
        cadence = (Indicator) findViewById(R.id.cadence);
        gradient = (Indicator) findViewById(R.id.gradient);
        cdtMode = (Indicator) findViewById(R.id.cdtMode);

        if (!VelocompApplication.isDebug) {
            cpuClockSpeed.setVisibility(View.GONE);
            freeMemory.setVisibility(View.GONE);
        }

        climbGradient = (SeekBarConfig) findViewById(R.id.climbGradient);

        if (climbGradient != null) {
            climbGradient.setOnSeekBarChangeListener(new GradientSeekBarListener(climbThreshold));
        }
        descendGradient = (SeekBarConfig) findViewById(R.id.descentGradient);

        if (descendGradient != null) {
            descendGradient.setOnSeekBarChangeListener(new GradientSeekBarListener(descentThreshold));
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
                int visibility = findViewById(R.id.configs).getVisibility();
                findViewById(R.id.configs).setVisibility(View.VISIBLE == visibility ? View.GONE : View.VISIBLE);
                findViewById(R.id.indicators).setVisibility(View.VISIBLE != visibility ? View.GONE : View.VISIBLE);
            }
        });

        gradientGraph.addThreshold(climbThreshold);
        gradientGraph.addThreshold(descentThreshold);
    }

    private CDTBoardMessage getCdtBoardMessage() {
        CDTBoardMessage cdtBoardMessage = new CDTBoardMessage();
        cdtBoardMessage.climbGradient = (byte) climbGradient.getValue();
        cdtBoardMessage.descendGradient = (byte) descendGradient.getValue();
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

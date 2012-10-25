package org.vol.velocomp.views;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;
import org.vol.velocomp.R;
import org.vol.velocomp.messages.AutomaticBoardMessage;
import org.vol.velocomp.messages.AutomaticTelemetry;
import org.vol.velocomp.service.BikeService;

public class AutomaticBoard extends RelativeLayout {

    private Indicator cpuClockSpeed;
    private Indicator speed;
    private Indicator cadence;
    private Indicator mode;
    private Indicator timeout;

    private TextView severityThreshold;
    private SeekBar severityThresholdSeekBar;


    private Board board = new Board<AutomaticTelemetry, AutomaticBoard>(this, 500) {
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

            severityThreshold.setText(String.valueOf(telemetry.severityThreshold));
            severityThresholdSeekBar.setProgress(telemetry.severityThreshold);
            severityThresholdSeekBar.setSecondaryProgress(telemetry.idleValue);

        }

        @Override
        public void runOnUi(Runnable runnable) {
            post(runnable);
        }

        @Override
        public void onConnected() {
            severityThresholdSeekBar.setEnabled(true);
        }

        @Override
        public void onDisconnected() {
            severityThresholdSeekBar.setEnabled(false);
        }
    };

    private String geMode(byte state) {
        Mode result = Mode.getMode(state);
        return result != null ? result.getName() : "Unknown";
    }

    private class SeverityThresholdSeekBarListener implements SeekBar.OnSeekBarChangeListener {

        private TextView label;

        private SeverityThresholdSeekBarListener(TextView label) {
            this.label = label;
        }

        @Override
        public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
            label.setText(String.valueOf(i));
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

        severityThreshold = (TextView) findViewById(R.id.severityThreshold);
        severityThresholdSeekBar = (SeekBar) findViewById(R.id.severityThresholdSeekBar);
        if (severityThresholdSeekBar != null) {
            severityThresholdSeekBar.setOnSeekBarChangeListener(new SeverityThresholdSeekBarListener(severityThreshold));
            severityThresholdSeekBar.setMax(400);
        }

    }

    private AutomaticBoardMessage getAutomaticBoardMessage() {
        AutomaticBoardMessage automaticBoardMessage = new AutomaticBoardMessage();
        automaticBoardMessage.severityThreshold = (short) severityThresholdSeekBar.getProgress();
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

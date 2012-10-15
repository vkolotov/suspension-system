package org.vol.velocomp.views;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;
import org.vol.velocomp.R;
import org.vol.velocomp.messages.CDTBoardMessage;
import org.vol.velocomp.messages.CDTTelemetry;
import org.vol.velocomp.service.BikeService;

public class CDTBoard extends RelativeLayout {

    private TextView speed;
    private TextView cadence;
    private TextView gradient;
    private TextView cdtMode;
    private TextView climbGradient;

    private TextView delta;

    private SeekBar climbGradientSeekBar;
    private TextView descendGradient;
    private SeekBar descendGradientSeekBar;
    private View calibrateGradient;

    private Board board = new Board<CDTTelemetry, CDTBoard>(this) {
        @Override
        public CDTTelemetry getTelemetry() {
            return BikeService.getInstance().getCDTTelemetry();
        }

        @Override
        public void updateTelemetry(CDTTelemetry telemetry) {
            super.updateTelemetry(telemetry);
            speed.setText(String.valueOf(telemetry.speed));
            cadence.setText(String.valueOf(telemetry.cadence));
            gradient.setText(String.valueOf(telemetry.gradient));
            delta.setText(String.valueOf(telemetry.delta));

            cdtMode.setText(telemetry.suspensionMode == 0 ? "Climb" : (telemetry.suspensionMode == 1 ? "Trail" : "Descend"));

            descendGradient.setText(String.valueOf(telemetry.descendGradient));
            climbGradient.setText(String.valueOf(telemetry.climbGradient));
            descendGradientSeekBar.setProgress(Math.abs(telemetry.descendGradient));
            climbGradientSeekBar.setProgress(telemetry.climbGradient);

            descendGradientSeekBar.setSecondaryProgress(-telemetry.gradient);
            climbGradientSeekBar.setSecondaryProgress(telemetry.gradient);
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

        private GradientSeekBarListener(TextView label) {
            this.label = label;
        }

        @Override
        public void onProgressChanged(SeekBar seekBar, int i, boolean b) {

            if (seekBar == descendGradientSeekBar) {
                i = -i;
            }

            label.setText(String.valueOf(i));
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

        speed = (TextView) findViewById(R.id.speed);
        cadence = (TextView) findViewById(R.id.cadence);
        gradient = (TextView) findViewById(R.id.gradient);
        cdtMode = (TextView) findViewById(R.id.cdtMode);
        delta = (TextView) findViewById(R.id.delta);

        climbGradient = (TextView) findViewById(R.id.climbGradient);
        climbGradientSeekBar = (SeekBar) findViewById(R.id.climbGradientSeekBar);
        if (climbGradientSeekBar != null) {
            climbGradientSeekBar.setOnSeekBarChangeListener(new GradientSeekBarListener(climbGradient));
        }
        descendGradient = (TextView) findViewById(R.id.descendGradient);
        descendGradientSeekBar = (SeekBar) findViewById(R.id.descendGradientSeekBar);
        if (descendGradientSeekBar != null) {
            descendGradientSeekBar.setOnSeekBarChangeListener(new GradientSeekBarListener(descendGradient));
        }

        calibrateGradient = findViewById(R.id.gradientGroup);
        if (calibrateGradient != null) {
            calibrateGradient.setOnClickListener( new OnClickListener() {
                @Override
                public void onClick(View view) {
                    BikeService.getInstance().calibrateGradient();
                }
            });
        }
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

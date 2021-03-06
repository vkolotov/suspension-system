package org.vol.velocomp.views.boards;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.ToggleButton;
import org.vol.velocomp.R;
import org.vol.velocomp.VelocompApplication;
import org.vol.velocomp.messages.ManualTelemetry;
import org.vol.velocomp.service.BikeService;
import org.vol.velocomp.views.Board;
import org.vol.velocomp.views.controls.Indicator;
import org.vol.velocomp.views.Mode;

public class ManualBoard extends RelativeLayout {

    private ToggleButton climbButton;
    private ToggleButton trialButton;
    private ToggleButton descendButton;

    private Indicator cpuClockSpeed;
    private Indicator freeMemory;
    private Indicator speed;
    private Indicator cadence;

    private boolean firstLoad = true;


    private Board board = new Board<ManualTelemetry, ManualBoard>(this, 2000) {
        @Override
        public ManualTelemetry getTelemetry() {
            return BikeService.getInstance().getManualTelemetry();
        }

        @Override
        public void updateTelemetry(ManualTelemetry telemetry) {
            super.updateTelemetry(telemetry);

            cpuClockSpeed.setValue(telemetry.clockSpeed);
            freeMemory.setValue(telemetry.freeMemory);
            speed.setValue(telemetry.speed);
            cadence.setValue(telemetry.cadence);
            updateButtons(telemetry.suspensionMode);
        }

        @Override
        public void runOnUi(Runnable runnable) {
            post(runnable);
        }

        @Override
        public void onConnected() {

        }

        @Override
        public void onDisconnected() {
            disableButtons();
        }
    };

    private OnClickListener onClickListener = new OnClickListener() {
        @Override
        public void onClick(View view) {
            updateButtons(view == climbButton ? 0 : view == trialButton ? 1 : 2);
            if (view == climbButton) {
                BikeService.getInstance().setClimbModeAll();
            } else if (view == trialButton) {
                BikeService.getInstance().setTrailModeAll();
            } else {
                BikeService.getInstance().setDescendModeAll();
            }
        }
    };

    public ManualBoard(Context context) {
        super(context);
    }

    public ManualBoard(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public ManualBoard(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();
        climbButton = (ToggleButton) findViewById(R.id.climbButton);
        if (climbButton != null) {
            climbButton.setOnClickListener(onClickListener);
        }
        trialButton = (ToggleButton) findViewById(R.id.trailButton);
        if (trialButton != null) {
            trialButton.setOnClickListener(onClickListener);
        }
        descendButton = (ToggleButton) findViewById(R.id.descendButton);
        if (descendButton != null) {
            descendButton.setOnClickListener(onClickListener);
        }

        cpuClockSpeed = (Indicator) findViewById(R.id.clockSpeed);
        freeMemory = (Indicator) findViewById(R.id.freeMemory);

        if (!VelocompApplication.isDebug) {
            cpuClockSpeed.setVisibility(View.GONE);
            freeMemory.setVisibility(View.GONE);
        }

        cadence = (Indicator) findViewById(R.id.cadence);
        speed = (Indicator) findViewById(R.id.speed);

    }

    private void updateButtons(int mode) {
        climbButton.setEnabled(mode != 0);
        climbButton.setChecked(mode == 0);
        trialButton.setEnabled(mode != 1);
        trialButton.setChecked(mode == 1);
        descendButton.setEnabled(mode != 2);
        descendButton.setChecked(mode == 2);
    }

    private void disableButtons() {
        climbButton.setEnabled(false);
        climbButton.setChecked(false);
        trialButton.setEnabled(false);
        trialButton.setChecked(false);
        descendButton.setEnabled(false);
        descendButton.setChecked(false);
    }

    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
        if (!firstLoad) {
            BikeService.getInstance().setManualMode();
        }
        board.onShow(Mode.MANUAL);
        firstLoad = false;
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        board.onHide();
    }
}

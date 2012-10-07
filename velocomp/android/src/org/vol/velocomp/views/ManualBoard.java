package org.vol.velocomp.views;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.ToggleButton;
import org.vol.velocomp.R;
import org.vol.velocomp.messages.Telemetry;
import org.vol.velocomp.service.BikeService;

public class ManualBoard extends RelativeLayout {

    private ToggleButton climbButton;
    private ToggleButton trialButton;
    private ToggleButton descendButton;

    private OnClickListener onClickListener = new OnClickListener() {
        @Override
        public void onClick(View view) {
            climbButton.setEnabled(view != climbButton);
            climbButton.setChecked(view == climbButton);
            trialButton.setEnabled(view != trialButton);
            trialButton.setChecked(view == trialButton);
            descendButton.setEnabled(view != descendButton);
            descendButton.setChecked(view == descendButton);
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
    }

    public void updateTelemetry(Telemetry telemetry) {
        ((TextView) findViewById(R.id.speedField)).setText(String.valueOf(telemetry.speed));
        ((TextView) findViewById(R.id.cadence)).setText(String.valueOf(telemetry.cadence));
    }

}

package org.vol.velocomp.views;

import android.view.View;
import android.view.ViewParent;
import org.vol.velocomp.messages.Telemetry;
import org.vol.velocomp.service.BikeService;
import org.vol.velocomp.threads.TelemetryThread;



enum Mode {
    MANUAL(0, "Manual"),
    CDT(10, "CDT"),
    AUTOMATIC(20, "Automatic"),

    COMMON_STATE(100, "Common"),
    SLEEP_STATE(101, "Sleep"),

    CLIMB_STATE(11, "Climb"),
    DESCENT_STATE(12, "Descent"),
    TRAIL_STATE(13, "Trail"),

    ABSORB_STATE(21, "Absorb"),
    ACTIVE_STATE(22, "Active"),
    BURNOUT_STATE(23, "Burnout"),
    IDLE_STATE(24, "Idle"),
    PEDAL_STATE(25, "Pedal"),
    STANDOVER_STATE(26, "Standover");

    private int state;
    private String name;

    Mode(int state, String name) {
        this.state = state;
        this.name = name;
    }

    int getBoard() {
        return state / 10;
    }

    public String getName() {
        return name;
    }

    static Mode matchState(int state) {
        if (state < 10) {
            return  MANUAL;
        } else if (state >= 10 && state < 20) {
            return CDT;
        } else {
            return AUTOMATIC;
        }
    }

    static Mode getMode(int state) {
        for (Mode mode : values()) {
            if (mode.state == state) {
                return mode;
            }
        }
        return null;
    }
}

public abstract class Board<T extends Telemetry, V extends View & ViewParent> {

    private TelemetryThread telemetryThread;
    private boolean isShown;
    private Mode currentMode = null;

    private V boardView;
    private Dashboard dashboard;

    protected Board(V boardView) {
        BikeService.getInstance().addListener(bikeServiceListener);
        this.boardView = boardView;
    }

    private BikeService.BikeServiceListener bikeServiceListener = new BikeService.BikeServiceListener() {
        @Override
        public void onDisconnected(Exception ex) {
            stopTelemetryThread();
            runOnUi( new Runnable() {
                @Override
                public void run() {
                    Board.this.onDisconnected();
                }
            });
        }

        @Override
        public void onConnected() {
            startTelemetryThread();
            runOnUi( new Runnable() {
                @Override
                public void run() {
                    Board.this.onConnected();
                }
            });
        }
    };

    public void startTelemetryThread() {
        if (isShown && BikeService.getInstance().isConnected()
                && (telemetryThread == null || telemetryThread.isKilled())) {

            telemetryThread = new TelemetryThread(this, 2000);
            telemetryThread.start();
        }
    }

    public void stopTelemetryThread() {
        if (telemetryThread != null) {
            telemetryThread.cancel();
            telemetryThread = null;
        }
    }

    public void onShow(Mode mode) {
        this.currentMode = mode;
        isShown = true;
        startTelemetryThread();
    }

    public void onHide() {
        isShown = false;
        stopTelemetryThread();
    }

    abstract public T getTelemetry();

    public void updateTelemetry(T telemetry) {
        if (Mode.matchState(telemetry.state) != currentMode) {
            currentMode = Mode.matchState(telemetry.state);
            onChangeMode(currentMode);
        }
    }

    abstract public void runOnUi(Runnable runnable);

    abstract public void onConnected();

    abstract public void onDisconnected();

    public void onChangeMode(Mode mode) {
        getDashboard().setBoard(mode);
    }

    public Dashboard getDashboard() {

        if (dashboard != null) {
            return dashboard;
        }

        ViewParent tmp = boardView;
        while (tmp != null) {
            tmp = tmp.getParent();
            if (tmp instanceof Dashboard) {
                dashboard = (Dashboard) tmp;
                break;
            }
        }
        return dashboard;
    }
}

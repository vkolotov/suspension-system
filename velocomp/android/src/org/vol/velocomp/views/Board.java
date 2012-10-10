package org.vol.velocomp.views;

import android.view.View;
import android.view.ViewParent;
import org.vol.velocomp.messages.Telemetry;
import org.vol.velocomp.service.BikeService;
import org.vol.velocomp.threads.TelemetryThread;

enum Mode {
    MANUAL(0),
    CDT(10),
    AUTOMATIC(20);

    private int state;

    Mode(int state) {
        this.state = state;
    }

    int getBoard() {
        return state / 10;
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

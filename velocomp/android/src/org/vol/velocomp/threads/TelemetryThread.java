package org.vol.velocomp.threads;

import org.vol.velocomp.exceptions.ConnectionException;
import org.vol.velocomp.messages.Telemetry;
import org.vol.velocomp.views.Board;

import java.util.concurrent.TimeoutException;

public class TelemetryThread extends GenericThread {

    private Board board;

    public TelemetryThread(Board board, long sleepTime) {
        super(sleepTime);
        this.board = board;
    }

    @Override
    public void kill() {

    }

    @Override
    public void perform() throws TimeoutException, ConnectionException {
        final Telemetry telemetry = board.getTelemetry();
        if (telemetry != null) {
            board.runOnUi(new Runnable() {
                @Override
                public void run() {
                    board.updateTelemetry(telemetry);
                }
            });
        }
    }

}

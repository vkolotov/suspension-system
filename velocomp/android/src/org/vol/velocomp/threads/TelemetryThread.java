package org.vol.velocomp.threads;

import org.vol.velocomp.BikeConnection;
import org.vol.velocomp.ConnectionException;
import org.vol.velocomp.VelocompActivity;
import org.vol.velocomp.messages.Telemetry;

import java.util.concurrent.TimeoutException;


public class TelemetryThread extends GenericThread {

    public TelemetryThread(VelocompActivity velocompActivity) {
        super(velocompActivity, 2000);
    }

    @Override
    public void kill() {

    }

    @Override
    public void perform() throws TimeoutException, ConnectionException {
        if (velocompActivity.isConnected()) {
            final Telemetry telemetry = BikeConnection.getInstance().request(Telemetry.class, (byte) 1, 5000);
            this.velocompActivity.runOnUiThread(new Runnable() {
                public void run() {
                    velocompActivity.updateTelemetry(telemetry);
                }
            });

        }
    }
}

package org.vol.velocomp.threads;


import org.vol.velocomp.BikeConnection;
import org.vol.velocomp.ConnectionException;
import org.vol.velocomp.VelocompActivity;

import java.util.concurrent.TimeoutException;

public class BikeConnectionThread extends GenericThread {

    public BikeConnectionThread(VelocompActivity velocompActivity) {
        super(velocompActivity, 2000);
    }

    @Override
    public void kill() {
        try {
            BikeConnection.getInstance().disconnect();
        } catch (ConnectionException e) {

        }
    }

    @Override
    public void perform() throws TimeoutException, ConnectionException {
        try {
            if (!BikeConnection.getInstance().testConnection(2000)) {
                velocompActivity.setConnected(false);
                BikeConnection.getInstance().connect();
                velocompActivity.setConnected(true);
                setConnectionStatus("Connected");
                //setStatus("OK");
            } else {
                velocompActivity.setConnected(true);
                setConnectionStatus("Connected");
                //setStatus("OK");
            }

        } catch (TimeoutException e) {
            setConnectionStatus("Disconnected");
            velocompActivity.setConnected(false);
            throw e;

        } catch (ConnectionException e) {
            setConnectionStatus("Disconnected");
            velocompActivity.setConnected(false);
            throw e;
        }
    }

    public void setConnectionStatus(final String status) {
        this.velocompActivity.runOnUiThread(new Runnable() {
            public void run() {
                velocompActivity.setConnectionStatus(status);
            }
        });
    }
}

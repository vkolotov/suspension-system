package org.vol.velocomp.threads;


import org.vol.velocomp.exceptions.ConnectionException;
import org.vol.velocomp.service.BikeService;

import java.util.concurrent.TimeoutException;

public class BikeConnectionThread extends GenericThread {

    public BikeConnectionThread() {
        super(3000);
    }

    @Override
    public void kill() {
        //BikeService.getInstance().disconnect(true);
    }

    @Override
    public void perform() throws TimeoutException, ConnectionException {
        if (!BikeService.getInstance().isConnected()) {
            BikeService.getInstance().connect();
        } else {
            BikeService.getInstance().testConnection();
        }
    }

}

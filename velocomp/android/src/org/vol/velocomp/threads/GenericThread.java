package org.vol.velocomp.threads;


import org.vol.velocomp.ConnectionException;
import org.vol.velocomp.VelocompActivity;

import java.util.concurrent.TimeoutException;

public abstract class GenericThread extends Thread {

    private boolean run = true;
    private long sleepTime;
    protected VelocompActivity velocompActivity;

    public GenericThread(VelocompActivity velocompActivity, long sleepTime) {
        this.velocompActivity = velocompActivity;
        this.sleepTime = sleepTime;
    }

    @Override
    public void run() {
        while (run) {

            try {
                perform();
            } catch (TimeoutException e) {
                //TODO logging
                //setStatus("Timeout: " + e.getMessage());
            } catch (ConnectionException e) {
                //TODO logging
                //setStatus("Connection error: " + e.getMessage());
            }


            try {
                Thread.sleep(sleepTime);
            } catch (InterruptedException e) {
                run = false;
            }

            if (!run) {
                kill();
            }
        }

    }

    public boolean isKilled() {
        return !run;
    }

    public void cancel() {
        run = false;
        //interrupt();
    }

    abstract public void kill() ;


    abstract public void perform() throws TimeoutException, ConnectionException;


}

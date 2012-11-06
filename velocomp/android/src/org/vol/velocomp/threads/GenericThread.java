package org.vol.velocomp.threads;


import org.vol.velocomp.exceptions.ConnectionException;

import java.util.concurrent.TimeoutException;

public abstract class GenericThread extends Thread {

    private boolean run = true;
    private long sleepTime;

    public GenericThread(long sleepTime) {
        this.sleepTime = sleepTime;
    }

    @Override
    public void run() {
        while (run) {

            try {
                perform();
            } catch (TimeoutException e) {
                //TODO logging
            } catch (ConnectionException e) {
                //TODO logging
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
    }

    abstract public void kill();


    abstract public void perform() throws TimeoutException, ConnectionException;


}

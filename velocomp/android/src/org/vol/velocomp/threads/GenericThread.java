package org.vol.velocomp.threads;


import android.util.Log;
import org.vol.velocomp.exceptions.ConnectionException;

import java.util.concurrent.TimeoutException;

public abstract class GenericThread extends Thread {

    private static final String TAG = GenericThread.class.getSimpleName();

    private boolean run = true;
    private long sleepTime;

    public GenericThread(long sleepTime) {
        this.sleepTime = sleepTime;
    }

    @Override
    public void run() {
        while (run) {
            Log.d(TAG, "Thread run: " + this.getClass());
            try {
                perform();
            } catch (TimeoutException e) {

            } catch (ConnectionException e) {

            }


            try {
                Thread.sleep(sleepTime);
            } catch (InterruptedException e) {
                run = false;
            }

            if (!run) {
                Log.d(TAG, "Killing thread: " + this.getClass());
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

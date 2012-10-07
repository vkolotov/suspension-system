package org.vol.velocomp.service;


import org.vol.velocomp.ConnectionException;
import org.vol.velocomp.messages.Configuration;

import java.util.concurrent.TimeoutException;

public class BikeService {

    public static final int TIMEOUT = 2000;

    public enum RequestCode {

        RESET(1),

        SAVE_CONFIGURATION(10),
        RELOAD_CONFIGURATION(11),
        RESET_CONFIGURATION(12),
        GET_CONFIGURATION(13),
        SEND_CONFIGURATION(14),
        SEND_FRONT_SUSPENSION_CONFIGURATION(15),
        SEND_REAR_SUSPENSION_CONFIGURATION(16),

        DESCEND_MODE_ALL(50),
        TRAIL_MODE_ALL(51),
        CLIMB_MODE_ALL(52);


        private byte code;
        RequestCode(int code) {
            this.code = (byte) code;
        }
    }

    private static BikeService instance;
    private static BikeServiceListener listener;
    private boolean isConnected;

    public interface BikeServiceListener {
        void onDisconnected(Exception ex);
        void onConnected();
    }

    private BikeService() {
    }

    synchronized public static BikeService getInstance() {
        if (instance == null) {
            instance = new BikeService();
            BikeConnection.getInstance().init();
        }
        return instance;
    }

    public static void setListener(BikeServiceListener listener) {
        BikeService.listener = listener;
    }

    synchronized public void connect() {
        try {
            BikeConnection.getInstance().connect(TIMEOUT);
            isConnected = true;
            listener.onConnected();
        } catch (ConnectionException e) {
            disconnect();
            listener.onDisconnected(e);
        }
    }

    synchronized public void disconnect() {
        try {
            BikeConnection.getInstance().disconnect();
            listener.onDisconnected(null);
        } catch (ConnectionException e) {
            listener.onDisconnected(e);
        }
        isConnected = false;
    }

    synchronized public boolean isDeviceFound() {
        return BikeConnection.getInstance().isConnected();
    }

    public boolean isConnected() {
        return isConnected;
    }

    synchronized public void testConnection() {
        try {
            BikeConnection.getInstance().testConnection(TIMEOUT);
            isConnected = true;
            listener.onConnected();
        } catch (TimeoutException e) {
            disconnect();
            listener.onDisconnected(e);
        } catch (ConnectionException e) {
            disconnect();
            listener.onDisconnected(e);
        }
    }

    public void setTrailModeAll() {
        sendMessage(RequestCode.TRAIL_MODE_ALL);
    }

    public void setClimbModeAll() {
        sendMessage(RequestCode.CLIMB_MODE_ALL);
    }

    public void setDescendModeAll() {
        sendMessage(RequestCode.DESCEND_MODE_ALL);
    }

    public Configuration getConfiguration() {
        return getMessage(Configuration.class, RequestCode.GET_CONFIGURATION);
    }

    public boolean sendConfiguration(Configuration configuration) {
        return sendMessage(RequestCode.SEND_CONFIGURATION, configuration);
    }

    public boolean sendFrontSuspensionConfiguration(Configuration.SuspensionSystemConfig configuration) {
        return sendMessage(RequestCode.SEND_FRONT_SUSPENSION_CONFIGURATION, configuration);
    }

    public boolean sendRearSuspensionConfiguration(Configuration.SuspensionSystemConfig configuration) {
        return sendMessage(RequestCode.SEND_REAR_SUSPENSION_CONFIGURATION, configuration);
    }

    synchronized private <Message> boolean sendMessage(RequestCode requestCode, Message message) {
        if (!isConnected) {
            return false;
        }
        try {
            BikeConnection.getInstance().send(requestCode.code, message, TIMEOUT);
            listener.onConnected();
            this.isConnected = true;
            return true;
        } catch (TimeoutException e) {
            disconnect();
            listener.onDisconnected(e);
        } catch (ConnectionException e) {
            disconnect();
            listener.onDisconnected(e);
        }
        return false;
    }

    synchronized private <Message> Message getMessage(Class<Message> clazz, RequestCode requestCode) {
        if (!isConnected) {
            return null;
        }
        try {
            Message result = BikeConnection.getInstance().request(clazz, requestCode.code, TIMEOUT);
            listener.onConnected();
            isConnected = true;
            return result;
        } catch (TimeoutException e) {
            disconnect();
            listener.onDisconnected(e);
        } catch (ConnectionException e) {
            disconnect();
            listener.onDisconnected(e);
        }
        return null;
    }

    synchronized private void sendMessage(RequestCode requestCode) {
        if (!isConnected) {
            return;
        }
        try {
            BikeConnection.getInstance().sendMessageId(requestCode.code);
            isConnected = true;
            listener.onConnected();
        } catch (ConnectionException e) {
            disconnect();
            listener.onDisconnected(e);
        }
    }

}

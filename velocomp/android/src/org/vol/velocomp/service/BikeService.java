package org.vol.velocomp.service;


import org.vol.velocomp.ConnectionException;
import org.vol.velocomp.messages.*;

import java.util.HashSet;
import java.util.Set;
import java.util.concurrent.TimeoutException;

public class BikeService {

    public static final int TIMEOUT = 2000;

    public enum RequestCode {

        RESET(1),
        SLEEP(2),

        SAVE_CONFIGURATION(10),
        RELOAD_CONFIGURATION(11),
        RESET_CONFIGURATION(12),
        GET_CONFIGURATION(13),
        SEND_CONFIGURATION(14),
        SEND_FRONT_SUSPENSION_CONFIGURATION(15),
        SEND_REAR_SUSPENSION_CONFIGURATION(16),
        SEND_CDT_BOARD_MESSAGE(17),
        SEND_AUTOMATIC_BOARD_MESSAGE(18),

        DESCEND_MODE_ALL(50),
        TRAIL_MODE_ALL(51),
        CLIMB_MODE_ALL(52),
        SET_MANUAL_MODE(53),
        SET_CDT_MODE(54),
        SET_AUTOMATIC_MODE(55),
        CALIBRATE_GRADIENT(56),


        GET_MANUAL_TELEMETRY(60),
        GET_CDT_TELEMETRY(61),
        GET_AUTOMATIC_TELEMETRY(62);


        private byte code;
        RequestCode(int code) {
            this.code = (byte) code;
        }
    }

    private static BikeService instance;
    private Set<BikeServiceListener> listeners = new HashSet<BikeServiceListener>(5);
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

    public void addListener(BikeServiceListener listener) {
        listeners.add(listener);
    }

    private void notifyListeners(boolean isConnected, Exception ex) {
        for (BikeServiceListener bikeServiceListener : listeners) {
            if (isConnected) {
                bikeServiceListener.onConnected();
            } else {
                bikeServiceListener.onDisconnected(ex);
            }
        }
    }

    synchronized public void connect() {
        try {
            BikeConnection.getInstance().connect(TIMEOUT);
            BikeConnection.getInstance().testConnection(TIMEOUT);
            isConnected = true;
            notifyListeners(true, null);
        } catch (ConnectionException e) {
            disconnect();
            notifyListeners(false, e);
        } catch (TimeoutException e) {
            disconnect();
            notifyListeners(false, e);
        }
    }

    synchronized public void disconnect() {
        try {
            BikeConnection.getInstance().disconnect();
            notifyListeners(false, null);
        } catch (ConnectionException e) {
            notifyListeners(false, e);
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
            notifyListeners(true, null);
        } catch (TimeoutException e) {
            disconnect();
            notifyListeners(false, e);
        } catch (ConnectionException e) {
            disconnect();
            notifyListeners(false, e);
        }
    }

    public boolean setTrailModeAll() {
        return sendMessage(RequestCode.TRAIL_MODE_ALL);
    }

    public boolean setClimbModeAll() {
        return sendMessage(RequestCode.CLIMB_MODE_ALL);
    }

    public boolean setDescendModeAll() {
        return sendMessage(RequestCode.DESCEND_MODE_ALL);
    }

    public boolean sleep() {
        return sendMessage(RequestCode.SLEEP);
    }

    public boolean reset() {
        return sendMessage(RequestCode.RESET);
    }

    public Configuration getConfiguration() {
        return getMessage(Configuration.class, RequestCode.GET_CONFIGURATION);
    }

    public boolean sendConfiguration(Configuration configuration) {
        return sendMessage(RequestCode.SEND_CONFIGURATION, configuration);
    }

    public boolean sendCDTBoardMessage(CDTBoardMessage configuration) {
        return sendMessage(RequestCode.SEND_CDT_BOARD_MESSAGE, configuration);
    }

    public boolean sendAutomaticMessage(AutomaticBoardMessage configuration) {
        return sendMessage(RequestCode.SEND_AUTOMATIC_BOARD_MESSAGE, configuration);
    }

    public boolean saveConfiguration() {
        return sendMessage(RequestCode.SAVE_CONFIGURATION);
    }

    public boolean setManualMode() {
        return sendMessage(RequestCode.SET_MANUAL_MODE);
    }

    public boolean setCDTMode() {
        return sendMessage(RequestCode.SET_CDT_MODE);
    }

    public boolean setAutomaticMode() {
        return sendMessage(RequestCode.SET_AUTOMATIC_MODE);
    }

    public boolean calibrateGradient() {
        return sendMessage(RequestCode.CALIBRATE_GRADIENT);
    }

    public boolean sendFrontSuspensionConfiguration(Configuration.SuspensionSystemConfig configuration) {
        return sendMessage(RequestCode.SEND_FRONT_SUSPENSION_CONFIGURATION, configuration);
    }

    public boolean sendRearSuspensionConfiguration(Configuration.SuspensionSystemConfig configuration) {
        return sendMessage(RequestCode.SEND_REAR_SUSPENSION_CONFIGURATION, configuration);
    }

    public ManualTelemetry getManualTelemetry() {
        return getMessage(ManualTelemetry.class, RequestCode.GET_MANUAL_TELEMETRY);
    }

    public CDTTelemetry getCDTTelemetry() {
        return getMessage(CDTTelemetry.class, RequestCode.GET_CDT_TELEMETRY);
    }

    public AutomaticTelemetry getAutomaticTelemetry() {
        return getMessage(AutomaticTelemetry.class, RequestCode.GET_AUTOMATIC_TELEMETRY);
    }

    synchronized private <Message> boolean sendMessage(RequestCode requestCode, Message message) {
        if (!isConnected) {
            return false;
        }
        try {
            BikeConnection.getInstance().send(requestCode.code, message, TIMEOUT);
            notifyListeners(true, null);
            this.isConnected = true;
            return true;
        } catch (TimeoutException e) {
            disconnect();
            notifyListeners(false, e);
        } catch (ConnectionException e) {
            disconnect();
            notifyListeners(false, e);
        }
        return false;
    }

    synchronized private <Message> Message getMessage(Class<Message> clazz, RequestCode requestCode) {
        if (!isConnected) {
            return null;
        }
        try {
            Message result = BikeConnection.getInstance().request(clazz, requestCode.code, TIMEOUT);
            notifyListeners(true, null);
            isConnected = true;
            return result;
        } catch (TimeoutException e) {
            disconnect();
            notifyListeners(false, e);
        } catch (ConnectionException e) {
            disconnect();
            notifyListeners(false, e);
        }
        return null;
    }

    synchronized private boolean sendMessage(RequestCode requestCode) {
        if (!isConnected) {
            return false;
        }
        try {
            BikeConnection.getInstance().sendMessageId(requestCode.code);
            isConnected = true;
            notifyListeners(true, null);
            return true;
        } catch (ConnectionException e) {
            disconnect();
            notifyListeners(false, e);
            return false;
        }
    }

}

package org.vol.velocomp.service;


import org.vol.velocomp.exceptions.*;
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

    abstract class InvocationHandler<T> {

        abstract T invoke() throws BikeCommunicationException, InterruptedException, TimeoutException;

        T handle() {
            if (!isConnected) {
                return null;
            }
            try {
                return invoke();
            } catch (BikeCommunicationException e) {
                disconnect(false);
                for (BikeServiceListener bikeServiceListener : listeners) {
                    bikeServiceListener.exception(e);
                }
            } catch (InterruptedException e) {
                disconnect(false);
                for (BikeServiceListener bikeServiceListener : listeners) {
                    bikeServiceListener.exception(e);
                }
            } catch (TimeoutException e) {
                disconnect(false);
                for (BikeServiceListener bikeServiceListener : listeners) {
                    bikeServiceListener.exception(e);
                }
            }
            return null;
        }

    }

    abstract class BooleanInvocationHandler extends InvocationHandler<Boolean> {
        @Override
        Boolean handle() {
            if (!isConnected) {
                return false;
            }
            Boolean result = super.handle();
            return result != null ? result : false;
        }
    }

    private static BikeService instance;
    private Set<BikeServiceListener> listeners = new HashSet<BikeServiceListener>(5);
    private boolean isConnected;

    public static class BikeServiceListener {
        public void exception(BikeCommunicationException ex) {}
        public void exception(BikeNotFoundException ex) {}
        public void exception(BikeNotPairedException ex) {}
        public void exception(BluetoothNotReadyException ex) {}
        public void exception(SocketAcquiringException ex) {}
        public void exception(SocketConnectionException ex) {}
        public void exception(StreamConnectionException ex) {}
        public void exception(TimeoutException ex) {}
        public void exception(InterruptedException ex) {}
        public void connect() {}
        public void disconnect() {}
    }

    public static class BasicBikeServiceListener extends BikeServiceListener {
        public void exception(BikeCommunicationException ex) { disconnect();}
        public void exception(BikeNotFoundException ex) { disconnect();}
        public void exception(BikeNotPairedException ex) { disconnect();}
        public void exception(BluetoothNotReadyException ex) { disconnect();}
        public void exception(SocketAcquiringException ex) { disconnect();}
        public void exception(SocketConnectionException ex) { disconnect();}
        public void exception(StreamConnectionException ex) { disconnect();}
        public void exception(TimeoutException ex) { disconnect();}
        public void exception(InterruptedException ex) { disconnect();}
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

    synchronized public void connect() {
        try {
            BikeConnection.getInstance().connect(TIMEOUT);
            BikeConnection.getInstance().testConnection(TIMEOUT);
            isConnected = true;
            for (BikeServiceListener bikeServiceListener : listeners) {
                bikeServiceListener.connect();
            }
        } catch (BikeCommunicationException e) {
            disconnect(false);
            for (BikeServiceListener bikeServiceListener : listeners) {
                bikeServiceListener.exception(e);
            }
        } catch (BikeNotFoundException e) {
            disconnect(false);
            for (BikeServiceListener bikeServiceListener : listeners) {
                bikeServiceListener.exception(e);
            }
        } catch (BikeNotPairedException e) {
            disconnect(false);
            for (BikeServiceListener bikeServiceListener : listeners) {
                bikeServiceListener.exception(e);
            }
        } catch (BluetoothNotReadyException e) {
            disconnect(false);
            for (BikeServiceListener bikeServiceListener : listeners) {
                bikeServiceListener.exception(e);
            }
        } catch (SocketAcquiringException e) {
            disconnect(false);
            for (BikeServiceListener bikeServiceListener : listeners) {
                bikeServiceListener.exception(e);
            }
        } catch (SocketConnectionException e) {
            disconnect(false);
            for (BikeServiceListener bikeServiceListener : listeners) {
                bikeServiceListener.exception(e);
            }
        } catch (StreamConnectionException e) {
            disconnect(false);
            for (BikeServiceListener bikeServiceListener : listeners) {
                bikeServiceListener.exception(e);
            }
        } catch (TimeoutException e) {
            disconnect(false);
            for (BikeServiceListener bikeServiceListener : listeners) {
                bikeServiceListener.exception(e);
            }
        } catch (InterruptedException e) {
            disconnect(false);
            for (BikeServiceListener bikeServiceListener : listeners) {
                bikeServiceListener.exception(e);
            }
        } catch (ConnectionException e) {
            disconnect(false);
            throw new IllegalStateException(e);
        }
    }

    synchronized public void disconnect(boolean notifyListeners) {
        BikeConnection.getInstance().disconnect();
        isConnected = false;
        if (notifyListeners) {
            for (BikeServiceListener bikeServiceListener : listeners) {
                bikeServiceListener.disconnect();
            }
        }
    }

    public boolean isConnected() {
        return isConnected;
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

    synchronized public boolean testConnection() {
        return new BooleanInvocationHandler() {
            @Override
            Boolean invoke() throws BikeCommunicationException, InterruptedException, TimeoutException {
                BikeConnection.getInstance().testConnection(TIMEOUT);
                return true;
            }
        }.handle();
    }

    synchronized private <Message> boolean sendMessage(final RequestCode requestCode, final Message message) {
        return new BooleanInvocationHandler() {
            @Override
            Boolean invoke() throws BikeCommunicationException, InterruptedException, TimeoutException {
                BikeConnection.getInstance().send(requestCode.code, message, TIMEOUT);
                return true;
            }
        }.handle();
    }

    synchronized private <Message> Message getMessage(final Class<Message> clazz, final RequestCode requestCode) {
        return new InvocationHandler<Message>() {
            @Override
            Message invoke() throws BikeCommunicationException, InterruptedException, TimeoutException {
                return BikeConnection.getInstance().request(clazz, requestCode.code, TIMEOUT);
            }
        }.handle();
    }

    synchronized private boolean sendMessage(final RequestCode requestCode) {
        return new BooleanInvocationHandler() {
            @Override
            Boolean invoke() throws BikeCommunicationException, InterruptedException, TimeoutException {
                BikeConnection.getInstance().sendMessageId(requestCode.code);
                return true;
            }
        }.handle();
    }

}

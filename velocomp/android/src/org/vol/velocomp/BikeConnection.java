package org.vol.velocomp;


import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import org.vol.velocomp.messages.BikeMessageSerializer;
import org.vol.velocomp.messages.StatusMessage;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Set;
import java.util.concurrent.*;

public class BikeConnection {

    private BluetoothAdapter bluetoothAdapter;
    private BluetoothDevice bikeBluetooth;
    private BluetoothSocket bluetoothSocket;
    private DataInputStream inputStream;
    private DataOutputStream outputStream;
    private ExecutorService executorService;

    private static BikeConnection instance;

    synchronized public void init() {
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    }

    synchronized public static BikeConnection getInstance() {
        if (instance == null) {
            instance = new BikeConnection();
        }
        return instance;
    }

    private BikeConnection() {
    }

    synchronized public void connect() throws ConnectionException {

        if (isConnected()) {
            disconnect();
        }

        this.executorService = Executors.newSingleThreadExecutor();

        this.bikeBluetooth = getBikeDevice();

        this.bluetoothSocket = getBluetoothSocket();

        try {
            this.bluetoothSocket.connect();
        } catch (IOException e) {
            throw new ConnectionException("Could not open socket with bike");
        }
        try {
            this.inputStream = new DataInputStream(this.bluetoothSocket.getInputStream());
            this.outputStream = new DataOutputStream(this.bluetoothSocket.getOutputStream());
        } catch (IOException e) {
            throw new ConnectionException("Could not get input/output stream");
        }
    }

    synchronized public void disconnect() throws ConnectionException {

        this.executorService.shutdownNow();

        try {
            if (this.outputStream != null) {
                this.outputStream.close();
                this.outputStream = null;
            }
            if (this.inputStream != null) {
                this.inputStream.close();
                this.inputStream = null;
            }
            if (this.bluetoothSocket != null) {
                this.bluetoothSocket.close();
                this.bluetoothSocket = null;
            }
        } catch (IOException e) {
            throw new ConnectionException(e);
        }
    }

    synchronized public boolean testConnection(long timeout) throws TimeoutException {
        if (!isConnected()) {
            return false;
        }
        try {
            sendMessageId((byte) 0);
            return receive(StatusMessage.class, timeout).status == StatusMessage.OK;
        } catch (ConnectionException e) {
            return false;
        }
    }


    synchronized public <Request> void send(byte requestId, final Request message, long timeout)
            throws TimeoutException, ConnectionException {
        try {
            this.inputStream.skipBytes(this.inputStream.available());
        } catch (IOException e) {
            throw new ConnectionException(e.getMessage());
        }
        sendMessageId(requestId);
        checkStatus(timeout);
        sendMessage(message);
        checkStatus(timeout);
    }

    synchronized public <Response> Response request(
            final Class<Response> clazz, byte requestId, long timeout)
            throws TimeoutException, ConnectionException {
        sendMessageId(requestId);
        Response result = receive(clazz, timeout);
        checkExcessiveBytes();
        return result;
    }

    synchronized public <Response> Response receive(final Class<Response> clazz, long timeout)
            throws TimeoutException, ConnectionException {

        Future<Response> future = executorService.submit( new Callable<Response>() {
            @Override
            public Response call() throws Exception {

                Response result = null;
                //while (isConnected() && !Thread.currentThread().isInterrupted()) {
                    result = receiveMessage(clazz);
                //    if (result != null) {
                //        break;
                //    } else {
                //        Thread.sleep(100);
                //    }
                //}
                return result;
            }
        });


        try {
            return future.get(timeout, TimeUnit.MILLISECONDS);
        } catch (InterruptedException e) {
            throw new ConnectionException("Interrupted", e);
        } catch (ExecutionException e) {
            throw new ConnectionException("Execution", e);
        }
    }

    private void checkStatus(long timeout) throws TimeoutException, ConnectionException {
        StatusMessage statusMessage = receive(StatusMessage.class, timeout);
        if (statusMessage.status != StatusMessage.OK) {
            throw new ConnectionException("Reported error: " + statusMessage.status);
        }
    }

    private void sendMessageId(byte messageId) throws ConnectionException {
        checkConnection();
        try {
            this.outputStream.write(messageId);
            this.outputStream.flush();
        } catch (IOException e) {
            throw new ConnectionException("Was unable to send message ID", e);
        }
    }

    private <Request> void sendMessage(Request request) throws ConnectionException {
        checkConnection();
        try {
            BikeMessageSerializer.serialize(this.outputStream, request);
            this.outputStream.flush();
        } catch (IOException e) {
            throw new ConnectionException("Could not send message", e);
        }
    }

    private <Response> Response receiveMessage(final Class<Response> clazz) throws ConnectionException {
        checkConnection();
        try {
            return BikeMessageSerializer.deserialize(clazz, this.inputStream);
        } catch (IOException e) {
            throw new ConnectionException("Could not receive message", e);
        }
    }

    private BluetoothDevice getBikeDevice() throws ConnectionException {
        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice device : pairedDevices) {
                if (device.getName().equals("Bike")) {
                    return device;
                }
            }
        }
        throw new ConnectionException("Could not find bike bluetooth device");
    }

    private BluetoothSocket getBluetoothSocket() throws ConnectionException {
        if (this.bikeBluetooth == null || this.bikeBluetooth.getBondState() != BluetoothDevice.BOND_BONDED) {
            throw new ConnectionException("Bike device is not initialized");
        }

        Method method = null;
        try {
            method = bikeBluetooth.getClass().getMethod("createRfcommSocket", new Class[]{int.class});
            return (BluetoothSocket) method.invoke(bikeBluetooth, Integer.valueOf(1));
        } catch (NoSuchMethodException e) {
            throw new IllegalStateException(e);
        } catch (InvocationTargetException e) {
            throw new ConnectionException(e);
        } catch (IllegalAccessException e) {
            throw new IllegalStateException(e);
        }

    }

    private boolean isConnected() {
        return bluetoothSocket != null && outputStream != null && inputStream != null;
    }

    private void checkConnection() throws ConnectionException {
        if (!isConnected()) {
            throw new ConnectionException("Is not connected");
        }
    }

    private void checkExcessiveBytes() throws ConnectionException {
        try {
            if (this.inputStream.available() > 0) {
                throw new ConnectionException("Something is available after reading");
                //this.inputStream.skipBytes(this.inputStream.available());
            }
        } catch (IOException e) {
            throw new ConnectionException(e);
        }
    }

}

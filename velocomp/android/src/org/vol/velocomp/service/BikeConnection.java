package org.vol.velocomp.service;


import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.util.Log;
import org.vol.velocomp.exceptions.*;
import org.vol.velocomp.messages.BikeMessageSerializer;
import org.vol.velocomp.messages.ReceivedSize;
import org.vol.velocomp.messages.StatusMessage;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Set;
import java.util.concurrent.*;

public class BikeConnection {

    private static final String TAG = BikeConnection.class.getSimpleName();

    private BluetoothAdapter bluetoothAdapter;
    private BluetoothDevice bikeBluetooth;
    private BluetoothSocket bluetoothSocket;
    private DataInputStream inputStream;
    private DataOutputStream outputStream;
    private ExecutorService executorService;

    private static BikeConnection instance;

    public void init() {
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    }

    public static BikeConnection getInstance() {
        if (instance == null) {
            instance = new BikeConnection();
        }
        return instance;
    }

    private BikeConnection() {
    }

    public void connect(long timeout)
            throws ConnectionException, InterruptedException, TimeoutException {

        if (isConnected()) {
            disconnect();
        }

        this.executorService = Executors.newSingleThreadExecutor();

//        Future future = executorService.submit( new Callable() {
//            @Override
//            public Object call() throws ConnectionException {
                checkBluetoothAdapter();
                bikeBluetooth = getBikeDevice();
                checkBluetoothDevice();
                bluetoothSocket = getBluetoothSocket();

                try {
                    bluetoothSocket.connect();
                } catch (IOException e) {
                    disconnect();
                    Log.e(TAG, e.getMessage(), e);
                    throw new SocketConnectionException("Could not open socket with bike: " + e.getMessage());
                }
                try {
                    inputStream = new DataInputStream(bluetoothSocket.getInputStream());
                    outputStream = new DataOutputStream(bluetoothSocket.getOutputStream());
                } catch (IOException e) {
                    disconnect();
                    Log.e(TAG, e.getMessage(), e);
                    throw new StreamConnectionException("Could not get input/output stream: " + e.getMessage());
                }
//                return null;
//            }
//        });
//
//        try {
//            future.get(timeout, TimeUnit.MILLISECONDS);
//        } catch (InterruptedException e) {
//            Log.e(TAG, e.getMessage(), e);
//            disconnect();
//            throw e;
//        } catch (ExecutionException e) {
//            Log.e(TAG, e.getMessage(), e);
//            disconnect();
//            Throwable cause = e.getCause();
//            if (cause instanceof ConnectionException) {
//                throw (ConnectionException) cause;
//            } else {
//                throw new IllegalStateException(cause);
//            }
//        } catch (TimeoutException e) {
//            Log.e(TAG, e.getMessage(), e);
//            disconnect();
//            throw new TimeoutException("Connection timeout");
//        }
    }

    public void disconnect() {

        if (this.executorService != null) {
            this.executorService.shutdownNow();
        }

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
            Log.e(TAG, e.getMessage(), e);
            throw new IllegalStateException(e);
        }
    }

    public void testConnection(long timeout) throws TimeoutException, BikeCommunicationException, InterruptedException {
        if (!isConnected()) {
            throw new IllegalStateException("Is not connected");
        }
        sendMessageId((byte) 0);
        StatusMessage statusMessage = receive(StatusMessage.class, timeout);
        if (!statusMessage.isOk()) {
            throw new BikeCommunicationException("Bike reported error: " + statusMessage.status);
        }
    }

    public <Request> void send(byte requestId, final Request message, long timeout)
            throws TimeoutException, BikeCommunicationException, InterruptedException {
        try {
            this.inputStream.skipBytes(this.inputStream.available());
        } catch (IOException e) {
            Log.e(TAG, e.getMessage(), e);
            throw new IllegalStateException(e);
        }
        sendMessageId(requestId);
        checkStatus(timeout);
        int messageSize = BikeMessageSerializer.getSizeOf(message.getClass());
        int writtenSize = sendMessage(message);
        if (writtenSize != messageSize) {
            throw new BikeCommunicationException("Written size is wrong: expected " + messageSize + " but written " + writtenSize);
        }
        checkReceivedSize((short) messageSize, timeout);
    }

    public <Response> Response request(
            final Class<Response> clazz, byte requestId, long timeout)
            throws TimeoutException, BikeCommunicationException, InterruptedException {
        sendMessageId(requestId);
        Response result = receive(clazz, timeout);
        checkExcessiveBytes();
        return result;
    }

    public <Response> Response receive(final Class<Response> clazz, long timeout)
            throws TimeoutException, InterruptedException, BikeCommunicationException {

        Future<Response> future = executorService.submit( new Callable<Response>() {
            @Override
            public Response call() throws Exception {
                return receiveMessage(clazz);
            }
        });

        try {
            return future.get(timeout, TimeUnit.MILLISECONDS);
        } catch (InterruptedException e) {
            disconnect();
            Log.e(TAG, e.getMessage(), e);
            throw e;
        } catch (ExecutionException e) {
            disconnect();
            Log.e(TAG, e.getMessage(), e);
            Throwable cause = e.getCause();
            if (cause instanceof BikeCommunicationException) {
                throw (BikeCommunicationException) cause;
            } else {
                throw new IllegalStateException(cause);
            }
        }
    }

    private void checkStatus(long timeout) throws TimeoutException, BikeCommunicationException, InterruptedException {
        StatusMessage statusMessage = receive(StatusMessage.class, timeout);
        if (statusMessage.status != StatusMessage.OK) {
            throw new BikeCommunicationException("Reported error: " + statusMessage.status);
        }
    }

    private void checkReceivedSize(short expected, long timeout)
            throws TimeoutException, InterruptedException, BikeCommunicationException, BikeCommunicationException {
        ReceivedSize receivedSize = receive(ReceivedSize.class, timeout);
        if (receivedSize.bytes != expected) {
            throw new BikeCommunicationException("Received size is wrong: expected "
                    + expected + " but received: " + receivedSize.bytes);
        }
    }

    public void sendMessageId(byte messageId) throws BikeCommunicationException {
        checkConnection();
        try {
            this.outputStream.write(messageId);
            this.outputStream.flush();
        } catch (IOException e) {
            Log.e(TAG, e.getMessage(), e);
            throw new BikeCommunicationException("Was unable to send message ID", e);
        }
    }

    private <Request> int sendMessage(Request request) throws BikeCommunicationException {
        checkConnection();
        try {
            int writtenSize = this.outputStream.size();
            BikeMessageSerializer.serialize(this.outputStream, request);
            this.outputStream.flush();
            return this.outputStream.size() - writtenSize;
        } catch (IOException e) {
            Log.e(TAG, e.getMessage(), e);
            throw new BikeCommunicationException("Could not send message", e);
        }
    }

    private <Response> Response receiveMessage(final Class<Response> clazz) throws BikeCommunicationException {
        checkConnection();
        try {
            return BikeMessageSerializer.deserialize(clazz, this.inputStream);
        } catch (IOException e) {
            Log.e(TAG, e.getMessage(), e);
            throw new BikeCommunicationException("Could not receive message", e);
        }
    }

    private void checkBluetoothAdapter() throws BluetoothNotReadyException {
        switch (bluetoothAdapter.getState()) {
            case BluetoothAdapter.STATE_OFF:
                throw new BluetoothNotReadyException("Bluetooth adapter is off");
            case BluetoothAdapter.STATE_TURNING_OFF:
                throw new BluetoothNotReadyException("Bluetooth adapter is  turning off");
            case BluetoothAdapter.STATE_TURNING_ON:
                throw new BluetoothNotReadyException("Bluetooth adapter is turning on");
        }
    }

    private void checkBluetoothDevice() throws BikeNotPairedException {
        switch (bikeBluetooth.getBondState()) {
            case BluetoothDevice.BOND_NONE:
                throw new BikeNotPairedException("Bike is not paired");
            case BluetoothDevice.BOND_BONDING:
                throw new BikeNotPairedException("Bike is being paired");
        }
    }

    private BluetoothDevice getBikeDevice() throws BikeNotFoundException, BluetoothNotReadyException {

        String bikeName = "HC-05";

        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice device : pairedDevices) {
                if (device.getName().equals(bikeName)) {
                    return device;
                }
            }
        }
        throw new BikeNotFoundException("Could not find bike bluetooth device: " + bikeName);
    }

    private BluetoothSocket getBluetoothSocket() throws BikeNotPairedException, SocketAcquiringException {
        if (this.bikeBluetooth == null) {
            throw new IllegalStateException("Bike device is not initialized");
        }

        if (this.bikeBluetooth.getBondState() != BluetoothDevice.BOND_BONDED) {
            throw new BikeNotPairedException("Bike device is not paired");
        }

        try {
            Method method = bikeBluetooth.getClass().getMethod("createRfcommSocket", new Class[]{int.class});
            return (BluetoothSocket) method.invoke(bikeBluetooth, Integer.valueOf(1));
        } catch (NoSuchMethodException e) {
            Log.e(TAG, e.getMessage(), e);
            throw new IllegalStateException(e);
        } catch (InvocationTargetException e) {
            Log.e(TAG, e.getMessage(), e);
            throw new SocketAcquiringException(e.getCause());
        } catch (IllegalAccessException e) {
            Log.e(TAG, e.getMessage(), e);
            throw new IllegalStateException(e);
        }

    }

    public boolean isConnected() {
        return bluetoothSocket != null && outputStream != null && inputStream != null;
    }

    private void checkConnection() {
        if (!isConnected()) {
            throw new IllegalStateException("Is not connected");
        }
    }

    private void checkExcessiveBytes() throws BikeCommunicationException {
        try {
            if (this.inputStream.available() > 0) {
                this.inputStream.skipBytes(this.inputStream.available());
                throw new BikeCommunicationException("Something is available after reading");
            }
        } catch (IOException e) {
            Log.e(TAG, e.getMessage(), e);
            throw new BikeCommunicationException(e);
        }
    }

}

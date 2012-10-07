package org.vol.velocomp.service;


import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import org.vol.velocomp.ConnectionException;
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

    public void connect(long timeout) throws ConnectionException {

        if (isConnected()) {
            disconnect();
        }

        this.executorService = Executors.newSingleThreadExecutor();

        Future future = executorService.submit( new Callable() {
            @Override
            public Object call() throws ConnectionException {
                bikeBluetooth = getBikeDevice();
                bluetoothSocket = getBluetoothSocket();
                try {
                    bluetoothSocket.connect();
                } catch (IOException e) {
                    throw new ConnectionException("Could not open socket with bike");
                }
                try {
                    inputStream = new DataInputStream(bluetoothSocket.getInputStream());
                    outputStream = new DataOutputStream(bluetoothSocket.getOutputStream());
                } catch (IOException e) {
                    throw new ConnectionException("Could not get input/output stream");
                }
                return null;
            }
        });

        try {
            future.get(timeout, TimeUnit.MILLISECONDS);
        } catch (InterruptedException e) {
            throw new ConnectionException("Interrupted", e);
        } catch (ExecutionException e) {
            throw new ConnectionException("Execution", e);
        } catch (TimeoutException e) {
            throw new ConnectionException("Timeout", e);
        }
    }

    public void disconnect() throws ConnectionException {

        if (!isConnected()) {
            return;
        }

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

    public void testConnection(long timeout) throws TimeoutException, ConnectionException {
        if (!isConnected()) {
            throw new ConnectionException("Is not connected");
        }
        sendMessageId((byte) 0);
        if (receive(StatusMessage.class, timeout).status != StatusMessage.OK) {
            throw new ConnectionException("Device is not ready");
        }
    }

    public <Request> void send(byte requestId, final Request message, long timeout)
            throws TimeoutException, ConnectionException {
        try {
            this.inputStream.skipBytes(this.inputStream.available());
        } catch (IOException e) {
            throw new ConnectionException(e.getMessage());
        }
        sendMessageId(requestId);
        checkStatus(timeout);
        int messageSize = BikeMessageSerializer.getSizeOf(message.getClass());
        int writtenSize = sendMessage(message);
        if (writtenSize != messageSize) {
            throw new ConnectionException("Written size is wrong: expected " + messageSize + " but written " + writtenSize);
        }
        checkReceivedSize((short) messageSize, timeout);
    }

    public <Response> Response request(
            final Class<Response> clazz, byte requestId, long timeout)
            throws TimeoutException, ConnectionException {
        sendMessageId(requestId);
        Response result = receive(clazz, timeout);
        checkExcessiveBytes();
        return result;
    }

    public <Response> Response receive(final Class<Response> clazz, long timeout)
            throws TimeoutException, ConnectionException {

        Future<Response> future = executorService.submit( new Callable<Response>() {
            @Override
            public Response call() throws Exception {
                return receiveMessage(clazz);
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

    private void checkReceivedSize(short expected, long timeout) throws TimeoutException, ConnectionException {
        ReceivedSize receivedSize = receive(ReceivedSize.class, timeout);
        if (receivedSize.bytes != expected) {
            throw new ConnectionException("Received size is wrong: expected "
                    + expected + " but received: " + receivedSize.bytes);
        }
    }

    public void sendMessageId(byte messageId) throws ConnectionException {
        checkConnection();
        try {
            this.outputStream.write(messageId);
            this.outputStream.flush();
        } catch (IOException e) {
            throw new ConnectionException("Was unable to send message ID", e);
        }
    }

    private <Request> int sendMessage(Request request) throws ConnectionException {
        checkConnection();
        try {
            int writtenSize = this.outputStream.size();
            BikeMessageSerializer.serialize(this.outputStream, request);
            this.outputStream.flush();
            return this.outputStream.size() - writtenSize;
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
                if (device.getName().equals("HC-05")) {
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

        try {
            Method method = bikeBluetooth.getClass().getMethod("createRfcommSocket", new Class[]{int.class});
            return (BluetoothSocket) method.invoke(bikeBluetooth, Integer.valueOf(1));
        } catch (NoSuchMethodException e) {
            throw new IllegalStateException(e);
        } catch (InvocationTargetException e) {
            throw new ConnectionException(e);
        } catch (IllegalAccessException e) {
            throw new IllegalStateException(e);
        }

    }

    public boolean isConnected() {
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
                this.inputStream.skipBytes(this.inputStream.available());
                throw new ConnectionException("Something is available after reading");
            }
        } catch (IOException e) {
            throw new ConnectionException(e);
        }
    }

}

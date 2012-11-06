package org.vol.velocomp.exceptions;

public class BluetoothNotReadyException extends ConnectionException {

    public BluetoothNotReadyException() {
    }

    public BluetoothNotReadyException(String detailMessage) {
        super(detailMessage);
    }

    public BluetoothNotReadyException(String detailMessage, Throwable throwable) {
        super(detailMessage, throwable);
    }

    public BluetoothNotReadyException(Throwable throwable) {
        super(throwable);
    }
}

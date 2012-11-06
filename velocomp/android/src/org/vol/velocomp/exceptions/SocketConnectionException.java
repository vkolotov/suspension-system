package org.vol.velocomp.exceptions;

public class SocketConnectionException extends ConnectionException {

    public SocketConnectionException() {
    }

    public SocketConnectionException(String detailMessage) {
        super(detailMessage);
    }

    public SocketConnectionException(String detailMessage, Throwable throwable) {
        super(detailMessage, throwable);
    }

    public SocketConnectionException(Throwable throwable) {
        super(throwable);
    }
}

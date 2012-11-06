package org.vol.velocomp.exceptions;

public class StreamConnectionException extends ConnectionException {

    public StreamConnectionException() {
    }

    public StreamConnectionException(String detailMessage) {
        super(detailMessage);
    }

    public StreamConnectionException(String detailMessage, Throwable throwable) {
        super(detailMessage, throwable);
    }

    public StreamConnectionException(Throwable throwable) {
        super(throwable);
    }
}
